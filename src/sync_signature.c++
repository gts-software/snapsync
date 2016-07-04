#include "sync.h++"
#include <fstream>
#include <librsync.h>
#include <cryptopp/sha.h>

using namespace std;

namespace snapsync { namespace sync {

  void signature(std::istream& base, std::ostream& signature, size_t blockLength, size_t sumLength) {

    if(!blockLength) {
      blockLength = RS_DEFAULT_BLOCK_LEN;
    }

    if(!sumLength) {
      sumLength = RS_BLAKE2_SUM_LENGTH;
    }

    // set exceptions flags
    auto baseOldExceptions = base.exceptions();
    base.exceptions(istream::failbit | istream::badbit);

    auto signatureOldExceptions = signature.exceptions();
    signature.exceptions(ostream::failbit | ostream::badbit);

    // keep space for hashes
    signature.seekp(CryptoPP::SHA1::DIGESTSIZE * 2, ios::cur);

    // read base and write signature
    constexpr size_t BUFFER_SIZE = 1024;
    char buffer_in[BUFFER_SIZE];
    char buffer_out[BUFFER_SIZE];

    rs_buffers_t buffer;
    buffer.next_out = buffer_out;
    buffer.avail_out = BUFFER_SIZE;
    buffer.next_in = buffer_in;
    buffer.avail_in = 0;
    buffer.eof_in = 0;

    CryptoPP::SHA1 hashBase;
    CryptoPP::SHA1 hashSig;

    // run job
    auto job = rs_sig_begin(blockLength, sumLength, RS_BLAKE2_SIG_MAGIC);

    while(true) {

      // fill input buffer
      if(!buffer.eof_in && (BUFFER_SIZE - buffer.avail_in) > 0) {
        auto count = base.rdbuf()->sgetn(buffer_in + buffer.avail_in, BUFFER_SIZE - buffer.avail_in);
        hashBase.Update(reinterpret_cast<const byte*>(buffer_in + buffer.avail_in), count);
        buffer.avail_in += count;
        buffer.eof_in = (count == 0);
      }

      // perform iteration
      auto status = rs_job_iter(job, &buffer);

      if(status != RS_DONE && status != RS_BLOCKED) {
        throw std::runtime_error("could not create signature");
      }

      // forward input buffer
      if(buffer.avail_in > 0) {
        std::memmove(buffer_in, buffer.next_in, buffer.avail_in);
      }
      buffer.next_in = buffer_in;

      // write output buffer
      if(buffer.avail_out < BUFFER_SIZE) {
        signature.write(buffer_out, BUFFER_SIZE - buffer.avail_out);
        hashSig.Update(reinterpret_cast<const byte*>(buffer_out), BUFFER_SIZE - buffer.avail_out);
      }
      buffer.next_out = buffer_out;
      buffer.avail_out = BUFFER_SIZE;

      // stop if it is done
      if(status == RS_DONE) {
        break;
      }
    }

    if(rs_job_free(job) != RS_DONE) {
      throw std::runtime_error("could not create signature");
    }

    // finalize base hash
    byte digestBase[CryptoPP::SHA1::DIGESTSIZE];
    hashBase.Final(digestBase);

    // write base hash to file and include it in signature hash
    auto endpos = signature.tellp();
    signature.seekp(CryptoPP::SHA1::DIGESTSIZE, ios::beg);
    signature.write(reinterpret_cast<const char*>(digestBase), CryptoPP::SHA1::DIGESTSIZE);
    hashSig.Update(digestBase, CryptoPP::SHA1::DIGESTSIZE);

    // finalize signature hash
    byte digestSig[CryptoPP::SHA1::DIGESTSIZE];
    hashSig.Final(digestSig);

    // write signature hash to file
    signature.seekp(0, ios::beg);
    signature.write(reinterpret_cast<char*>(&digestSig[0]), CryptoPP::SHA1::DIGESTSIZE);
    signature.seekp(endpos, ios::beg);

    // flush
    signature.flush();

    // reset exceptions flags
    base.exceptions(baseOldExceptions);
    signature.exceptions(signatureOldExceptions);
  }

  void signature(boost::filesystem::path basePath, boost::filesystem::path signaturePath, size_t blockLength, size_t sumLength) {

      // open files
      ifstream base(basePath.string().c_str(), ios::binary | ios::in);
      ofstream signature(signaturePath.string().c_str(), ios::binary | ios::trunc | ios::out);

      // run operation
      sync::signature(base, signature, blockLength, sumLength);

      // close files
      base.close();
      signature.close();
  }

} }

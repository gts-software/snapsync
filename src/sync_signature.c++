#include "sync.h++"
#include <fstream>
#include <librsync.h>
#include <cryptopp/sha.h>

using namespace std;

namespace snapsync { namespace sync {

  void signature(std::istream& base, std::ostream& signature) {

    // set exceptions flags
    auto baseOldExceptions = base.exceptions();
    base.exceptions(istream::failbit | istream::badbit);

    auto signatureOldExceptions = signature.exceptions();
    signature.exceptions(ostream::failbit | ostream::badbit);

    // keep space for hash
    signature.seekp(CryptoPP::SHA1::DIGESTSIZE, ios::cur);

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

    CryptoPP::SHA1 hash;

    // run job
    auto job = rs_sig_begin(RS_DEFAULT_BLOCK_LEN, 0, RS_BLAKE2_SIG_MAGIC);

    while(true) {

      // fill input buffer
      if(!buffer.eof_in && (BUFFER_SIZE - buffer.avail_in) > 0) {
        auto count = base.rdbuf()->sgetn(buffer_in + buffer.avail_in, BUFFER_SIZE - buffer.avail_in);
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
        hash.Update(reinterpret_cast<const byte*>(buffer_out), BUFFER_SIZE - buffer.avail_out);
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

    // finalize hash
    byte digest[CryptoPP::SHA1::DIGESTSIZE];
    hash.Final(digest);

    // write hash to file
    auto oldpos = signature.tellp();
    signature.seekp(0, ios::beg);
    signature.write(reinterpret_cast<char*>(&digest[0]), CryptoPP::SHA1::DIGESTSIZE);
    signature.seekp(oldpos, ios::beg);

    // flush
    signature.flush();

    // reset exceptions flags
    base.exceptions(baseOldExceptions);
    signature.exceptions(signatureOldExceptions);
  }

  void signature(boost::filesystem::path basePath, boost::filesystem::path signaturePath) {

      // open files
      ifstream base(basePath.string().c_str(), ios::binary | ios::in);
      ofstream signature(signaturePath.string().c_str(), ios::binary | ios::trunc | ios::out);

      // run operation
      sync::signature(base, signature);

      // close files
      base.close();
      signature.close();
  }

} }

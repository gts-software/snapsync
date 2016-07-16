#include "sync.hh"
#include <librsync.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

using namespace std;

namespace snapsync { namespace sync {

  void delta(std::istream& signature, std::istream& target, std::ostream& patch) {

      #define BUFFER_SIZE (size_t)1024
      char buffer_in[BUFFER_SIZE];
      char buffer_out[BUFFER_SIZE];

      // set exceptions flags
      ios::iostate signatureOldExceptions = signature.exceptions();
      signature.exceptions(istream::failbit | istream::badbit);

      ios::iostate targetOldExceptions = target.exceptions();
      target.exceptions(istream::failbit | istream::badbit);

      ios::iostate patchOldExceptions = patch.exceptions();
      patch.exceptions(ostream::failbit | ostream::badbit);

      // read hashes
      byte digestSignature[CryptoPP::SHA1::DIGESTSIZE];
      byte digestBase[CryptoPP::SHA1::DIGESTSIZE];

      signature.read(reinterpret_cast<char*>(digestSignature), CryptoPP::SHA1::DIGESTSIZE);
      signature.read(reinterpret_cast<char*>(digestBase), CryptoPP::SHA1::DIGESTSIZE);

      // calculate hash of signature
      CryptoPP::SHA1 hashSignature2;
      byte digestSignature2[CryptoPP::SHA1::DIGESTSIZE];

      for(size_t count = 0; (count = signature.rdbuf()->sgetn(buffer_in, BUFFER_SIZE)) != 0;) {
        hashSignature2.Update(reinterpret_cast<const byte*>(buffer_in), count);
      }

      hashSignature2.Update(reinterpret_cast<const byte*>(digestBase), CryptoPP::SHA1::DIGESTSIZE);
      hashSignature2.Final(digestSignature2);

      signature.clear();
      signature.seekg(CryptoPP::SHA1::DIGESTSIZE * 2, ios::beg);

      // validate hash of signature
      if(memcmp(digestSignature, digestSignature2, CryptoPP::SHA1::DIGESTSIZE) != 0) {
        throw InvalidFileException();
      }

      // load signature into memory
      rs_buffers_t buffer;
      buffer.next_out = buffer_out;
      buffer.avail_out = BUFFER_SIZE;
      buffer.next_in = buffer_in;
      buffer.avail_in = 0;
      buffer.eof_in = 0;

      rs_signature_t *signatureSet = NULL;
      rs_job_t* job = rs_loadsig_begin(&signatureSet);

      while(true) {

        // fill input buffer
        if(!buffer.eof_in && (BUFFER_SIZE - buffer.avail_in) > 0) {
          size_t count = signature.rdbuf()->sgetn(buffer_in + buffer.avail_in, BUFFER_SIZE - buffer.avail_in);
          buffer.avail_in += count;
          buffer.eof_in = (count == 0);
        }

        // perform iteration
        rs_result status = rs_job_iter(job, &buffer);

        if(status != RS_DONE && status != RS_BLOCKED) {
          throw std::runtime_error("could not read signature");
        }

        // forward input buffer
        if(buffer.avail_in > 0) {
          std::memmove(buffer_in, buffer.next_in, buffer.avail_in);
        }
        buffer.next_in = buffer_in;

        // fix output buffer (we do not care about its content)
        buffer.next_out = buffer_out;
        buffer.avail_out = BUFFER_SIZE;

        // stop if it is done
        if(status == RS_DONE) {
          break;
        }
      }

      if(rs_job_free(job) != RS_DONE) {
        throw std::runtime_error("could not read signature");
      }

      // fix signature set
      rs_build_hash_table(signatureSet);

      // keep space for hashes
      patch.seekp(CryptoPP::SHA1::DIGESTSIZE * 3, ios::cur);

      // construct deltas
      buffer.next_out = buffer_out;
      buffer.avail_out = BUFFER_SIZE;
      buffer.next_in = buffer_in;
      buffer.avail_in = 0;
      buffer.eof_in = 0;

      CryptoPP::SHA1 hashTarget;
      CryptoPP::SHA1 hashPatch;

      // run job
      job = rs_delta_begin(signatureSet);

      while(true) {

        // fill input buffer
        if(!buffer.eof_in && (BUFFER_SIZE - buffer.avail_in) > 0) {
          size_t count = target.rdbuf()->sgetn(buffer_in + buffer.avail_in, BUFFER_SIZE - buffer.avail_in);
          hashTarget.Update(reinterpret_cast<const byte*>(buffer_in + buffer.avail_in), count);
          buffer.avail_in += count;
          buffer.eof_in = (count == 0);
        }

        // perform iteration
        rs_result status = rs_job_iter(job, &buffer);

        if(status != RS_DONE && status != RS_BLOCKED) {
          throw std::runtime_error("could not create patch");
        }

        // forward input buffer
        if(buffer.avail_in > 0) {
          std::memmove(buffer_in, buffer.next_in, buffer.avail_in);
        }
        buffer.next_in = buffer_in;

        // write output buffer
        if(buffer.avail_out < BUFFER_SIZE) {
          patch.write(buffer_out, BUFFER_SIZE - buffer.avail_out);
          hashPatch.Update(reinterpret_cast<const byte*>(buffer_out), BUFFER_SIZE - buffer.avail_out);
        }
        buffer.next_out = buffer_out;
        buffer.avail_out = BUFFER_SIZE;

        // stop if it is done
        if(status == RS_DONE) {
          break;
        }
      }

      if(rs_job_free(job) != RS_DONE) {
        throw std::runtime_error("could not create patch");
      }

      // free signature set
      rs_free_sumset(signatureSet);

      // finalize target hash
      byte digestTarget[CryptoPP::SHA1::DIGESTSIZE];
      hashTarget.Final(digestTarget);

      // write base hash to file and include it in patch hash
      ios::pos_type endpos = patch.tellp();
      patch.seekp(CryptoPP::SHA1::DIGESTSIZE, ios::beg);
      patch.write(reinterpret_cast<const char*>(digestBase), CryptoPP::SHA1::DIGESTSIZE);
      hashPatch.Update(digestBase, CryptoPP::SHA1::DIGESTSIZE);

      // write target hash to file and include it in patch hash
      patch.write(reinterpret_cast<const char*>(digestTarget), CryptoPP::SHA1::DIGESTSIZE);
      hashPatch.Update(digestTarget, CryptoPP::SHA1::DIGESTSIZE);

      // finalize patch hash
      byte digestPatch[CryptoPP::SHA1::DIGESTSIZE];
      hashPatch.Final(digestPatch);

      // write patch hash to file
      patch.seekp(0, ios::beg);
      patch.write(reinterpret_cast<char*>(&digestPatch[0]), CryptoPP::SHA1::DIGESTSIZE);
      patch.seekp(endpos, ios::beg);

      // flush
      patch.flush();

      // reset exceptions flags
      signature.exceptions(signatureOldExceptions);
      target.exceptions(targetOldExceptions);
      patch.exceptions(patchOldExceptions);

  }

  void delta(boost::filesystem::path signaturePath, boost::filesystem::path targetPath, boost::filesystem::path patchPath) {

      // open files
      ifstream signature(signaturePath.string().c_str(), ios::binary | ios::in);
      ifstream target(targetPath.string().c_str(), ios::binary | ios::in);
      ofstream patch(patchPath.string().c_str(), ios::binary | ios::trunc | ios::out);

      // run operation
      sync::delta(signature, target, patch);

      // close files
      signature.close();
      target.close();
      patch.close();
  }

} }

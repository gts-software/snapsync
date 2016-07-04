#include "sync.h++"
#include <librsync.h>
#include <cryptopp/sha.h>
#include <fstream>

using namespace std;

namespace snapsync { namespace sync {

  rs_result read_base_callback(void *opaque, rs_long_t pos, size_t *len, void **buf) {

    try {
      std::istream& base = *(std::istream*)(opaque);

      base.seekg(pos, ios::beg);
      base.read(reinterpret_cast<char*>(*buf), *len);

      return RS_DONE;
    }
    catch(...) {
      *len = 0;
      return RS_IO_ERROR;
    }
  }

  void patch(std::istream& patch, std::istream& base, std::ostream& target) {

    constexpr size_t BUFFER_SIZE = 1024;
    char buffer_in[BUFFER_SIZE];
    char buffer_out[BUFFER_SIZE];

    // set exceptions flags
    auto patchOldExceptions = patch.exceptions();
    patch.exceptions(istream::failbit | istream::badbit);

    auto baseOldExceptions = base.exceptions();
    base.exceptions(istream::failbit | istream::badbit);

    auto targetOldExceptions = target.exceptions();
    target.exceptions(ostream::failbit | ostream::badbit);

    // read hashes
    byte digestPatch[CryptoPP::SHA1::DIGESTSIZE];
    byte digestBase[CryptoPP::SHA1::DIGESTSIZE];
    byte digestTarget[CryptoPP::SHA1::DIGESTSIZE];

    patch.read(reinterpret_cast<char*>(digestPatch), CryptoPP::SHA1::DIGESTSIZE);
    patch.read(reinterpret_cast<char*>(digestBase), CryptoPP::SHA1::DIGESTSIZE);
    patch.read(reinterpret_cast<char*>(digestTarget), CryptoPP::SHA1::DIGESTSIZE);

    // calculate hash of patch
    CryptoPP::SHA1 hashPatch2;
    byte digestPatch2[CryptoPP::SHA1::DIGESTSIZE];

    for(size_t count = 0; (count = patch.rdbuf()->sgetn(buffer_in, BUFFER_SIZE)) != 0;) {
      hashPatch2.Update(reinterpret_cast<const byte*>(buffer_in), count);
    }

    hashPatch2.Update(reinterpret_cast<const byte*>(digestBase), CryptoPP::SHA1::DIGESTSIZE);
    hashPatch2.Update(reinterpret_cast<const byte*>(digestTarget), CryptoPP::SHA1::DIGESTSIZE);
    hashPatch2.Final(digestPatch2);

    patch.clear();
    patch.seekg(CryptoPP::SHA1::DIGESTSIZE * 3, ios::beg);

    // validate hash of patch
    if(memcmp(digestPatch, digestPatch2, CryptoPP::SHA1::DIGESTSIZE) != 0) {
      throw InvalidFileException();
    }

    // calculate hash of base
    CryptoPP::SHA1 hashBase2;
    byte digestBase2[CryptoPP::SHA1::DIGESTSIZE];

    for(size_t count = 0; (count = base.rdbuf()->sgetn(buffer_in, BUFFER_SIZE)) != 0;) {
      hashBase2.Update(reinterpret_cast<const byte*>(buffer_in), count);
    }

    hashBase2.Final(digestBase2);

    base.clear();
    base.seekg(0, ios::beg);

    // validate hash of base
    if(memcmp(digestBase, digestBase2, CryptoPP::SHA1::DIGESTSIZE) != 0) {
      throw InvalidFileException();
    }

    // read patch and write target
    rs_buffers_t buffer;
    buffer.next_out = buffer_out;
    buffer.avail_out = BUFFER_SIZE;
    buffer.next_in = buffer_in;
    buffer.avail_in = 0;
    buffer.eof_in = 0;

    CryptoPP::SHA1 hashTarget2;

    // run job
    auto job = rs_patch_begin(read_base_callback, &base);

    while(true) {

      // fill input buffer
      if(!buffer.eof_in && (BUFFER_SIZE - buffer.avail_in) > 0) {
        auto count = patch.rdbuf()->sgetn(buffer_in + buffer.avail_in, BUFFER_SIZE - buffer.avail_in);
        buffer.avail_in += count;
        buffer.eof_in = (count == 0);
      }

      // perform iteration
      auto status = rs_job_iter(job, &buffer);

      if(status != RS_DONE && status != RS_BLOCKED) {
        throw std::runtime_error("could not create target");
      }

      // forward input buffer
      if(buffer.avail_in > 0) {
        std::memmove(buffer_in, buffer.next_in, buffer.avail_in);
      }
      buffer.next_in = buffer_in;

      // write output buffer
      if(buffer.avail_out < BUFFER_SIZE) {
        target.write(buffer_out, BUFFER_SIZE - buffer.avail_out);
        hashTarget2.Update(reinterpret_cast<const byte*>(buffer_out), BUFFER_SIZE - buffer.avail_out);
      }
      buffer.next_out = buffer_out;
      buffer.avail_out = BUFFER_SIZE;

      // stop if it is done
      if(status == RS_DONE) {
        break;
      }
    }

    if(rs_job_free(job) != RS_DONE) {
      throw std::runtime_error("could not create target");
    }

    // finalize target hash
    byte digestTarget2[CryptoPP::SHA1::DIGESTSIZE];
    hashTarget2.Final(digestTarget2);

    // validate hash of target
    if(memcmp(digestTarget, digestTarget2, CryptoPP::SHA1::DIGESTSIZE) != 0) {
      throw InvalidFileException();
    }

    // flush
    target.flush();

    // reset exceptions flags
    patch.exceptions(patchOldExceptions);
    base.exceptions(baseOldExceptions);
    target.exceptions(targetOldExceptions);
  }

  void patch(boost::filesystem::path patchPath, boost::filesystem::path basePath, boost::filesystem::path targetPath) {

      // open files
      ifstream patch(patchPath.string().c_str(), ios::binary | ios::in);
      ifstream base(basePath.string().c_str(), ios::binary | ios::in);
      ofstream target(targetPath.string().c_str(), ios::binary | ios::trunc | ios::out);

      // run operation
      sync::patch(patch, base, target);

      // close files
      patch.close();
      base.close();
      target.close();
  }

} }

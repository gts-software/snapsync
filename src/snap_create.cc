#include "snap.hh"
#include "snap_internal.hh"
#include "byteorder.hh"
#include <vector>
#include <fstream>
#include <cryptopp/sha.h>
#include <stdint.h>
#include <algorithm>

using namespace std;
using namespace boost::filesystem;

namespace snapsync { namespace snap {

  template<typename T>
  void write_value(const T& value, std::ostream& image, CryptoPP::SHA1& hash) {
    const T target = byteorder::htobe(value);
    image.write(reinterpret_cast<const char*>(&target), sizeof(T));
    hash.Update(reinterpret_cast<const byte*>(&target), sizeof(T));
  }

  template<>
  void write_value<std::string>(const std::string& content, std::ostream& image, CryptoPP::SHA1& hash) {
    const uint64_t size = static_cast<uint64_t>(content.size());
    write_value(size, image, hash);
    if(size > 0) {
      image.write(content.data(), size);
        hash.Update(reinterpret_cast<const byte*>(content.data()), size);
    }
  }

  void write_file(boost::filesystem::path file, std::ofstream& image, CryptoPP::SHA1& hash) {

    // write file size
    const uint64_t filesize = static_cast<uint64_t>(file_size(file));
    write_value(filesize, image, hash);

    // open file
    std::ifstream stream(file.string().c_str(), ios::binary | ios::in);
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);

    // read file
    #define CHUNKSIZE (size_t)1024
    char chunk[CHUNKSIZE];

    for(
      size_t remaining = static_cast<size_t>(filesize),
             chunksize = std::min(CHUNKSIZE, remaining);
      remaining > 0;
      remaining -= chunksize,
      chunksize = std::min(CHUNKSIZE, remaining)
    ) {
      stream.read(chunk, chunksize);
      image.write(chunk, chunksize);
      hash.Update(reinterpret_cast<const byte*>(chunk), chunksize);
    }
  }

  void write_directory(boost::filesystem::path directory, std::ofstream& image, CryptoPP::SHA1& hash) {

    // load children paths into memory
    vector<path> children;
    copy(directory_iterator(directory), directory_iterator(), back_inserter(children));
    sort(children.begin(), children.end());

    // write size
    uint64_t count = static_cast<uint64_t>(children.size());
    write_value(count, image, hash);

    // iterate children
    for(vector<path>::iterator child = children.begin(); child != children.end(); ++child) {

      // get status
      file_status stat = symlink_status(*child);

      // write name
      write_value(child->filename().string(), image, hash);

      // write permissions
      perms pmask = stat.permissions();
      write_value(static_cast<uint16_t>(pmask), image, hash);

      // write type and content
      if(is_directory(stat)) {
        write_value(NODE_DIRECTORY, image, hash);
        write_directory(*child, image, hash);
      }
      else
      if(is_regular_file(stat)) {
        write_value(NODE_FILE, image, hash);
        write_file(*child, image, hash);
      }
      else
      if(is_symlink(stat)) {
        write_value(NODE_SYMLINK, image, hash);
        path to = read_symlink(*child);
        write_value(to.generic_string(), image, hash);
      }
      else
      if(exists(stat)) {
        throw new std::runtime_error("cannot handle node type");
      }
      else {
        write_value(NODE_NONE, image, hash);
      }
    }
  }

  void create(boost::filesystem::path directory, std::ofstream& image) {

    // enable exceptions on image stream
    ios::iostate oldExceptions = image.exceptions();
    image.exceptions(std::ofstream::failbit | std::ofstream::badbit | std::ofstream::eofbit);

    // keep space for hash
    image.seekp(CryptoPP::SHA1::DIGESTSIZE, ios::cur);

    // write directory
    CryptoPP::SHA1 hash;
    write_directory(directory, image, hash);

    // finalize hash
    byte digest[CryptoPP::SHA1::DIGESTSIZE];
    hash.Final(digest);

    // write hash to file
    ios::pos_type oldpos = image.tellp();
    image.seekp(0, ios::beg);
    image.write(reinterpret_cast<char*>(&digest[0]), CryptoPP::SHA1::DIGESTSIZE);
    image.seekp(oldpos, ios::beg);

    // flush
    image.flush();
    image.exceptions(oldExceptions);
  }

  void create(boost::filesystem::path directory, boost::filesystem::path image) {

      // open file
      std::ofstream stream(image.string().c_str(), ios::binary | ios::trunc | ios::out);

      // create image
      create(directory, stream);

      // close
      stream.close();
  }

} }

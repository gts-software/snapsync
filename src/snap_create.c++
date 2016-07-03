#include "snap.h++"
#include "snap_internal.h++"
#include "byteorder.h++"
#include <vector>
#include <fstream>
#include <cryptopp/sha.h>

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
    const std::uint64_t size = static_cast<std::uint64_t>(content.size());
    write_value(size, image, hash);
    if(size > 0) {
      image.write(content.data(), size);
        hash.Update(reinterpret_cast<const byte*>(content.data()), size);
    }
  }

  void write_file(boost::filesystem::path file, std::ofstream& image, CryptoPP::SHA1& hash) {

    // write file size
    const std::uint64_t filesize = static_cast<std::uint64_t>(file_size(file));
    write_value(filesize, image, hash);

    // open file
    ifstream stream(file.string().c_str(), ios::binary | ios::in);
    stream.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);

    // read file
    constexpr size_t CHUNKSIZE = 1024;
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
    std::uint64_t count = static_cast<std::uint64_t>(children.size());
    write_value(count, image, hash);

    // iterate children
    for(auto child : children) {

      // write name
      write_value(child.filename().string(), image, hash);

      // write type and content
      if(is_directory(child)) {
        write_value(NODE_DIRECTORY, image, hash);
        write_directory(child, image, hash);
      }
      else
      if(is_regular_file(child)) {
        write_value(NODE_FILE, image, hash);
        write_file(child, image, hash);
      }
      else
      if(exists(child)) {
        write_value(NODE_UNKNOWN, image, hash);
      }
      else {
        write_value(NODE_NONE, image, hash);
      }
    }
  }

  void create(boost::filesystem::path directory, std::ofstream& image) {

    // enable exceptions on image stream
    image.exceptions(ofstream::failbit | ofstream::badbit | ofstream::eofbit);

    // keep space for hash
    image.seekp(CryptoPP::SHA1::DIGESTSIZE, ios::cur);

    // write directory
    CryptoPP::SHA1 hash;
    write_directory(directory, image, hash);

    // finalize hash
    byte digest[CryptoPP::SHA1::DIGESTSIZE];
    hash.Final(digest);

    // write hash to file
    image.seekp(0, ios::beg);
    image.write(reinterpret_cast<char*>(&digest[0]), CryptoPP::SHA1::DIGESTSIZE);

    // flush
    image.flush();
  }

  void create(boost::filesystem::path directory, boost::filesystem::path image) {

      // open file
      ofstream stream(image.string().c_str(), ios::binary | ios::trunc | ios::out);

      // create image
      create(directory, stream);

      // close
      stream.close();
  }

} }

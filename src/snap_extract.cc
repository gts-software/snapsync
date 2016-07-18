#include "snap.hh"
#include "snap_internal.hh"
#include "byteorder.hh"
#include <vector>
#include <fstream>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

using namespace std;
using namespace boost::filesystem;

namespace snapsync { namespace snap {

  template< class InputIt, class Size, class OutputIt>
  OutputIt copy_n(InputIt first, Size count, OutputIt result)
  {
      if (count > 0) {
          *result++ = *first;
          for (Size i = 1; i < count; ++i) {
              *result++ = *++first;
          }
      }
      return result;
  }

  template<typename T>
  void read_value(std::istream& image, T& value) {
    T source = T();
    image.read(reinterpret_cast<char*>(&source), sizeof(T));
    value = byteorder::betoh(source);
  }

  template<>
  void read_value<std::string>(std::istream& image, std::string& content) {
    uint64_t size = 0;
    read_value(image, size);
    content.resize(static_cast<std::size_t>(size), '\0');
    if(size > 0) {
      image.read(const_cast<char*>(content.data()), size);
    }
  }

  void read_file(std::ifstream& image, boost::filesystem::path file) {

      // read file size
      uint64_t filesize = 0;
      read_value(image, filesize);

      // open file
      ofstream stream(file.string().c_str(), ios::binary | ios::trunc | ios::out);
      stream.exceptions(ofstream::failbit | ofstream::badbit | ofstream::eofbit);

      // copy file content
      if(filesize > 0) {
        snap::copy_n(
          istreambuf_iterator<char>(image),
          static_cast<std::size_t>(filesize),
          ostreambuf_iterator<char>(stream));
        image.seekg(1, ios::cur);
      }

      // flush and close
      stream.flush();
      stream.close();
  }

  void read_directory(std::ifstream& image, boost::filesystem::path directory) {

    // create target directory
    create_directories(directory);

    // read children count
    uint64_t size = 0;
    read_value(image, size);

    // extract children
    for( size_t i = 0; i < size; ++i ) {

      // extract name
      std::string name;
      read_value(image, name);

      // extract permission mask
      uint16_t pmask;
      read_value(image, pmask);
      cout << hex << "0x" << static_cast<perms>(pmask) << endl;

      // extract type
      NODE_TYPE type = NODE_UNKNOWN;
      read_value(image, type);

      // extract directory
      if(type == NODE_DIRECTORY) {
        read_directory(image, directory / name);
      }
      else
      if(type == NODE_FILE) {
        read_file(image, directory / name);
      }
      else
      if(type == NODE_SYMLINK) {
        std::string to;
        read_value(image, to);
        create_symlink(path(to), directory / name);
      }
      else
      if(type == NODE_UNKNOWN || type == NODE_NONE) {
        // skip
      }
      else {
        throw std::runtime_error("invalid node type");
      }

      // apply permissions
      permissions(directory / name, static_cast<perms>(pmask));
    }
  }

  void extract(std::ifstream& image, boost::filesystem::path directory) {

    // enable exceptions on image stream
    ios::iostate oldExceptions = image.exceptions();
    image.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);

    // read hash
    byte digest1[CryptoPP::SHA1::DIGESTSIZE];
    image.read(reinterpret_cast<char*>(&digest1[0]), CryptoPP::SHA1::DIGESTSIZE);

    // calculate hash of file
    CryptoPP::SHA1 hash;
    byte digest2[CryptoPP::SHA1::DIGESTSIZE];

    image.exceptions(ifstream::goodbit);

    CryptoPP::FileSource fs(image, true,
        new CryptoPP::HashFilter(hash,
          new CryptoPP::ArraySink(digest2, CryptoPP::SHA1::DIGESTSIZE)
        )
      );

    image.clear();
    image.seekg(CryptoPP::SHA1::DIGESTSIZE, ios::beg);
    image.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);

    // compare hashes
    if(memcmp(digest1, digest2, CryptoPP::SHA1::DIGESTSIZE) != 0) {
      throw InvalidFileException();
    }

    // read directory
    read_directory(image, directory);

    // done
    image.exceptions(oldExceptions);
  }

  void extract(boost::filesystem::path image, boost::filesystem::path directory) {

      // open file
      ifstream stream(image.string().c_str(), ios::binary | ios::in);

      // extract image
      extract(stream, directory);
  }

} }

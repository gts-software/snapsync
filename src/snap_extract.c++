#include "snap.h++"
#include "snap_internal.h++"
#include "fileno.h++"
#include <vector>

using namespace std;
using namespace boost::filesystem;

namespace snapsync { namespace snap {

  template<typename T>
  void read_value(std::istream& image, T& value) {
    image.read(reinterpret_cast<char*>(&value), sizeof(T));
  }

  template<>
  void read_value<std::string>(std::istream& image, std::string& content) {
    std::uint64_t size = 0;
    read_value(image, size);
    content.resize(static_cast<std::size_t>(size), '\0');
    if(size > 0) {
      image.read(const_cast<char*>(content.data()), size);
    }
  }

  void read_file(std::ifstream& image, boost::filesystem::path file) {

      // read file size
      std::uint64_t filesize = 0;
      read_value(image, filesize);

      // open file
      ofstream stream(file.string().c_str(), ios::binary | ios::trunc | ios::out);
      stream.exceptions(ofstream::failbit | ofstream::badbit | ofstream::eofbit);

      // copy file content
      std::copy_n(
        istreambuf_iterator<char>(image),
        static_cast<std::size_t>(filesize),
        ostreambuf_iterator<char>(stream));
      image.seekg(1, ios::cur);
  }

  void read_directory(std::ifstream& image, boost::filesystem::path directory) {

    // create target directory
    create_directories(directory);

    // read children count
    std::uint64_t size = 0;
    read_value(image, size);

    // extract children
    for( size_t i = 0; i < size; ++i ) {

      // extract name
      std::string name;
      read_value(image, name);

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
    }
  }

  void extract(std::ifstream& image, boost::filesystem::path directory) {

    // enable exceptions on image stream
    image.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);

    // read directory
    read_directory(image, directory);
  }

  void extract(boost::filesystem::path image, boost::filesystem::path directory) {

      // open file
      ifstream stream(image.string().c_str(), ios::binary | ios::in);

      // extract image
      extract(stream, directory);
  }

} }

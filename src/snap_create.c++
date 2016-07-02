#include "snap.h++"
#include "snap_internal.h++"
#include "fileno.h++"
#include <vector>

using namespace std;
using namespace boost::filesystem;

namespace snapsync { namespace snap {

  template<typename T>
  void write_value(const T& value, std::ostream& image) {
    image.write(reinterpret_cast<const char*>(&value), sizeof(T));
  }

  template<>
  void write_value<std::string>(const std::string& content, std::ostream& image) {
    const std::uint64_t size = static_cast<std::uint64_t>(content.size());
    write_value(size, image);
    if(size > 0) {
      image.write(content.data(), size);
    }
  }

  void write_file(boost::filesystem::path file, std::ofstream& image) {

    // write file size
    const std::uint64_t filesize = static_cast<std::uint64_t>(file_size(file));
    write_value(filesize, image);

    // open file
    ifstream stream(file.string().c_str(), ios::binary | ios::in);
    stream.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);

    // copy content
    std::copy_n(
      istreambuf_iterator<char>(stream),
      static_cast<std::size_t>(filesize),
      ostreambuf_iterator<char>(image));
  }

  void write_directory(boost::filesystem::path directory, std::ofstream& image) {

    // load children paths into memory
    vector<path> children;
    copy(directory_iterator(directory), directory_iterator(), back_inserter(children));
    sort(children.begin(), children.end());

    // write size
    std::uint64_t count = static_cast<std::uint64_t>(children.size());
    write_value(count, image);

    // iterate children
    for(auto child : children) {

      // write name
      write_value(child.filename().string(), image);

      // write type and content
      if(is_directory(child)) {
        write_value(NODE_DIRECTORY, image);
        write_directory(child, image);
      }
      else
      if(is_regular_file(child)) {
        write_value(NODE_FILE, image);
        write_file(child, image);
      }
      else
      if(exists(child)) {
        write_value(NODE_UNKNOWN, image);
      }
      else {
        write_value(NODE_NONE, image);
      }
    }
  }

  void create(boost::filesystem::path directory, std::ofstream& image) {

    // enable exceptions on image stream
    image.exceptions(ofstream::failbit | ofstream::badbit | ofstream::eofbit);

    // write directory
    write_directory(directory, image);
  }

  void create(boost::filesystem::path directory, boost::filesystem::path image) {

      // open file
      ofstream stream(image.string().c_str(), ios::binary | ios::trunc | ios::out);

      // create image
      create(directory, stream);
  }

} }

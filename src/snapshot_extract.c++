#include "snapshot.h++"
#include "snapshot_internal.h++"
#include "fileno.h++"
#include <vector>

using namespace std;
using namespace boost::filesystem;

namespace snapshot {

  void extract(std::ifstream& image, boost::filesystem::path directory) {

    // enable exceptions on image stream
    image.exceptions(ifstream::failbit | ifstream::badbit);

    // read directory
    //read_directory(image, directory);
  }

}

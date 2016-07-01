#include "snap.h++"
#include "sync.h++"
#include <iostream>

using namespace std;
using namespace boost::filesystem;
using namespace snapsync;

int main(int argc, char** argv) {

  if(argc < 4) {
    std::cerr << "Usage: " << std::endl
      << "- " << argv[0] << " create <directory> <image>" << std::endl
      << "- " << argv[0] << " extract <image> <directory>" << std::endl;
    return 1;
  }

  try {
    std::string command = argv[1];
    if(command == "create") {
      snap::create(path(argv[2]), path(argv[3]));
      return 0;
    }
    else
    if(command == "extract") {
      snap::extract(path(argv[2]), path(argv[3]));
      return 0;
    }
    else {
      return 2;
    }
  }
  catch(const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 3;
  }
  catch(...) {
    std::cerr << "Exception: unknown" << std::endl;
    return 3;
  }
}

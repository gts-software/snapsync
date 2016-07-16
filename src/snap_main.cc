#include "snap.hh"
#include <iostream>

using namespace std;
using namespace boost::filesystem;
using namespace snapsync;

void usage(char* path) {
  std::cerr << "Usage: " << std::endl
    << "1) " << path << " create <directory:in> <image:out>" << std::endl
    << "2) " << path << " extract <image:in> <directory:out>" << std::endl;
}

int main(int argc, char** argv) {

  if(argc < 4) {
    usage(argv[0]);
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
      usage(argv[0]);
      return 1;
    }
  }
  catch(const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 2;
  }
  catch(...) {
    std::cerr << "Exception: unknown" << std::endl;
    return 2;
  }
}

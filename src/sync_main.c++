#include "sync.h++"
#include <iostream>

using namespace std;
using namespace boost::filesystem;
using namespace snapsync;

int main(int argc, char** argv) {

  auto usage = [&argv]{
    std::cerr << "Usage: " << std::endl
      << "1) " << argv[0] << " signature <base:in> <signature:out>" << std::endl
      << "2) " << argv[0] << " delta <signature:in> <target:in> <patch:out>" << std::endl
      << "3) " << argv[0] << " patch <patch:in> <base:in> <target:out>" << std::endl;
  };

  if(argc < 2) {
    usage();
    return 1;
  }

  try {
    std::string command = argv[1];
    if(command == "signature") {
      if(argc < 4) {
        usage();
        return 1;
      }
      sync::signature(path(argv[2]), path(argv[3]));
      return 0;
    }
    else
    if(command == "delta") {
      if(argc < 5) {
        usage();
        return 1;
      }
      sync::delta(path(argv[2]), path(argv[3]), path(argv[4]));
      return 0;
    }
    else
    if(command == "patch") {
      if(argc < 5) {
        usage();
        return 1;
      }
      sync::patch(path(argv[2]), path(argv[3]), path(argv[4]));
      return 0;
    }
    else {
      usage();
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

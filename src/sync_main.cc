#include "sync.hh"
#include <iostream>

using namespace std;
using namespace boost::filesystem;
using namespace snapsync;

void usage(char* path) {
  std::cerr << "Usage: " << std::endl
    << "1) " << path << " signature <base:in> <signature:out> [block-length] [sum-length]" << std::endl
    << "2) " << path << " delta <signature:in> <target:in> <patch:out>" << std::endl
    << "3) " << path << " patch <patch:in> <base:in> <target:out>" << std::endl;
}

int main(int argc, char** argv) {

  if(argc < 2) {
    usage(argv[0]);
    return 1;
  }

  try {
    std::string command = argv[1];
    if(command == "signature") {
      if(argc < 4) {
        usage(argv[0]);
        return 1;
      }
      size_t blockLen = 0, sumLen = 0;
      if(argc >= 5) {
        blockLen = atoi(argv[4]);
      }
      if(argc >= 6) {
        sumLen = atoi(argv[5]);
      }
      sync::signature(path(argv[2]), path(argv[3]), blockLen, sumLen);
      return 0;
    }
    else
    if(command == "delta") {
      if(argc < 5) {
        usage(argv[0]);
        return 1;
      }
      sync::delta(path(argv[2]), path(argv[3]), path(argv[4]));
      return 0;
    }
    else
    if(command == "patch") {
      if(argc < 5) {
        usage(argv[0]);
        return 1;
      }
      sync::patch(path(argv[2]), path(argv[3]), path(argv[4]));
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

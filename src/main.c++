#include "snapshot.h++"
#include <iostream>

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv) {

  if(argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <directory> <image>" << std::endl;
    return 1;
  }

  try {
    snapshot::create(path(argv[1]), path(argv[2]));
    return 0;
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

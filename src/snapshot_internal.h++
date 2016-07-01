#include <boost/filesystem.hpp>

namespace snapshot {

  enum NODE_TYPE {
    NODE_DIRECTORY,
    NODE_FILE,
    NODE_UNKNOWN,
    NODE_NONE
  };
}

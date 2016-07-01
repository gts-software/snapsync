#include <boost/filesystem.hpp>

namespace snapsync { namespace snap {

  enum NODE_TYPE {
    NODE_DIRECTORY,
    NODE_FILE,
    NODE_UNKNOWN,
    NODE_NONE
  };
} }

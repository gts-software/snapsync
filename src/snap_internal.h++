#include <boost/filesystem.hpp>

namespace snapsync { namespace snap {

  enum NODE_TYPE : std::uint8_t {
    NODE_DIRECTORY,
    NODE_FILE,
    NODE_UNKNOWN,
    NODE_NONE
  };
} }

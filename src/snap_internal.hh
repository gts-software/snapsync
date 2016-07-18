#include <boost/filesystem.hpp>

namespace snapsync { namespace snap {

  #define NODE_DIRECTORY (uint8_t)0x1
  #define NODE_FILE      (uint8_t)0x2
  #define NODE_SYMLINK   (uint8_t)0x3
  #define NODE_UNKNOWN   (uint8_t)0x8
  #define NODE_NONE      (uint8_t)0x9

  typedef uint8_t NODE_TYPE;
} }

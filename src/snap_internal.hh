#include <boost/filesystem.hpp>

namespace snapsync { namespace snap {

  enum NODE_TYPE : std::uint8_t {
    NODE_DIRECTORY,
    NODE_FILE,
    NODE_UNKNOWN,
    NODE_NONE
  };
} }

namespace byteorder {

    template<typename T>
    T htobe(const T& value);

    template<>
    inline snapsync::snap::NODE_TYPE htobe<snapsync::snap::NODE_TYPE>(const snapsync::snap::NODE_TYPE& value) {
      return value;
    }

    template<typename T>
    T betoh(const T& value);

    template<>
    inline snapsync::snap::NODE_TYPE betoh<snapsync::snap::NODE_TYPE>(const snapsync::snap::NODE_TYPE& value) {
      return value;
    }
}

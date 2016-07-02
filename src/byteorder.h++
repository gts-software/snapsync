#include <endian.h>

namespace byteorder {

    template<typename T>
    T htobe(const T& value);

    template<>
    inline std::uint8_t htobe<std::uint8_t>(const std::uint8_t& value) {
      return value;
    }

    template<>
    inline std::uint16_t htobe<std::uint16_t>(const std::uint16_t& value) {
      return htobe16(value);
    }

    template<>
    inline std::uint32_t htobe<std::uint32_t>(const std::uint32_t& value) {
      return htobe32(value);
    }

    template<>
    inline std::uint64_t htobe<std::uint64_t>(const std::uint64_t& value) {
      return htobe64(value);
    }

    template<typename T>
    T betoh(const T& value);

    template<>
    inline std::uint8_t betoh<std::uint8_t>(const std::uint8_t& value) {
      return value;
    }

    template<>
    inline std::uint16_t betoh<std::uint16_t>(const std::uint16_t& value) {
      return be16toh(value);
    }

    template<>
    inline std::uint32_t betoh<std::uint32_t>(const std::uint32_t& value) {
      return be32toh(value);
    }

    template<>
    inline std::uint64_t betoh<std::uint64_t>(const std::uint64_t& value) {
      return be64toh(value);
    }
}

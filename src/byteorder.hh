#include <endian.h>
#include <stdint.h>

namespace byteorder {

    template<typename T>
    T htobe(const T& value);

    template<>
    inline uint8_t htobe<uint8_t>(const uint8_t& value) {
      return value;
    }

    template<>
    inline uint16_t htobe<uint16_t>(const uint16_t& value) {
      return htobe16(value);
    }

    template<>
    inline uint32_t htobe<uint32_t>(const uint32_t& value) {
      return htobe32(value);
    }

    template<>
    inline uint64_t htobe<uint64_t>(const uint64_t& value) {
      return htobe64(value);
    }

    template<typename T>
    T betoh(const T& value);

    template<>
    inline uint8_t betoh<uint8_t>(const uint8_t& value) {
      return value;
    }

    template<>
    inline uint16_t betoh<uint16_t>(const uint16_t& value) {
      return be16toh(value);
    }

    template<>
    inline uint32_t betoh<uint32_t>(const uint32_t& value) {
      return be32toh(value);
    }

    template<>
    inline uint64_t betoh<uint64_t>(const uint64_t& value) {
      return be64toh(value);
    }
}

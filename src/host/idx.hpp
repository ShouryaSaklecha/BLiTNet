#pragma once
#include <cstdint>
// MNIST's IDX file format; host only
namespace bn {

inline std::uint32_t readBE32(const unsigned char* b) {
  return (std::uint32_t(b[0]) << 24) | (std::uint32_t(b[1]) << 16) |
         (std::uint32_t(b[2]) << 8) | std::uint32_t(b[3]);
}

}

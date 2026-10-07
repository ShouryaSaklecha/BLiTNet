#pragma once
#include <cstdint>
#include <vector>
#include "types.hpp"
// MNIST's IDX file format; host only
namespace bn {

using Bytes = std::vector<unsigned char>;

inline std::uint32_t readBE32(const unsigned char* b) {
  return (std::uint32_t(b[0]) << 24) | (std::uint32_t(b[1]) << 16) |
         (std::uint32_t(b[2]) << 8) | std::uint32_t(b[3]);
}

struct IdxImages {
  int count = 0, rows = 0, cols = 0;
  std::vector<Pixel> pixels;
};

inline bool parseImages(const Bytes& f, IdxImages& out) {
  if (f.size() < 16 || readBE32(&f[0]) != 2051) return false;
  out.count = readBE32(&f[4]);
  out.rows = readBE32(&f[8]);
  out.cols = readBE32(&f[12]);
  if (f.size() != 16 + std::size_t(out.count) * out.rows * out.cols) return false;
  out.pixels.assign(f.begin() + 16, f.end());
  return true;
}

inline bool parseLabels(const Bytes& f, Bytes& labels) {
  if (f.size() < 8 || readBE32(&f[0]) != 2049) return false;
  if (f.size() != 8 + std::size_t(readBE32(&f[4]))) return false;
  labels.assign(f.begin() + 8, f.end());
  return true;
}

}

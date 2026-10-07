#pragma once
#include <cstddef>
#include <fstream>
#include <iterator>
#include <string>
#include "idx.hpp"
#include "sizes.hpp"
#include "top.hpp"
// host side: MNIST from disk into the chip's input stream
namespace bn {

inline bool readFile(const std::string& path, Bytes& out) {
  std::ifstream in(path, std::ios::binary);
  if (!in) return false;
  out.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
  return true;
}

inline void writeImage(const IdxImages& im, int i, ImageStream& s) {
  const Pixel* p = &im.pixels[std::size_t(i) * kInputs];
  for (int k = 0; k < kInputs; ++k) s.write(p[k]);
}

}

#pragma once
#include "sizes.hpp"
#include "types.hpp"
namespace bn {

inline Output toInput(Pixel p, bool binary) {
  Output bit  = p >= kPixelThreshold ? Output(1) : Output(0);
  Output grey = Output(p) / Output(255);
  return binary ? bit : grey;
}

inline Output input(Pixel p) { return toInput(p, kBinaryPixels); }

}

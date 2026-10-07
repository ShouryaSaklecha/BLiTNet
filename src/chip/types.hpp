#pragma once
#include <cstdint>
#include <ap_int.h>
namespace bn {

using Pixel = std::uint8_t;
using PixelIndex = ap_uint<10>;

using Weight    = float;
using Output    = float;
using Sum       = float;
using Threshold = float;
using Rate      = float;

}

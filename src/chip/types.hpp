#pragma once
#include <cstdint>
#include <ap_int.h>
// every number type lives here; change precision in this file only
namespace bn {

using Pixel = std::uint8_t;
using PixelIndex = ap_uint<10>;

using Weight    = float;
using Output    = float;
using Sum       = float;
using Threshold = float;
using Rate      = float;

}

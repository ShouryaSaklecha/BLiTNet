#pragma once
// every size in the network, fixed before compiling, the way the hardware is.
// change a value here and everything else follows; nothing else hard-codes a size.
namespace bn {

// the image: one MNIST digit
constexpr int kRows   = 28;
constexpr int kCols   = 28;
constexpr int kInputs = kRows * kCols;  // one input per pixel

}

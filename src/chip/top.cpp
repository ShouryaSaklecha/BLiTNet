#include "top.hpp"

// placeholder until section S: passes one image through to prove the toolchain
void blitnet(bn::ImageStream& in, bn::ImageStream& out) {
#pragma HLS INTERFACE mode=axis port=in
#pragma HLS INTERFACE mode=axis port=out
  for (int i = 0; i < bn::kInputs; ++i) {
#pragma HLS PIPELINE II=1
    out.write(in.read());
  }
}

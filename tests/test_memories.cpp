#include <cstddef>
#include "check.hpp"
#include "memories.hpp"

static_assert(bn::kInhib == 12, "0.015 x 784 = 11.76, rounded");
static_assert((1 << 10) >= bn::kInputs, "10 bits name every pixel");

static bn::Memories m;  // static: too big for the stack, and starts at zero

int main() {
  using bn::kCols;
  CHECK(bn::patchPixel(0, 0) == 0);
  CHECK(bn::patchPixel(0, 99) == 9 * kCols + 9);
  CHECK(bn::patchPixel(15, 0) == 0);
  CHECK(bn::patchPixel(16, 0) == 1);
  CHECK(bn::patchPixel(19 * 16, 0) == kCols);
  CHECK(bn::patchPixel(180 * 16, 0) == 9 * kCols + 9);
  CHECK(bn::patchPixel(bn::kNeurons - 1, 99) == bn::kInputs - 1);

  const std::size_t N = bn::kNeurons;
  CHECK(sizeof(m.exc) / sizeof(m.exc[0][0]) == N * bn::kPatchPixels);
  CHECK(sizeof(m.inh) / sizeof(m.inh[0][0]) == N * bn::kInhib);
  CHECK(sizeof(m.inhPixel) / sizeof(m.inhPixel[0][0]) == N * bn::kInhib);
  CHECK(sizeof(m.threshold) / sizeof(m.threshold[0]) == N);
  CHECK(sizeof(m.target) / sizeof(m.target[0]) == N);

  bool zero = true;
  for (std::size_t n = 0; n < N; ++n) {
    zero &= m.threshold[n] == 0 && m.target[n] == 0;
    for (int k = 0; k < bn::kPatchPixels; ++k) zero &= m.exc[n][k] == 0;
  }
  CHECK(zero);
  return chk::failures();
}

#include "check.hpp"
#include "input.hpp"

int main() {
  CHECK_NEAR(bn::toInput(0, true), 0.0, 1e-9);
  CHECK_NEAR(bn::toInput(127, true), 0.0, 1e-9);
  CHECK_NEAR(bn::toInput(128, true), 1.0, 1e-9);
  CHECK_NEAR(bn::toInput(255, true), 1.0, 1e-9);

  CHECK_NEAR(bn::toInput(0, false), 0.0, 1e-6);
  CHECK_NEAR(bn::toInput(128, false), 128.0 / 255, 1e-6);
  CHECK_NEAR(bn::toInput(255, false), 1.0, 1e-6);

  CHECK(bn::kBinaryPixels && bn::kPixelThreshold == 128);
  return chk::failures();
}

#include "check.hpp"
#include "init.hpp"

static bn::Memories a, b;

int main() {
  bn::Rng r1(7), r2(7);
  bn::initTargets(a, r1);
  bn::initTargets(b, r2);

  bool inRange = true, same = true;
  double sum = 0;
  for (int n = 0; n < bn::kNeurons; ++n) {
    inRange &= a.target[n] >= 0.03f && a.target[n] <= 0.25f;
    same &= a.target[n] == b.target[n];
    sum += a.target[n];
  }
  CHECK(inRange && same);
  CHECK_NEAR(sum / bn::kNeurons, 0.14, 0.005);

  double spread = 0;
  for (int p = 0; p < bn::kPatchPositions; ++p) {
    float lo = 1, hi = 0;
    for (int i = 0; i < bn::kNeuronsPerPos; ++i) {
      float t = a.target[p * bn::kNeuronsPerPos + i];
      lo = t < lo ? t : lo;
      hi = t > hi ? t : hi;
    }
    spread += hi - lo;
  }
  CHECK(spread / bn::kPatchPositions > 0.15);
  return chk::failures();
}

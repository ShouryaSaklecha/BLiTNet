#include "check.hpp"
#include "init.hpp"
#include "load.hpp"

static bn::Memories host, chip;

int main() {
  bn::Rng rng(11);
  bn::initAll(host, rng);
  bn::loadNetwork(chip, &host.exc[0][0], &host.inhPixel[0][0], &host.inh[0][0],
                  host.threshold, host.target);

  bool same = true;
  for (int n = 0; n < bn::kNeurons; ++n) {
    for (int k = 0; k < bn::kPatchPixels; ++k) same &= chip.exc[n][k] == host.exc[n][k];
    for (int j = 0; j < bn::kInhib; ++j)
      same &= chip.inhPixel[n][j] == host.inhPixel[n][j] && chip.inh[n][j] == host.inh[n][j];
    same &= chip.threshold[n] == host.threshold[n] && chip.target[n] == host.target[n];
  }
  CHECK(same);
  CHECK(chip.exc[bn::kNeurons - 1][bn::kPatchPixels - 1] != 0);
  return chk::failures();
}

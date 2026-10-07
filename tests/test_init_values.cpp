#include <set>
#include "check.hpp"
#include "init.hpp"

static bn::Memories a, b, c;

int main() {
  bn::Rng r1(3), r2(3);
  bn::initAll(a, r1);
  bn::initAll(b, r2);

  bool exc = true, inh = true, thr = true, wiring = true, same = true;
  for (int n = 0; n < bn::kNeurons; ++n) {
    for (int k = 0; k < bn::kPatchPixels; ++k) exc &= a.exc[n][k] >= 0 && a.exc[n][k] <= bn::kExcInitMax;
    std::set<int> pixels;
    for (int j = 0; j < bn::kInhib; ++j) {
      inh &= a.inh[n][j] >= 0 && a.inh[n][j] <= bn::kInhInitMax;
      pixels.insert(int(a.inhPixel[n][j]));
      same &= a.inhPixel[n][j] == b.inhPixel[n][j] && a.inh[n][j] == b.inh[n][j];
    }
    wiring &= int(pixels.size()) == bn::kInhib && *pixels.rbegin() < bn::kInputs;
    thr &= a.threshold[n] >= 0 && a.threshold[n] <= bn::kThresholdMax;
    same &= a.threshold[n] == b.threshold[n] && a.exc[n][0] == b.exc[n][0];
  }
  CHECK(exc && inh && thr);
  CHECK(wiring);
  CHECK(same);
  CHECK(a.inhPixel[0][0] != a.inhPixel[1][0] || a.inhPixel[0][1] != a.inhPixel[1][1]);

  bn::InitConfig cfg;
  cfg.inhMax = 0.2f;
  bn::Rng r3(3);
  bn::initAll(c, r3, cfg);
  bool wider = false;
  for (int n = 0; n < bn::kNeurons; ++n)
    for (int j = 0; j < bn::kInhib; ++j) wider |= c.inh[n][j] > bn::kInhInitMax;
  CHECK(wider);
  return chk::failures();
}

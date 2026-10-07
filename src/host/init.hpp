#pragma once
#include <vector>
#include "memories.hpp"
#include "rng.hpp"
namespace bn {

inline void initTargets(Memories& m, Rng& rng) {
  for (int n = 0; n < kNeurons; ++n) m.target[n] = rng.uniform(kTargetLo, kTargetHi);
}

struct InitConfig {
  float thresholdMax = kThresholdMax;
  float excMax       = kExcInitMax;
  float inhMax       = kInhInitMax;
};

inline void initAll(Memories& m, Rng& rng, const InitConfig& c = InitConfig()) {
  initTargets(m, rng);
  for (int n = 0; n < kNeurons; ++n) {
    m.threshold[n] = rng.uniform(0.f, c.thresholdMax);
    for (int k = 0; k < kPatchPixels; ++k) m.exc[n][k] = rng.uniform(0.f, c.excMax);
    std::vector<int> px = rng.pick(kInhib, kInputs);
    for (int j = 0; j < kInhib; ++j) {
      m.inhPixel[n][j] = px[j];
      m.inh[n][j] = rng.uniform(0.f, c.inhMax);
    }
  }
}

}

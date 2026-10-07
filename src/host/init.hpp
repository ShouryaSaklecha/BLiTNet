#pragma once
#include "memories.hpp"
#include "rng.hpp"
// host side: the network's starting values, rolled once before training
namespace bn {

inline void initTargets(Memories& m, Rng& rng) {
  for (int n = 0; n < kNeurons; ++n) m.target[n] = rng.uniform(kTargetLo, kTargetHi);
}

}

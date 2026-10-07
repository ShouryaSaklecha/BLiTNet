#pragma once
#include "memories.hpp"
namespace bn {

inline void loadNetwork(Memories& m, const Weight* exc, const PixelIndex* inhPixel,
                        const Weight* inh, const Threshold* threshold, const Rate* target) {
  for (int n = 0; n < kNeurons; ++n) {
    for (int k = 0; k < kPatchPixels; ++k) m.exc[n][k] = exc[n * kPatchPixels + k];
    for (int j = 0; j < kInhib; ++j) {
      m.inhPixel[n][j] = inhPixel[n * kInhib + j];
      m.inh[n][j] = inh[n * kInhib + j];
    }
    m.threshold[n] = threshold[n];
    m.target[n] = target[n];
  }
}

}

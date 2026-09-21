#pragma once
namespace bn {

inline float clip01(float v) { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); }

inline bool spiked(float v) { return v > 0.f; }

// one full update per step, nothing carried between steps; output size also encodes firing time
inline float neuronUpdate(float excitation, float inhibition, float threshold) {
  return clip01(excitation - inhibition - threshold);
}

}

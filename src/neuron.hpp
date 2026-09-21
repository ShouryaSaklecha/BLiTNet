#pragma once
// The neuron, membrane-free (this is the whole point of BLiTNet).
// One timestep is one complete update. No voltage carried between steps, no leak,
// no time constant. That is what makes the model cheap on an FPGA: a MAC, a
// subtract, and a clip.
namespace bn {

inline float clip01(float v) { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); }

inline bool spiked(float v) { return v > 0.f; }

// Network state update, eq (1) with noise = 0 and constant input = 0 (MNIST case).
//   activity = clip( excitation - inhibition - threshold )
// A value > 0 counts as a spike; its size also encodes sub-timestep timing.
inline float neuronUpdate(float excitation, float inhibition, float threshold) {
  return clip01(excitation - inhibition - threshold);
}

}

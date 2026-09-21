#pragma once
#include <vector>
#include "weights.hpp"
#include "config.hpp"
// One feature layer: excitatory and inhibitory input blocks, a per-neuron
// threshold, and a per-neuron target firing rate. It ties together the neuron
// update (eq 1) and the four stabilising mechanisms (eqs 2,4,6,7,8).
namespace bn {

struct Layer {
  int nPre = 0, nPost = 0;
  Weights exc, inh;
  std::vector<float> theta;    // per-neuron threshold
  std::vector<float> fTarget;  // per-neuron target firing rate
  std::vector<float> act;      // current activity (also the spike pattern)
  std::vector<float> scratch;  // buffer for the winner-take-all step
  float kNorm = 1.f;           // excitatory normalisation total (eq 5)
  int nWinners = 0;            // how many neurons may fire per image (competition)

  void init(int pre, int post, const Config& c, Rng& rng);

  // eq (1): activity = clip(excitation - inhibition - threshold).
  void forward(const float* pre);

  // Apply learning for one presentation: excitatory STDP + normalise,
  // inhibitory STDP + scale, then intrinsic threshold plasticity.
  void learn(const float* pre, float lr, float lrItp);

  const std::vector<float>& output() const { return act; }
};

}

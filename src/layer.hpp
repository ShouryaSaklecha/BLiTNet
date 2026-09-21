#pragma once
#include <vector>
#include "weights.hpp"
#include "config.hpp"
namespace bn {

struct Layer {
  int nPre = 0, nPost = 0;
  Weights exc, inh;
  std::vector<float> theta;    // per-neuron threshold
  std::vector<float> fTarget;  // per-neuron target firing rate
  std::vector<float> act;      // current output, also the spike pattern
  std::vector<float> scratch;  // buffer for the winner-take-all step
  float kNorm = 1.f;           // total each neuron's excitatory weights sum to
  int nWinners = 0;            // how many neurons may fire per image

  void init(int pre, int post, const Config& c, Rng& rng);

  void forward(const float* pre);

  void learn(const float* pre, float lr, float lrItp);

  const std::vector<float>& output() const { return act; }
};

}

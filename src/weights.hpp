#pragma once
#include <vector>
#include <cstdint>
#include "rng.hpp"
namespace bn {

struct Weights {
  int nPost = 0, nPre = 0;
  std::vector<float> w;        // nPost x nPre, one row per output neuron
  std::vector<uint8_t> mask;   // 1 where a connection exists

  void init(int post, int pre, float p, float wMax, Rng& rng);

  const float* row(int j) const { return &w[(size_t)j * nPre]; }
  float* row(int j) { return &w[(size_t)j * nPre]; }

  // weighted sum of the connected inputs into neuron j
  float dot(int j, const float* pre) const;

  // strengthen connections where input was on and the neuron fired
  void stdpExc(const float* pre, const float* postAct, float lr);

  // rescale a neuron's incoming weights to a fixed total, so they compete
  void normalise(float k);

  // learn the inhibitory balance: build up when the neuron fires, relax when it stays silent
  void stdpInh(const float* pre, const float* postAct, const float* fTgt, float lr);

  // nudge inhibition by the neuron's net input drive
  void scale(const float* pre, float lr);
};

}

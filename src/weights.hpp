#pragma once
#include <vector>
#include <cstdint>
#include "rng.hpp"
// A dense block of connections from `nPre` neurons to `nPost` neurons.
// Weights are stored as non-negative magnitudes; the layer decides whether the
// block is excitatory (added) or inhibitory (subtracted). A mask allows sparse
// connectivity: only masked-in synapses carry signal or ever change.
namespace bn {

struct Weights {
  int nPost = 0, nPre = 0;
  std::vector<float> w;        // nPost * nPre, row-major by post neuron
  std::vector<uint8_t> mask;   // 1 = connection exists

  void init(int post, int pre, float p, float wMax, Rng& rng);

  const float* row(int j) const { return &w[(size_t)j * nPre]; }
  float* row(int j) { return &w[(size_t)j * nPre]; }

  // Weighted input to post neuron j: sum over pre of mask*w*pre.
  float dot(int j, const float* pre) const;

  // Excitatory STDP, potentiation term of eq (2): pre active and post fired
  // strengthens the synapse. The competitive weakening is supplied by normalise().
  void stdpExc(const float* pre, const float* postAct, float lr);

  // Rescale each post neuron's incoming weights so their total equals k
  // (eqs 4 and 5): one synapse can only grow by shrinking the others.
  void normalise(float k);

  // Inhibitory STDP, eq (6): strengthen when pre active and post fires; weaken,
  // scaled by the post neuron's target rate, when pre active and post is silent.
  void stdpInh(const float* pre, const float* postAct, const float* fTgt, float lr);

  // Inhibitory scaling, eq (7): nudge a neuron's inhibition by its net pre drive.
  void scale(const float* pre, float lr);
};

}

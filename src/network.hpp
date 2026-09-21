#pragma once
#include <vector>
#include "layer.hpp"
#include "mnist.hpp"
#include "config.hpp"
// The network: input layer (the pixels) feeding one plastic feature layer.
// The readout is a separate linear decoder (see decoder.hpp), which is the
// paper's best-scoring configuration.
namespace bn {

struct Net {
  Config cfg;
  Layer feature;

  void build(Rng& rng);

  // One unsupervised epoch: present each image once, learning-rate annealed to
  // zero across the epoch (eqs 3 and 9). No labels are used here.
  void trainFeatures(const Mnist& tr, bool progress);

  // Run one image through the frozen feature layer and return its activity.
  void encode(const float* img, std::vector<float>& out);

  // Fraction of feature neurons that fired, averaged over a sample of images.
  float meanSparsity(const Mnist& m, int sample);
};

}

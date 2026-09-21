#pragma once
#include <vector>
#include "layer.hpp"
#include "mnist.hpp"
#include "config.hpp"
namespace bn {

struct Net {
  Config cfg;
  Layer feature;

  void build(Rng& rng);

  // one unsupervised pass over the images, no labels
  void trainFeatures(const Mnist& tr, bool progress);

  void encode(const float* img, std::vector<float>& out);

  // fraction of feature neurons that fire, averaged over a sample
  float meanSparsity(const Mnist& m, int sample);
};

}

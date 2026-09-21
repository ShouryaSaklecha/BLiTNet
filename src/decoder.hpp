#pragma once
#include <vector>
#include "network.hpp"
#include "mnist.hpp"
// Linear readout: ridge regression from a feature vector to a one-hot class
// target, solved in closed form. This is the paper's best-scoring readout, and
// it needs labels only for this final step, never for feature learning.
namespace bn {

struct Decoder {
  int dim = 0, nClasses = 10;
  std::vector<double> W;  // (dim+1) * nClasses, last row is the bias

  void fit(Net& net, const Mnist& tr, float lambda);
  int predict(const std::vector<float>& feat) const;
  float evaluate(Net& net, const Mnist& te) const;
};

// Baseline for comparison: the same ridge decoder run directly on raw pixels,
// so we can see how much the feature layer adds.
float ridgeBaselineRawPixels(const Mnist& tr, const Mnist& te,
                             float lambda, int nClasses);

}

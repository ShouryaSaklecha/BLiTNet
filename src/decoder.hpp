#pragma once
#include <vector>
#include "network.hpp"
#include "mnist.hpp"
namespace bn {

// linear readout fitted on the frozen features; labels used only here
struct Decoder {
  int dim = 0, nClasses = 10;
  std::vector<double> W;  // (dim+1) x nClasses, last row is the bias

  void fit(Net& net, const Mnist& tr, float lambda);
  int predict(const std::vector<float>& feat) const;
  float evaluate(Net& net, const Mnist& te) const;
};

// same readout on raw pixels, to show what the feature layer adds
float ridgeBaselineRawPixels(const Mnist& tr, const Mnist& te,
                             float lambda, int nClasses);

}

#pragma once
#include "types.hpp"
// the neuron, paper Eq 1 with noise and constant input at zero (MNIST setting)
namespace bn {

inline Output clip01(Sum v) { return v < 0 ? Output(0) : v > 1 ? Output(1) : Output(v); }

inline Output neuron(Sum excitation, Sum inhibition, Threshold threshold) {
  return clip01(excitation - inhibition - threshold);
}

inline bool fired(Output x) { return x > 0; }

}

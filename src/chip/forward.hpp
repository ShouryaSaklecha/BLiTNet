#pragma once
#include "memories.hpp"
#include "neuron.hpp"
namespace bn {

inline Sum excitation(const Memories& m, int n, const Output x[kInputs]) {
  Sum s = 0;
  for (int k = 0; k < kPatchPixels; ++k) s += x[patchPixel(n, k)] * m.exc[n][k];
  return s;
}

inline Sum inhibition(const Memories& m, int n, const Output x[kInputs]) {
  Sum s = 0;
  for (int j = 0; j < kInhib; ++j) s += x[m.inhPixel[n][j]] * m.inh[n][j];
  return s;
}

inline Output neuronOutput(const Memories& m, int n, const Output x[kInputs]) {
  return neuron(excitation(m, n, x), inhibition(m, n, x), m.threshold[n]);
}

}

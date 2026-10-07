#pragma once
#include "input.hpp"
#include "memories.hpp"
#include "neuron.hpp"
#include "stream.hpp"
namespace bn {

using ImageStream = Stream<Pixel, kInputs>;

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

inline void readImage(ImageStream& in, Output x[kInputs]) {
  for (int i = 0; i < kInputs; ++i) x[i] = input(in.read());
}

inline void layer(const Memories& m, const Output x[kInputs], Output y[kNeurons]) {
  for (int n = 0; n < kNeurons; ++n) y[n] = neuronOutput(m, n, x);
}

inline void forward(const Memories& m, ImageStream& in, Output y[kNeurons]) {
  Output x[kInputs];
  readImage(in, x);
  layer(m, x, y);
}

}

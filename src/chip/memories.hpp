#pragma once
#include "sizes.hpp"
#include "types.hpp"
namespace bn {

struct Memories {
  Weight exc[kNeurons][kPatchPixels];
  PixelIndex inhPixel[kNeurons][kInhib];
  Weight inh[kNeurons][kInhib];
  Threshold threshold[kNeurons];
  Rate target[kNeurons];
};

inline int patchPixel(int neuron, int k) {
  int pos = neuron / kNeuronsPerPos;
  int row = (pos / kPositions) * kStride + k / kPatch;
  int col = (pos % kPositions) * kStride + k % kPatch;
  return row * kCols + col;
}

}

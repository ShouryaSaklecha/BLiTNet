#pragma once
#include <vector>
#include <string>
#include <cstdint>
// Loader for the classic MNIST IDX files (big-endian header, raw uint8 pixels).
namespace bn {

struct Mnist {
  int n = 0, rows = 0, cols = 0;
  std::vector<float> images;    // n * (rows*cols), scaled to [0,1]
  std::vector<uint8_t> labels;  // n

  int dim() const { return rows * cols; }
  const float* image(int i) const { return &images[(size_t)i * dim()]; }
  float meanActivity() const;   // average pixel value across the set
};

// limit <= 0 loads everything; otherwise the first `limit` samples.
Mnist loadMnist(const std::string& imgPath, const std::string& lblPath, int limit);

}

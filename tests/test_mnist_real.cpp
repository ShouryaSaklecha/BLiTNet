#include <cstdio>
#include <string>
#include "check.hpp"
#include "mnist.hpp"

static bool load(const char* name, bn::Bytes& b) {
  if (bn::readFile(std::string("data/") + name, b)) return true;
  std::printf("missing data/%s: run scripts/fetch-data.sh\n", name);
  return false;
}

int main() {
  bn::Bytes f, trainLab, testLab;
  bn::IdxImages train, test;
  if (!load("train-images-idx3-ubyte", f) || !bn::parseImages(f, train)) return 1;
  if (!load("t10k-images-idx3-ubyte", f) || !bn::parseImages(f, test)) return 1;
  if (!load("train-labels-idx1-ubyte", f) || !bn::parseLabels(f, trainLab)) return 1;
  if (!load("t10k-labels-idx1-ubyte", f) || !bn::parseLabels(f, testLab)) return 1;
  CHECK(train.count == 60000 && test.count == 10000);
  CHECK(train.rows == bn::kRows && train.cols == bn::kCols);
  CHECK(int(trainLab.size()) == train.count && int(testLab.size()) == test.count);
  CHECK(trainLab[0] == 5 && testLab[0] == 7);

  bn::ImageStream s;
  bn::writeImage(train, 0, s);
  int n = 0;
  bool same = true;
  while (!s.empty()) same &= s.read() == train.pixels[n++];
  CHECK(n == bn::kInputs && same);
  return chk::failures();
}

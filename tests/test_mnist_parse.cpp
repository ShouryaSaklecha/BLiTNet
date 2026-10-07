#include <vector>
#include "check.hpp"
#include "idx.hpp"

static void put32(std::vector<unsigned char>& f, unsigned v) {
  for (int s = 24; s >= 0; s -= 8) f.push_back((v >> s) & 0xFF);
}

int main() {
  std::vector<unsigned char> img;
  put32(img, 2051); put32(img, 2); put32(img, 2); put32(img, 2);
  for (unsigned char p : {0, 64, 128, 255, 1, 2, 3, 4}) img.push_back(p);
  bn::IdxImages im;
  CHECK(bn::parseImages(img, im));
  CHECK(im.count == 2 && im.rows == 2 && im.cols == 2);
  CHECK(im.pixels.size() == 8 && im.pixels[3] == 255 && im.pixels[4] == 1);

  img.pop_back();
  CHECK(!bn::parseImages(img, im));
  img[3] = 0x01;
  CHECK(!bn::parseImages(img, im));

  std::vector<unsigned char> lab;
  put32(lab, 2049); put32(lab, 3);
  for (unsigned char d : {7, 2, 1}) lab.push_back(d);
  std::vector<unsigned char> labels;
  CHECK(bn::parseLabels(lab, labels));
  CHECK(labels.size() == 3 && labels[0] == 7 && labels[2] == 1);
  return chk::failures();
}

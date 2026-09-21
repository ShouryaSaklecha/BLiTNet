#include "mnist.hpp"
#include <fstream>
#include <stdexcept>

namespace bn {

static uint32_t be32(std::ifstream& f) {
  unsigned char b[4];
  f.read(reinterpret_cast<char*>(b), 4);
  return (uint32_t(b[0]) << 24) | (uint32_t(b[1]) << 16) |
         (uint32_t(b[2]) << 8) | uint32_t(b[3]);
}

float Mnist::meanActivity() const {
  double s = 0;
  for (float v : images) s += v;
  return images.empty() ? 0.f : float(s / images.size());
}

Mnist loadMnist(const std::string& imgPath, const std::string& lblPath, int limit) {
  std::ifstream fi(imgPath, std::ios::binary), fl(lblPath, std::ios::binary);
  if (!fi || !fl) throw std::runtime_error("cannot open MNIST files: " + imgPath);

  if (be32(fi) != 2051) throw std::runtime_error("bad image magic");
  uint32_t ni = be32(fi);
  uint32_t rows = be32(fi), cols = be32(fi);
  if (be32(fl) != 2049) throw std::runtime_error("bad label magic");
  uint32_t nl = be32(fl);

  uint32_t n = ni < nl ? ni : nl;
  if (limit > 0 && (uint32_t)limit < n) n = limit;

  Mnist m;
  m.n = n; m.rows = rows; m.cols = cols;
  size_t d = size_t(rows) * cols;
  m.images.resize(size_t(n) * d);
  m.labels.resize(n);

  std::vector<unsigned char> buf(d);
  for (uint32_t i = 0; i < n; ++i) {
    fi.read(reinterpret_cast<char*>(buf.data()), d);
    for (size_t k = 0; k < d; ++k)
      m.images[size_t(i) * d + k] = buf[k] / 255.0f;
    unsigned char lb;
    fl.read(reinterpret_cast<char*>(&lb), 1);
    m.labels[i] = lb;
  }
  return m;
}

}

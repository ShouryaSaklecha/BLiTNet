#include "decoder.hpp"
#include "linalg.hpp"
#include <functional>

namespace bn {

static std::vector<double> trainRidge(
    int dim, int nClasses, int n,
    const std::function<const float*(int)>& feat,
    const std::function<uint8_t(int)>& label, float lambda) {
  int D = dim + 1;                       // last column is the bias
  std::vector<double> A((size_t)D * D, 0.0), B((size_t)D * nClasses, 0.0);
  std::vector<float> x(D);

  for (int s = 0; s < n; ++s) {
    const float* f = feat(s);
    for (int i = 0; i < dim; ++i) x[i] = f[i];
    x[dim] = 1.0f;
    int y = label(s);
    for (int r = 0; r < D; ++r) {
      double xr = x[r];
      if (xr == 0.0) continue;
      double* Ar = &A[(size_t)r * D];
      for (int c = 0; c < D; ++c) Ar[c] += xr * x[c];
      B[(size_t)r * nClasses + y] += xr;
    }
  }
  for (int r = 0; r < D; ++r) A[(size_t)r * D + r] += lambda;
  solveLinearSystem(A, B, D, nClasses);
  return B;
}

static int argmaxScore(const std::vector<double>& W, int dim, int nClasses,
                       const float* f) {
  int best = 0;
  double bestv = -1e300;
  for (int c = 0; c < nClasses; ++c) {
    double s = W[(size_t)dim * nClasses + c];
    for (int i = 0; i < dim; ++i) s += f[i] * W[(size_t)i * nClasses + c];
    if (s > bestv) { bestv = s; best = c; }
  }
  return best;
}

void Decoder::fit(Net& net, const Mnist& tr, float lambda) {
  dim = net.cfg.nFeature;
  nClasses = net.cfg.nClasses;
  int n = net.cfg.nTrain < tr.n ? net.cfg.nTrain : tr.n;
  std::vector<float> buf;
  auto feat = [&](int i) -> const float* { net.encode(tr.image(i), buf); return buf.data(); };
  auto lab = [&](int i) -> uint8_t { return tr.labels[i]; };
  W = trainRidge(dim, nClasses, n, feat, lab, lambda);
}

int Decoder::predict(const std::vector<float>& feat) const {
  return argmaxScore(W, dim, nClasses, feat.data());
}

float Decoder::evaluate(Net& net, const Mnist& te) const {
  int n = net.cfg.nTest < te.n ? net.cfg.nTest : te.n;
  std::vector<float> buf;
  int correct = 0;
  for (int i = 0; i < n; ++i) {
    net.encode(te.image(i), buf);
    if (argmaxScore(W, dim, nClasses, buf.data()) == te.labels[i]) ++correct;
  }
  return 100.0f * correct / n;
}

float ridgeBaselineRawPixels(const Mnist& tr, const Mnist& te,
                             float lambda, int nClasses) {
  int dim = tr.dim();
  auto feat = [&](int i) -> const float* { return tr.image(i); };
  auto lab = [&](int i) -> uint8_t { return tr.labels[i]; };
  std::vector<double> W = trainRidge(dim, nClasses, tr.n, feat, lab, lambda);
  int correct = 0;
  for (int i = 0; i < te.n; ++i)
    if (argmaxScore(W, dim, nClasses, te.image(i)) == te.labels[i]) ++correct;
  return 100.0f * correct / te.n;
}

}

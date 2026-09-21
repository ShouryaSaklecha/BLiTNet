#include "network.hpp"
#include <cstdio>

namespace bn {

void Net::build(Rng& rng) {
  feature.init(cfg.nInput, cfg.nFeature, cfg, rng);
}

void Net::trainFeatures(const Mnist& tr, bool progress) {
  int T = cfg.nTrain < tr.n ? cfg.nTrain : tr.n;
  for (int t = 0; t < T; ++t) {
    float frac = 1.f - float(t) / float(T);   // 1 at the start, 0 at the end
    float lr = cfg.etaInit * frac * frac;
    float lrItp = cfg.etaItpMul * cfg.etaInit * frac * frac;

    const float* img = tr.image(t);
    feature.forward(img);
    feature.learn(img, lr, lrItp);

    if (progress && (t % 10000 == 0))
      std::printf("  trained %6d / %d images\n", t, T);
  }
}

void Net::encode(const float* img, std::vector<float>& out) {
  feature.forward(img);
  out = feature.output();
}

float Net::meanSparsity(const Mnist& m, int sample) {
  int s = sample < m.n ? sample : m.n;
  std::vector<float> f;
  long fired = 0;
  for (int i = 0; i < s; ++i) {
    encode(m.image(i), f);
    for (float v : f) if (v > 0.f) ++fired;
  }
  return float(double(fired) / (double(s) * cfg.nFeature));
}

}

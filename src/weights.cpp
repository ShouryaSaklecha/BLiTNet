#include "weights.hpp"

namespace bn {

void Weights::init(int post, int pre, float p, float wMax, Rng& rng) {
  nPost = post; nPre = pre;
  w.assign((size_t)post * pre, 0.f);
  mask.assign((size_t)post * pre, 0);
  for (size_t idx = 0; idx < w.size(); ++idx) {
    if (p >= 1.0f || rng.chance(p)) {
      mask[idx] = 1;
      w[idx] = rng.uniform(0.f, wMax);  // random start breaks symmetry
    }
  }
}

float Weights::dot(int j, const float* pre) const {
  const float* wr = row(j);
  const uint8_t* mr = &mask[(size_t)j * nPre];
  float s = 0.f;
  for (int i = 0; i < nPre; ++i)
    if (mr[i]) s += wr[i] * pre[i];
  return s;
}

void Weights::stdpExc(const float* pre, const float* postAct, float lr) {
  for (int j = 0; j < nPost; ++j) {
    if (postAct[j] <= 0.f) continue;          // post must have fired
    float* wr = row(j);
    const uint8_t* mr = &mask[(size_t)j * nPre];
    for (int i = 0; i < nPre; ++i)
      if (mr[i]) wr[i] += lr * pre[i];         // pre active -> strengthen
  }
}

void Weights::normalise(float k) {
  for (int j = 0; j < nPost; ++j) {
    float* wr = row(j);
    const uint8_t* mr = &mask[(size_t)j * nPre];
    float sum = 0.f;
    for (int i = 0; i < nPre; ++i) if (mr[i]) sum += wr[i];
    if (sum <= 1e-9f) continue;
    float g = k / sum;
    for (int i = 0; i < nPre; ++i) if (mr[i]) wr[i] *= g;
  }
}

void Weights::stdpInh(const float* pre, const float* postAct, const float* fTgt,
                      float lr) {
  for (int j = 0; j < nPost; ++j) {
    float* wr = row(j);
    const uint8_t* mr = &mask[(size_t)j * nPre];
    bool fired = postAct[j] > 0.f;
    float sign = fired ? 1.f : -fTgt[j];       // eq 6 weakening scaled by target
    for (int i = 0; i < nPre; ++i) {
      if (!mr[i]) continue;
      wr[i] += lr * pre[i] * sign;
      if (wr[i] < 0.f) wr[i] = 0.f;            // magnitude stays non-negative
    }
  }
}

void Weights::scale(const float* pre, float lr) {
  for (int j = 0; j < nPost; ++j) {
    float net = 0.f;
    const uint8_t* mr = &mask[(size_t)j * nPre];
    for (int i = 0; i < nPre; ++i) if (mr[i]) net += pre[i];
    float f = 1.f - lr * (net > 0.f ? 1.f : 0.f);
    float* wj = row(j);
    for (int i = 0; i < nPre; ++i) if (mr[i]) wj[i] *= f;
  }
}

}

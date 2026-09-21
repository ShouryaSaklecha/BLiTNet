#include "layer.hpp"
#include "neuron.hpp"
#include <algorithm>
#include <cmath>

namespace bn {

void Layer::init(int pre, int post, const Config& c, Rng& rng) {
  nPre = pre; nPost = post;
  exc.init(post, pre, c.pExc, 1.0f, rng);
  inh.init(post, pre, c.pInh, 0.05f, rng);
  theta.assign(post, 0.0f);
  fTarget.resize(post);
  act.assign(post, 0.f);
  scratch.assign(post, 0.f);

  // how many neurons may fire per image, from the mean target rate
  float meanRate = 0.5f * (c.fTargetLo + c.fTargetHi);
  nWinners = std::max(1, int(std::lround(meanRate * post)));

  // spread the target firing rates across the population
  for (int j = 0; j < post; ++j) {
    float u = post > 1 ? float(j) / float(post - 1) : 0.f;
    fTarget[j] = c.fTargetLo + u * (c.fTargetHi - c.fTargetLo);
    theta[j] = rng.uniform(0.05f, 0.15f);
  }

  kNorm = (0.1f / c.pExc) / (c.inputMeanRate > 1e-6f ? c.inputMeanRate : 0.13f);

  exc.normalise(kNorm);
}

void Layer::forward(const float* pre) {
  for (int j = 0; j < nPost; ++j) {
    float e = exc.dot(j, pre);
    float i = inh.dot(j, pre);
    act[j] = neuronUpdate(e, i, theta[j]);
  }
  // keep only the strongest nWinners, silence the rest, so neurons compete and specialise
  if (nWinners < nPost) {
    scratch = act;
    std::nth_element(scratch.begin(), scratch.begin() + (nPost - nWinners),
                     scratch.end());
    float thr = scratch[nPost - nWinners];
    for (int j = 0; j < nPost; ++j)
      if (act[j] < thr) act[j] = 0.f;
  }
}

void Layer::learn(const float* pre, float lr, float lrItp) {
  exc.stdpExc(pre, act.data(), lr);
  exc.normalise(kNorm);

  inh.stdpInh(pre, act.data(), fTarget.data(), lr);
  inh.scale(pre, lr);

  // move each threshold so the neuron's firing rate homes on its target
  for (int j = 0; j < nPost; ++j) {
    float fired = act[j] > 0.f ? 1.f : 0.f;
    theta[j] += lrItp * (fired - fTarget[j]);
    if (theta[j] < 0.f) theta[j] = 0.f;
  }
}

}

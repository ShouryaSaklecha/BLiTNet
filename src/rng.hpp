#pragma once
#include <random>
namespace bn {

struct Rng {
  std::mt19937 gen;
  explicit Rng(unsigned s) : gen(s) {}

  float uniform(float a, float b) {
    std::uniform_real_distribution<float> d(a, b);
    return d(gen);
  }
  bool chance(float p) {
    std::uniform_real_distribution<float> d(0.f, 1.f);
    return d(gen) < p;
  }
};

}

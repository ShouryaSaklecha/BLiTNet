#pragma once
#include <random>
#include <utility>
#include <vector>
namespace bn {

class Rng {
 public:
  explicit Rng(unsigned seed) : gen_(seed) {}

  float uniform(float a, float b) {
    return std::uniform_real_distribution<float>(a, b)(gen_);
  }

  std::vector<int> pick(int k, int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = i;
    for (int i = 0; i < k; ++i)
      std::swap(v[i], v[std::uniform_int_distribution<int>(i, n - 1)(gen_)]);
    v.resize(k);
    return v;
  }

 private:
  std::mt19937 gen_;
};

}

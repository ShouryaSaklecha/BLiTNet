#include <set>
#include "check.hpp"
#include "rng.hpp"

int main() {
  bn::Rng a(42), b(42);
  bool same = true;
  for (int i = 0; i < 5; ++i) same &= a.uniform(0.f, 1.f) == b.uniform(0.f, 1.f);
  CHECK(same);

  bn::Rng r(1);
  double sum = 0;
  bool inRange = true;
  for (int i = 0; i < 10000; ++i) {
    float v = r.uniform(2.f, 4.f);
    inRange &= v >= 2.f && v <= 4.f;
    sum += v;
  }
  CHECK(inRange);
  CHECK_NEAR(sum / 10000, 3.0, 0.05);

  auto p = r.pick(12, 784);
  std::set<int> s(p.begin(), p.end());
  CHECK(p.size() == 12 && s.size() == 12);
  CHECK(*s.begin() >= 0 && *s.rbegin() < 784);
  return chk::failures();
}

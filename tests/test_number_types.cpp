#include "check.hpp"
#include "types.hpp"

int main() {
  bn::Weight w = 0.5f;
  bn::Output x = 0.5f;
  bn::Sum s = 0.5f;
  bn::Bar b = 0.5f;
  bn::Rate r = 0.5f;
  CHECK_NEAR(w + x + s + b + r, 2.5, 1e-6);
  return chk::failures();
}

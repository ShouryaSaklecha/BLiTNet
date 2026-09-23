#include "check.hpp"

int main() {
  CHECK(1 + 1 == 2);  // passes, prints nothing
  CHECK(1 + 1 == 3);  // fails on purpose, prints this line
  // pass only if exactly the deliberate failure was counted
  return chk::failures() == 1 ? 0 : 1;
}

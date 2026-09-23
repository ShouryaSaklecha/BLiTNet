#pragma once
#include <cstdio>
#include <cmath>

namespace chk {
// how many checks have failed so far in this test program
inline int& failures() { static int n = 0; return n; }
}

// on failure: print where and what, count it, and keep going
#define CHECK(cond)                                                    \
  do {                                                                 \
    if (!(cond)) {                                                     \
      ++chk::failures();                                               \
      std::printf("%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, #cond); \
    }                                                                  \
  } while (0)

// decimals: pass if a and b differ by at most tol; written as !(<=) so NaN fails
#define CHECK_NEAR(a, b, tol)                                          \
  do {                                                                 \
    double a_ = (a), b_ = (b);                                         \
    if (!(std::fabs(a_ - b_) <= (tol))) {                              \
      ++chk::failures();                                               \
      std::printf("%s:%d: CHECK_NEAR failed: %s = %.17g, %s = %.17g, tol %g\n", \
                  __FILE__, __LINE__, #a, a_, #b, b_, double(tol));    \
    }                                                                  \
  } while (0)

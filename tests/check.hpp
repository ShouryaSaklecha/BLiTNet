#pragma once
#include <cstdio>

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

#pragma once
#include <vector>
#include <cmath>
// Solve A X = B in place by Gaussian elimination with partial pivoting.
// A is n x n (row-major), B is n x m (row-major). On return B holds X.
namespace bn {

inline void solveLinearSystem(std::vector<double>& A, std::vector<double>& B,
                              int n, int m) {
  auto a = [&](int r, int c) -> double& { return A[(size_t)r * n + c]; };
  auto b = [&](int r, int c) -> double& { return B[(size_t)r * m + c]; };

  for (int col = 0; col < n; ++col) {
    int piv = col;
    double best = std::fabs(a(col, col));
    for (int r = col + 1; r < n; ++r) {
      double v = std::fabs(a(r, col));
      if (v > best) { best = v; piv = r; }
    }
    if (piv != col) {
      for (int c = 0; c < n; ++c) std::swap(a(col, c), a(piv, c));
      for (int c = 0; c < m; ++c) std::swap(b(col, c), b(piv, c));
    }
    double d = a(col, col);
    if (std::fabs(d) < 1e-12) d = 1e-12;
    for (int r = 0; r < n; ++r) {
      if (r == col) continue;
      double f = a(r, col) / d;
      if (f == 0.0) continue;
      for (int c = col; c < n; ++c) a(r, c) -= f * a(col, c);
      for (int c = 0; c < m; ++c) b(r, c) -= f * b(col, c);
    }
  }
  for (int r = 0; r < n; ++r) {
    double d = a(r, r);
    if (std::fabs(d) < 1e-12) d = 1e-12;
    for (int c = 0; c < m; ++c) b(r, c) /= d;
  }
}

}

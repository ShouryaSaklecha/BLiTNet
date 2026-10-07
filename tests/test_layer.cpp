#include "check.hpp"
#include "forward.hpp"
#include "init.hpp"

static bn::Memories m;
static bn::Output x[bn::kInputs], y[bn::kNeurons], z[bn::kNeurons];

static void writeImage(bn::ImageStream& s) {
  for (int i = 0; i < bn::kInputs; ++i) s.write(i % 3 == 0 ? 200 : 50);
}

int main() {
  bn::ImageStream in;
  writeImage(in);
  bn::readImage(in, x);
  bool bits = in.empty();
  for (int i = 0; i < bn::kInputs; ++i) bits &= x[i] == (i % 3 == 0 ? 1 : 0);
  CHECK(bits);

  bn::Rng rng(5);
  bn::initAll(m, rng);
  bn::layer(m, x, y);
  bool match = true;
  for (int n = 0; n < bn::kNeurons; ++n) match &= y[n] == bn::neuronOutput(m, n, x);
  CHECK(match);

  writeImage(in);
  bn::forward(m, in, z);
  bool same = in.empty();
  for (int n = 0; n < bn::kNeurons; ++n) same &= z[n] == y[n];
  CHECK(same);
  return chk::failures();
}

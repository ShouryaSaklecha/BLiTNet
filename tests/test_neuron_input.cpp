#include "check.hpp"
#include "forward.hpp"

static bn::Memories m;
static bn::Output x[bn::kInputs];

int main() {
  x[0] = 1; x[1] = 1; x[28] = 1; x[500] = 1;

  m.exc[0][0] = 0.2f;
  m.exc[0][1] = 0.3f;
  m.exc[0][10] = 0.1f;
  m.exc[0][2] = 0.9f;
  CHECK_NEAR(bn::excitation(m, 0, x), 0.6, 1e-6);

  m.inhPixel[0][0] = 500; m.inh[0][0] = 0.25f;
  m.inhPixel[0][1] = 3;   m.inh[0][1] = 0.5f;
  CHECK_NEAR(bn::inhibition(m, 0, x), 0.25, 1e-6);

  m.threshold[0] = 0.1f;
  CHECK_NEAR(bn::neuronOutput(m, 0, x), 0.25, 1e-6);

  m.exc[16][0] = 0.4f;
  CHECK_NEAR(bn::excitation(m, 16, x), 0.4, 1e-6);
  return chk::failures();
}

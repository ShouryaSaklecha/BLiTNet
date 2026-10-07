#include "check.hpp"
#include "neuron.hpp"

int main() {
  CHECK_NEAR(bn::clip01(-1.f), 0.0, 1e-9);
  CHECK_NEAR(bn::clip01(0.5f), 0.5, 1e-9);
  CHECK_NEAR(bn::clip01(2.f), 1.0, 1e-9);

  CHECK_NEAR(bn::neuron(0.30f, 0.10f, 0.25f), 0.0, 1e-6);
  CHECK_NEAR(bn::neuron(0.40f, 0.10f, 0.25f), 0.05, 1e-6);
  CHECK_NEAR(bn::neuron(2.00f, 0.10f, 0.25f), 1.0, 1e-6);

  CHECK(!bn::fired(0.f));
  CHECK(bn::fired(0.05f));
  return chk::failures();
}

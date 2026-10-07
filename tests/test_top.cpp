#include "check.hpp"
#include "top.hpp"

int main() {
  bn::ImageStream in, out;
  for (int i = 0; i < bn::kInputs; ++i) in.write(bn::Pixel(i % 256));
  blitnet(in, out);
  bool same = true;
  for (int i = 0; i < bn::kInputs; ++i) same &= out.read() == bn::Pixel(i % 256);
  CHECK(same && in.empty() && out.empty());
  return chk::failures();
}

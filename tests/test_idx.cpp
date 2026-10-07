#include "check.hpp"
#include "idx.hpp"

int main() {
  const unsigned char magic[4] = {0x00, 0x00, 0x08, 0x03};
  const unsigned char count[4] = {0x00, 0x00, 0xEA, 0x60};
  const unsigned char big[4]   = {0x12, 0x34, 0x56, 0x78};
  CHECK(bn::readBE32(magic) == 2051);
  CHECK(bn::readBE32(count) == 60000);
  CHECK(bn::readBE32(big) == 0x12345678u);
  return chk::failures();
}

#include "check.hpp"
#include "stream.hpp"

int main() {
  bn::Stream<int, 3> s;
  CHECK(s.empty() && !s.full());

  s.write(1); s.write(2); s.write(3);
  CHECK(s.full() && !s.empty());
  CHECK(s.read() == 1);
  CHECK(s.read() == 2);

  s.write(4);
  CHECK(s.read() == 3);
  CHECK(s.read() == 4);
  CHECK(s.empty() && !s.error());

  s.read();
  CHECK(s.error());

  bn::Stream<int, 1> t;
  t.write(7); t.write(8);
  CHECK(t.error());
  CHECK(t.read() == 7);

  return chk::failures();
}

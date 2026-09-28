#include "check.hpp"
#include "stream.hpp"

int main() {
  bn::Stream<int, 3> s;
  CHECK(s.empty() && !s.full());

  s.push(1); s.push(2); s.push(3);
  CHECK(s.full() && !s.empty());
  CHECK(s.pop() == 1);
  CHECK(s.pop() == 2);

  s.push(4);                       // wraps round the end of the buffer
  CHECK(s.pop() == 3);
  CHECK(s.pop() == 4);
  CHECK(s.empty() && !s.error());

  s.pop();
  CHECK(s.error());                // pop when empty

  bn::Stream<int, 1> t;
  t.push(7); t.push(8);
  CHECK(t.error());                // push when full
  CHECK(t.pop() == 7);             // the extra value was dropped

  return chk::failures();
}

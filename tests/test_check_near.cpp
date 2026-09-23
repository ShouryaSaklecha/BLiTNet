#include "check.hpp"

int main() {
  CHECK(0.1 + 0.2 == 0.3);              
  CHECK_NEAR(0.1 + 0.2, 0.3, 1e-9);   
  CHECK_NEAR(1.0, 1.1, 0.05);          
   //designed to padd at 2 failss
  return chk::failures() == 2 ? 0 : 1;
}

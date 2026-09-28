#include "sizes.hpp"

// checked by the compiler: if any of these is false, the test won't even build
static_assert(bn::kRows == 28 && bn::kCols == 28, "an MNIST digit is 28 x 28 pixels");
static_assert(bn::kInputs == bn::kRows * bn::kCols, "one input per pixel");
static_assert(bn::kInputs == 784, "784 inputs");

int main() { return 0; }

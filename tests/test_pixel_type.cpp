#include <limits>
#include "types.hpp"

static_assert(sizeof(bn::Pixel) == 1, "one byte per pixel");
static_assert(std::numeric_limits<bn::Pixel>::min() == 0, "no negative pixels");
static_assert(std::numeric_limits<bn::Pixel>::max() == 255, "white is 255");

int main() { return 0; }

#include "sizes.hpp"

static_assert(bn::kNeurons == bn::kPatchPositions * bn::kNeuronsPerPos, "neurons in total");

static_assert(bn::kNeuronsPerPos == 16, "16 per position; the paper uses 36");
static_assert(bn::kNeurons == 5776, "361 x 16 = 5,776");

int main() { return 0; }

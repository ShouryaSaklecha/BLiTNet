#include "sizes.hpp"

static_assert(bn::kPatchPixels == bn::kPatch * bn::kPatch, "a patch is square");
static_assert(bn::kPositions == (bn::kRows - bn::kPatch) / bn::kStride + 1, "positions per side");
static_assert(bn::kPatchPositions == bn::kPositions * bn::kPositions, "positions in total");

static_assert(bn::kPatch == 10 && bn::kStride == 1, "10 x 10 patch, stride 1");
static_assert(bn::kPositions == 19 && bn::kPatchPositions == 361, "19 x 19 = 361");

int main() { return 0; }

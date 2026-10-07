#pragma once
#include "sizes.hpp"
#include "stream.hpp"
#include "types.hpp"

namespace bn {
using ImageStream = Stream<Pixel, kInputs>;
}

void blitnet(bn::ImageStream& in, bn::ImageStream& out);

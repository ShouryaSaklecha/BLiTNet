#pragma once
// every size lives here; nothing else hard-codes one
namespace bn {

constexpr int kRows   = 28;
constexpr int kCols   = 28;
constexpr int kInputs = kRows * kCols;

constexpr int kPatch          = 10;
constexpr int kStride         = 1;  // paper silent, inferred from 12,996 = 361 x 36
constexpr int kPatchPixels    = kPatch * kPatch;
constexpr int kPositions      = (kRows - kPatch) / kStride + 1;
constexpr int kPatchPositions = kPositions * kPositions;

}

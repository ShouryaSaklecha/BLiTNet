#pragma once
namespace bn {

constexpr int kRows   = 28;
constexpr int kCols   = 28;
constexpr int kInputs = kRows * kCols;

constexpr int kPatch          = 10;
constexpr int kStride         = 1;
constexpr int kPatchPixels    = kPatch * kPatch;
constexpr int kPositions      = (kRows - kPatch) / kStride + 1;
constexpr int kPatchPositions = kPositions * kPositions;

constexpr int kNeuronsPerPos = 16;
constexpr int kNeurons       = kPatchPositions * kNeuronsPerPos;

constexpr int kInhib = int(0.015 * kInputs + 0.5);

constexpr float kTargetLo = 0.03f;
constexpr float kTargetHi = 0.25f;

constexpr float kThresholdMax = 0.1f;
constexpr float kExcInitMax   = 1.0f;
constexpr float kInhInitMax   = 0.05f;

constexpr bool kBinaryPixels  = true;
constexpr int kPixelThreshold = 128;

}

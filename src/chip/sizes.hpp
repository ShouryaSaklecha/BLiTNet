#pragma once
namespace bn {

constexpr int kRows   = 28;
constexpr int kCols   = 28;
constexpr int kInputs = kRows * kCols;

constexpr int kPatch          = 10;
constexpr int kStride         = 1;  // paper silent, inferred from 12,996 = 361 x 36
constexpr int kPatchPixels    = kPatch * kPatch;
constexpr int kPositions      = (kRows - kPatch) / kStride + 1;
constexpr int kPatchPositions = kPositions * kPositions;

constexpr int kNeuronsPerPos = 16;  // paper: 36
constexpr int kNeurons       = kPatchPositions * kNeuronsPerPos;

constexpr int kInhib = int(0.015 * kInputs + 0.5);  // paper: probability 0.015

constexpr float kTargetLo = 0.03f;  // paper: target rates uniform in [0.03, 0.25]
constexpr float kTargetHi = 0.25f;

constexpr bool kBinaryPixels  = true;  // paper Ext Fig S8a: "1 bit per pixel"
constexpr int kPixelThreshold = 128;   // paper silent

}

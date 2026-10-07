#!/usr/bin/env bash
# Vitis Libraries at the release matching Vitis HLS 2023.2; only the L1 headers we use
set -euo pipefail
cd "$(dirname "$0")/.."
tag=v2023.2_update1
dir=vendor/Vitis_Libraries
if [ ! -d "$dir/.git" ]; then
  git clone -q --depth 1 --branch "$tag" --filter=blob:none --sparse \
    https://github.com/Xilinx/Vitis_Libraries.git "$dir"
fi
git -C "$dir" sparse-checkout set \
  utils/L1/include blas/L1/include solver/L1/include data_mover/L1/include
echo "Vitis Libraries $tag at $(git -C "$dir" rev-parse --short HEAD)"

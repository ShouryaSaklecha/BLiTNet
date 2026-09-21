#!/usr/bin/env bash
# Fetch the four MNIST IDX files into data/. The files are public and unchanged
# since 2017; the mirror below is the CVDF copy, which stays up when
# yann.lecun.com does not.
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p data
base="https://storage.googleapis.com/cvdf-datasets/mnist"
for f in train-images-idx3-ubyte train-labels-idx1-ubyte \
         t10k-images-idx3-ubyte t10k-labels-idx1-ubyte; do
  if [ -f "data/$f" ]; then
    echo "have $f"
  else
    echo "fetching $f"
    curl -fsSL -o "data/$f.gz" "$base/$f.gz"
    gunzip "data/$f.gz"
  fi
done
echo "done. data/ ready."

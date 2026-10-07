#!/usr/bin/env bash
# MNIST into data/, each file checked against its exact size
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p data
base="https://storage.googleapis.com/cvdf-datasets/mnist"
for entry in train-images-idx3-ubyte:47040016 train-labels-idx1-ubyte:60008 \
             t10k-images-idx3-ubyte:7840016   t10k-labels-idx1-ubyte:10008; do
  f=${entry%%:*}; want=${entry##*:}
  [ -f "data/$f" ] || curl -fsSL "$base/$f.gz" | gunzip > "data/$f"
  got=$(stat -c %s "data/$f")
  if [ "$got" -ne "$want" ]; then echo "BAD  $f: $got bytes, want $want"; exit 1; fi
  echo "ok   $f ($got bytes)"
done

# blitnet-cpp

A small, readable C++ reimplementation of the unsupervised spiking feature
learner from:

> Stratton, Wabnitz, Essam, Cheung, Hamilton (2022), *Making a Spiking Net
> Work: Robust brain-like unsupervised machine learning*. arXiv:2208.01204.

The method is theirs. This is a from-scratch demonstrator built to understand it
and, later, to guide an FPGA implementation. It is not their code and makes no
claim on their work.

## What it does

Learns MNIST features with no labels, using the paper's mechanisms in a binned,
one-update-per-image form: a membrane-free neuron, excitatory STDP with weight
normalisation, balancing inhibition, intrinsic threshold plasticity, and
winner-take-all competition as a stand-in for balanced lateral inhibition. A
ridge-regression decoder reads the frozen features. Labels are used only in that
final readout.

This is a scaled demonstrator (500 feature neurons, one epoch), not the full
12,996-neuron network, so the accuracy it prints is its own and is reported
honestly. On this machine it clears the raw-pixel baseline (~86%) with learned
features (~88%).

## Build and run

```sh
scripts/fetch-data.sh   # downloads the four MNIST files into data/
make                    # g++ -O2 -std=c++17 -march=native
make run                # or: ./build/blitnet [nFeature] [nTrain]
```

## Layout

```
src/config.hpp     all tunable constants
src/rng.hpp        seeded RNG
src/mnist.*        IDX loader
src/neuron.hpp     the membrane-free neuron update
src/weights.*      synapse matrix: dot, excitatory/inhibitory STDP, normalise
src/layer.*        one feature layer: forward pass with WTA, then learn
src/network.*      build, unsupervised training loop, encode
src/linalg.hpp     Gaussian elimination
src/decoder.*      ridge-regression readout + raw-pixel baseline
src/main_test.cpp  the testbench
```

Each source file is kept under ~120 lines so the pieces read on their own.

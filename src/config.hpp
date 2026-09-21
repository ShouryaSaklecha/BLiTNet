#pragma once
// All tunable parameters in one place. Paper: Stratton et al. 2022 (BLiTNet / STUNN).
// For MNIST the paper sets noise xi = 0 and constant input c = 0, so balance comes
// entirely from inhibitory STDP + weight normalisation + threshold plasticity + a
// spread of target firing rates.
namespace bn {

struct Config {
  int nInput   = 784;    // 28x28 pixels
  int nFeature = 500;    // feature-layer neurons (paper sweeps up to ~13000)
  int nClasses = 10;
  int nTrain   = 60000;  // images for one unsupervised epoch
  int nTest    = 10000;

  // Learning rates, annealed to zero across the epoch (eqs 3 and 9).
  float etaInit    = 0.02f;  // base STDP rate
  float etaItpMul  = 2.0f;   // ITP rate = 2 * etaInit (eq 9)

  // Per-neuron target firing rates, spread low..high (eq 8 targets).
  // Low rates learn specific digits; high rates learn shared strokes.
  float fTargetLo = 0.03f;
  float fTargetHi = 0.20f;

  // Connection probabilities. Excitatory full, inhibitory sparse (eq 5 uses p).
  float pExc = 1.0f;
  float pInh = 0.10f;

  // Mean input activity, used for the normalisation constant k (eq 5).
  float inputMeanRate = 0.13f;   // measured from MNIST at load time, overwritten

  float ridgeLambda = 5.0f;      // linear-decoder regularisation
  unsigned seed = 1234;
};

}

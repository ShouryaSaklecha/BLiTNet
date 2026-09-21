#pragma once
namespace bn {

struct Config {
  int nInput   = 784;    // 28x28 pixels
  int nFeature = 500;
  int nClasses = 10;
  int nTrain   = 60000;
  int nTest    = 10000;

  float etaInit    = 0.02f;  // base learning rate, falls to zero over the epoch
  float etaItpMul  = 2.0f;   // threshold learning rate = this x etaInit

  float fTargetLo = 0.03f;   // per-neuron target firing rates, spread lo..hi
  float fTargetHi = 0.20f;

  float pExc = 1.0f;         // excitatory fully connected
  float pInh = 0.10f;        // inhibitory sparse

  float inputMeanRate = 0.13f;  // measured from the data at load time

  float ridgeLambda = 5.0f;
  unsigned seed = 1234;
};

}

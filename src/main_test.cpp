// Testbench: reproduce the paper's MNIST pipeline in miniature.
//   1. load MNIST
//   2. baseline: ridge decoder on raw pixels (shows what the features must beat)
//   3. train the feature layer unsupervised for one epoch (no labels)
//   4. fit a linear decoder on the frozen features (labels used only here)
//   5. report test accuracy and a few diagnostics
//
// This is faithful to the paper's five mechanisms (eqs 1,2,4,6,8) in the binned,
// one-shot-per-image interpretation. It is a scaled demonstrator, not the full
// 12996-neuron network, so the number it prints is its own, reported honestly.
#include <cstdio>
#include <chrono>
#include "config.hpp"
#include "rng.hpp"
#include "mnist.hpp"
#include "network.hpp"
#include "decoder.hpp"

using namespace bn;
using clk = std::chrono::steady_clock;

static double secsSince(clk::time_point t0) {
  return std::chrono::duration<double>(clk::now() - t0).count();
}

int main(int argc, char** argv) {
  std::setvbuf(stdout, nullptr, _IOLBF, 0);  // line-buffered even when piped
  Config cfg;
  if (argc > 1) cfg.nFeature = std::atoi(argv[1]);
  if (argc > 2) cfg.nTrain = std::atoi(argv[2]);

  const std::string dir = "data/";
  std::printf("== BLiTNet / STUNN demonstrator (Stratton et al. 2022) ==\n");
  std::printf("loading MNIST...\n");
  Mnist tr = loadMnist(dir + "train-images-idx3-ubyte",
                       dir + "train-labels-idx1-ubyte", cfg.nTrain);
  Mnist te = loadMnist(dir + "t10k-images-idx3-ubyte",
                       dir + "t10k-labels-idx1-ubyte", cfg.nTest);
  cfg.nTrain = tr.n; cfg.nTest = te.n;
  cfg.inputMeanRate = tr.meanActivity();
  std::printf("train=%d  test=%d  input dim=%d  mean pixel=%.3f\n",
              tr.n, te.n, tr.dim(), cfg.inputMeanRate);
  std::printf("feature neurons=%d  target rates=%.2f..%.2f\n\n",
              cfg.nFeature, cfg.fTargetLo, cfg.fTargetHi);

  // 2. raw-pixel baseline
  auto t0 = clk::now();
  float raw = ridgeBaselineRawPixels(tr, te, cfg.ridgeLambda, cfg.nClasses);
  std::printf("[baseline] ridge on raw pixels        : %.2f%%   (%.1fs)\n",
              raw, secsSince(t0));

  // 3. unsupervised feature learning
  Rng rng(cfg.seed);
  Net net; net.cfg = cfg; net.build(rng);
  std::printf("\ntraining feature layer, unsupervised, 1 epoch...\n");
  t0 = clk::now();
  net.trainFeatures(tr, true);
  double trainSecs = secsSince(t0);
  float sparsity = net.meanSparsity(te, 1000);
  std::printf("done in %.1fs   mean feature firing rate=%.1f%%\n\n",
              trainSecs, 100.f * sparsity);

  // 4. linear decode on frozen features
  t0 = clk::now();
  Decoder dec; dec.fit(net, tr, cfg.ridgeLambda);
  float acc = dec.evaluate(net, te);
  std::printf("[STUNN]    ridge on learned features   : %.2f%%   (%.1fs)\n",
              acc, secsSince(t0));

  std::printf("\nsummary: raw %.2f%%  ->  features %.2f%%   (delta %+.2f)\n",
              raw, acc, acc - raw);
  std::printf("paper reference: 97.7%% with 12996 features + linear decoder\n");
  return 0;
}

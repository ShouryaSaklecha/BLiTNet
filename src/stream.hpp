#pragma once
// a fixed-depth FIFO, the C++ stand-in for a hardware stream
namespace bn {

template <typename T, int N>
class Stream {
 public:
  void push(T v) {
    if (full()) { err_ = true; return; }
    buf_[in_] = v;
    in_ = (in_ + 1) % N;
    ++count_;
  }

  T pop() {
    if (empty()) { err_ = true; return T{}; }
    T v = buf_[out_];
    out_ = (out_ + 1) % N;
    --count_;
    return v;
  }

  bool empty() const { return count_ == 0; }
  bool full() const { return count_ == N; }
  bool error() const { return err_; }

 private:
  T buf_[N]{};
  int in_ = 0, out_ = 0, count_ = 0;
  bool err_ = false;
};

}

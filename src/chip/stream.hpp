#pragma once
// make test uses a FIFO that flags overflow; HLS uses hls::stream
#ifndef BN_SOFT_STREAM
#include <hls_stream.h>
#endif

namespace bn {

#ifndef BN_SOFT_STREAM
template <typename T, int N>
using Stream = hls::stream<T, N>;
#else
template <typename T, int N>
class Stream {
 public:
  void write(const T& v) {
    if (full()) { err_ = true; return; }
    buf_[in_] = v;
    in_ = (in_ + 1) % N;
    ++count_;
  }

  T read() {
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
#endif

}

#include <string>
#include <iostream>
#include <chrono>

#ifndef BENCH_TOOL_HH
# define BENCH_TOOL_HH

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::chrono::high_resolution_clock;

struct time_guard {
  high_resolution_clock::time_point      t0;
  const std::string&    msg;
  time_guard(const std::string& m) : t0(high_resolution_clock::now()), msg(m) {}
  ~time_guard() {
    high_resolution_clock::time_point    t1 = high_resolution_clock::now();
    std::cout << msg
              << double(duration_cast<nanoseconds>(t1 - t0).count())/1000000000
              << "s"
              << std::endl;
  }
};

#endif /* !BENCH_TOOL_HH */

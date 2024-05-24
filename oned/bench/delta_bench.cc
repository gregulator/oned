// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
//
// Compile and run from root of git repo with:
//
//   clang++ -I. -O3 --std=c++20 oned/bench/delta_bench.cc && time ./a.out
//
#include "oned/delta.hpp"

#include <iostream>
#include <vector>
#include <list>

#include <time.h>

// CPU time used: 0.000135 (Macbook Pro)
int main() {
  std::vector<int32_t> orig;
  orig.resize(1000000);
  std::vector<int32_t> encoded;
  encoded.resize(orig.size());
  clock_t start, end;
  start = clock();
  oned::DeltaEncode(orig, encoded);
  end = clock();
  double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  std::cout << "CPU time used: " << cpu_time_used << std::endl;
}

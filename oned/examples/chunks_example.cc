// Copyright (c) 2024 The OneD Authors
// See LICENSE file.
//
// Compile and run from root of git repo with:
//
//   clang++ -I. --std=c++20 oned/examples/chunks_example.cc && ./a.out
//
#include "oned/chunks.hpp"

#include <iostream>
#include <vector>

void ChunksExample() {
  std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (oned::Stripe<int> chunk : oned::Chunks(v, 3)) {
    std::cout << "Chunk: ";
    for (int x : chunk) {
      std::cout << x << " ";
    }
    std::cout << std::endl;
  }
  // OUTPUT:
  // Chunk: 0 1 2
  // Chunk: 3 4 5
  // Chunk: 6 7 8
  // Chunk: 9
}

int main() { ChunksExample(); }

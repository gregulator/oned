// Copyright (c) 2024 The OneD Authors
// See LICENSE file.
//
// Compile and run from root of git repo with:
//
//   clang++ -I. --std=c++11 oned/examples/runlength_example.cc && ./a.out
//
#include "oned/runlength.hpp"

#include <iostream>
#include <vector>

void RunlengthEncodeExample() {
  std::vector<uint8_t> orig({'A', 'A', 'A', 'A', 'B', 'B', 'B'});
  std::vector<uint8_t> encoded;
  encoded.resize(oned::ComputeRunlengthEncodeSize(orig.data(), orig.size()));
  oned::RunlengthStatus status = oned::RunlengthEncode(
      orig.data(), orig.size(), encoded.data(), encoded.size());
  if (status != oned::RunlengthStatus::kOk) {
    std::cout << "error code " << (int)status << std::endl;
    return;
  }
  for (int i = 0; i < encoded.size(); i += 2) {
    std::cout << (int)encoded[i] << " '" << encoded[i + 1] << "' ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 4 'A' 3 'B'
}

void RunlengthRoundtripExample() {
  std::vector<uint8_t> orig = {1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3,
                               4, 4, 4, 4, 4, 4, 4, 5, 6, 6, 6};
  size_t encoded_size =
      oned::ComputeRunlengthEncodeSize(orig.data(), orig.size());
  std::cout << "Encoded size: " << encoded_size << std::endl;
  // OUTPUT:
  // Encoded size: 12
  std::vector<uint8_t> encoded;
  encoded.resize(encoded_size);
  oned::RunlengthStatus status = oned::RunlengthEncode(
      orig.data(), orig.size(), encoded.data(), encoded.size());
  if (status != oned::RunlengthStatus::kOk) {
    std::cout << "error code " << (int)status << std::endl;
    return;
  }
  for (uint8_t x : encoded) {
    std::cout << (int)x << " ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 4 1 3 2 5 3 7 4 1 5 3 6

  size_t decoded_size =
      oned::ComputeRunlengthDecodeSize(encoded.data(), encoded.size());
  std::cout << "Decoded size: " << decoded_size << std::endl;
  // OUTPUT:
  // Decoded size: 23
  std::vector<uint8_t> decoded;
  decoded.resize(decoded_size);
  status = oned::RunlengthDecode(encoded.data(), encoded.size(), decoded.data(),
                                 decoded.size());
  if (status != oned::RunlengthStatus::kOk) {
    std::cout << "error code " << (int)status << std::endl;
    return;
  }
  for (uint8_t x : decoded) {
    std::cout << (int)x << " ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 1 1 1 1 2 2 2 3 3 3 3 3 4 4 4 4 4 4 4 5 6 6 6
}

int main() {
  RunlengthEncodeExample();
  RunlengthRoundtripExample();
  return 0;
}

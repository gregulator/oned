// Copyright (c) 2024 The OneD Authors
// See LICENSE file.
//
// Compile and run from root of git repo with:
//
//   clang++ -I. --std=c++20 oned/examples/simple8b_example.cc && ./a.out
//
#include "oned/simple8b.hpp"

#include <iostream>
#include <vector>

void Simple8bExample() {
  std::vector<uint64_t> v = {
      0,  1,      2,     3,  4,  5,  6,  7,  8,  9,
      10, 11,     12,    13, 14, 15, 16, 17, 18, 0x0fffaaffaaffaaff,
      20, 0xaaaa, 0xbbbb};
  std::optional<size_t> encoded_size =
      oned::ComputeSimple8bEncodeSize(v.data(), v.size());
  std::vector<uint64_t> decoded(v.size());
  if (!encoded_size) {
    std::cout << " Error determining simple8b encoding size" << std::endl;
    return;
  }
  std::vector<uint64_t> out(*encoded_size);

  oned::Simple8bStatus result =
      oned::Simple8bEncode(v.data(), v.size(), out.data(), out.size());
  if (result != oned::Simple8bStatus::kOk) {
    std::cout << " Error encoding with simple8b" << std::endl;
    return;
  }

  std::cout << " ENCODED:" << std::endl;
  for (uint64_t v : out) {
    std::cout << std::hex << v << std::endl;
  }

  result = oned::Simple8bDecode(out.data(), out.size(), decoded.data(),
                                decoded.size());
  if (result != oned::Simple8bStatus::kOk) {
    std::cout << " Error decoding with simple8b" << std::endl;
    return;
  }

  std::cout << " DECODED:" << std::endl;
  for (uint64_t v : decoded) {
    std::cout << std::hex << v << std::endl;
  }
}

void Simple8bExampleSigned() {
  std::vector<int64_t> v = {0,  1,       -1,       2, -2, 3,  -3, 4,
                            -5, 1000000, -1000000, 0, -1, -2, -3, -4};
  std::optional<size_t> encoded_size =
      oned::ComputeSimple8bEncodeSize(v.data(), v.size());
  std::vector<int64_t> decoded(v.size());
  if (!encoded_size) {
    std::cout << " Error determining simple8b encoding size" << std::endl;
    return;
  }
  std::vector<uint64_t> out(*encoded_size);

  oned::Simple8bStatus result =
      oned::Simple8bEncode(v.data(), v.size(), out.data(), out.size());
  if (result != oned::Simple8bStatus::kOk) {
    std::cout << " Error encoding with simple8b" << std::endl;
    return;
  }

  std::cout << " ENCODED:" << std::endl;
  for (uint64_t v : out) {
    std::cout << std::hex << v << std::endl;
  }

  result = oned::Simple8bDecode(out.data(), out.size(), decoded.data(),
                                decoded.size());
  if (result != oned::Simple8bStatus::kOk) {
    std::cout << " Error decoding with simple8b" << std::endl;
    return;
  }

  std::cout << " DECODED:" << std::endl;
  for (int64_t v : decoded) {
    std::cout << std::dec << v << std::endl;
  }
}

int main() {
  Simple8bExample();
  Simple8bExampleSigned();
}

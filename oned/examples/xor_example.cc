// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
//
// Compile and run from root of git repo with:
//
//   clang++ -I. --std=c++20 oned/examples/xor_example.cc && ./a.out
//
#include "oned/xor.hpp"

#include <iomanip>
#include <iostream>
#include <vector>

void XorExampleRoundtrip() {
  std::vector<int> orig = {0xFF, 0xF7, 0xF3, 0xF1, 0xF0};
  std::vector<int> encoded;
  encoded.resize(orig.size());
  oned::XorEncode(orig.data(), encoded.data(), orig.size());
  for (int value : encoded) {
    std::cout << std::hex << value << " ";
  }
  std::cout << std::dec << std::endl;
  // OUTPUT:
  // FF 8 4 2 1
  std::vector<int> decoded;
  decoded.resize(encoded.size());
  oned::XorDecode(encoded.data(), decoded.data(), encoded.size());
  for (int value : decoded) {
    std::cout << std::hex << value << " ";
  }
  std::cout << std::dec << std::endl;
  // OUTPUT:
  // FF F7 0xF3 0xF1 0xF0
}

void XorExampleDouble() {
  std::vector<float> orig = {1.1f, 1.2f, 1.3f, 1.4f, 1.5f};
  std::vector<float> encoded;
  encoded.resize(orig.size());
  oned::XorEncode((uint32_t *)orig.data(), (uint32_t *)encoded.data(),
                  orig.size());
  std::cout << "Double hex values before encoding: ";
  for (float value : orig) {
    std::cout << std::hex << std::setw(8) << std::setfill('0')
              << *(uint32_t *)(&value) << " ";
  }
  std::cout << std::endl << "Double hex values after encoding:  ";
  for (float value : encoded) {
    std::cout << std::hex << std::setw(8) << std::setfill('0')
              << *(uint32_t *)(&value) << " ";
  }
  std::cout << std::dec << std::endl;
  // OUTPUT:
}

int main() {
  XorExampleRoundtrip();
  XorExampleDouble();
}

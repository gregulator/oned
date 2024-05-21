// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
//
// Compile and run from root of git repo with:
//
//   clang++ -I. --std=c++20 oned/examples/delta_example.cc && ./a.out
//
#include "oned/delta.hpp"

#include <iostream>
#include <vector>

void DeltaExampleArrayToArray() {
  std::vector<int> orig = {2, 5, 4, 10, 9};
  std::vector<int> encoded;
  encoded.resize(orig.size());
  oned::DeltaEncode(orig.data(), encoded.data(), orig.size());
  for (int value : encoded) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 2 3 -1 6 -1
}

struct RGB {
  uint8_t r, g, b; // red, green, blue
};

void DeltaExampleStripeToArray() {
  std::array<RGB, 5> c = {
      RGB{0x32, 0x91, 0xA8}, // Teal
      RGB{0x32, 0x91, 0xA8}, RGB{0x32, 0x91, 0xA8},
      RGB{0x32, 0x91, 0xA8}, RGB{0x32, 0x91, 0xA8},
  };
  oned::Stripe<uint8_t> red_channel(&c[0].r, c.size(), sizeof(RGB));
  oned::Stripe<uint8_t> green_channel(&c[0].g, c.size(), sizeof(RGB));
  oned::Stripe<uint8_t> blue_channel(&c[0].b, c.size(), sizeof(RGB));
  std::vector<uint8_t> encoded;
  encoded.resize(red_channel.size());
  oned::DeltaEncode(red_channel, encoded.data());
  std::cout << std::hex << "Red channel: ";
  for (int value : encoded) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
  oned::DeltaEncode(green_channel, encoded.data());
  std::cout << "Green channel: ";
  for (int value : encoded) {
    std::cout << (int)value << " ";
  }
  std::cout << std::endl;
  oned::DeltaEncode(blue_channel, encoded.data());
  std::cout << "Blue channel: ";
  for (int value : encoded) {
    std::cout << value << " ";
  }
  std::cout << std::dec << std::endl;
  // OUTPUT:
  // Red channel: 32 0 0 0 0
  // Green channel: 91 0 0 0 0
  // Blue channel: a8 0 0 0 0
}

void DeltaExampleArrayToStripe() {
  std::vector<uint16_t> orig = {10, 25, 35, 40, 50, 70};
  std::vector<uint16_t> dest;
  dest.resize(orig.size() * 2);
  oned::Stripe dest_stripe(std::span(dest), 2); // Skip odd indices.
  oned::DeltaEncode(orig.data(), dest_stripe);
  for (int value : dest) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 10 0 15 0 10 0 5 0 10 0 20 0
}

void DeltaExampleStripeToStripe() {
  std::vector<int16_t> orig = {100, 100, 92, 92, 92, 87, 87, 87, 87, 75, 75};
  oned::Stripe orig_stripe(&orig[orig.size() - 1], orig.size(),
                           -sizeof(int16_t)); // Read array in reverse.
  std::vector<int16_t> dest;
  dest.resize(orig.size());
  oned::Stripe dest_stripe(
      &dest[dest.size() - 1], dest.size(),
      -sizeof(uint16_t)); // Write to destination array in reverse.
  oned::DeltaEncode(orig_stripe, dest_stripe);
  for (int value : dest) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 0 8 0 0 5 0 0 0 12 0 75
}

void DeltaExampleRoundTrip() {
  std::vector<int> orig = {2, 5, 4, 10, 9};
  std::vector<int> encoded;
  encoded.resize(orig.size());
  oned::DeltaEncode(orig.data(), encoded.data(), orig.size());
  std::vector<int> decoded;
  decoded.resize(encoded.size());
  oned::DeltaDecode(encoded.data(), decoded.data(), encoded.size());
  for (int value : decoded) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 2 5 4 10 9
}

void DeltaExampleInplace() {
  std::vector<int> orig = {2, 5, 4, 10, 9};
  oned::DeltaEncode(orig.data(), orig.data(), orig.size());
  for (int value : orig) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 2 3 -1 6 -1
  oned::DeltaDecode(orig.data(), orig.data(), orig.size());
  for (int value : orig) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 2 5 4 10 9
}

int main() {
  DeltaExampleArrayToArray();
  DeltaExampleStripeToArray();
  DeltaExampleArrayToStripe();
  DeltaExampleStripeToStripe();
  DeltaExampleRoundTrip();
  DeltaExampleInplace();
}

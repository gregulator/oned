// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
//
// Compile and run from root of git repo with:
//
//   clang++ -I. --std=c++20 oned/examples/led_example.cc && ./a.out
//
#include <cmath>
#include <iostream>
#include <stdint.h>

#include "oned/delta.hpp"
#include "oned/simple8b.hpp"
#include "oned/stripe.hpp"

struct Color {
  uint8_t r, g, b;
};

std::vector<Color> GenPulseData(Color a, Color b, int period_samples,
                                int num_samples) {
  double freq = 1.0 / (double)(period_samples);
  std::vector<Color> out(num_samples);
  for (int i = 0; i < num_samples; i++) {
    double blend = 0.5 + 0.5 * sin(2 * M_PI * freq * i);
    out[i] = Color{
        (uint8_t)(a.r + blend * (b.r - a.r)),
        (uint8_t)(a.g + blend * (b.g - a.g)),
        (uint8_t)(a.b + blend * (b.b - a.b)),
    };
  }
  return out;
}

void LedExample() {
  std::vector<Color> orig =
      GenPulseData(Color{0x00, 0x80, 0xA0}, Color{0xFF, 0xC0, 0xB0}, 60,
                   /* a million */ 1000);
  std::cout << "Original size: " << sizeof(Color) * orig.size() << std::endl;

  int x = 0;
  std::cout << "Original: " << std::endl;
  for (const Color &c : orig) {
    x++;
    std::cout << std::hex << (int)c.r << " " << (int)c.g << " " << (int)c.b
              << ",  ";
    if (x % 10 == 0) {
      std::cout << std::endl;
    }
  }
  std::cout << std::dec << std::endl;

  // Use a oned::Stripe to extract each channel.
  oned::Stripe<uint8_t> red(&orig[0].r, orig.size(), sizeof(Color));
  oned::Stripe<uint8_t> green(&orig[0].g, orig.size(), sizeof(Color));
  oned::Stripe<uint8_t> blue(&orig[0].b, orig.size(), sizeof(Color));

  // Delta-encode each channel in-place.
  DeltaEncode(red, red);
  DeltaEncode(green, green);
  DeltaEncode(blue, blue);

  std::cout << "Delta-encoded: " << std::endl;
  x = 0;
  for (int i = 0; i < red.size(); i++) {
    x++;
    std::cout << (int)(int8_t)red[i] << " " << (int)(int8_t)green[i] << " "
              << (int)(int8_t)blue[i] << ",  ";
    if (x % 10 == 0) {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;

  // Simple8b-encode each channel.
  std::vector<uint64_t> r_copy(red.begin(), red.end());
  std::vector<uint64_t> g_copy(green.begin(), green.end());
  std::vector<uint64_t> b_copy(blue.begin(), blue.end());
  std::vector<uint64_t> r_encoded;
  std::vector<uint64_t> g_encoded;
  std::vector<uint64_t> b_encoded;

  std::optional<size_t> encoded_size =
      oned::ComputeSimple8bEncodeSize(r_copy.data(), r_copy.size());
  if (!encoded_size) {
    std::cout << "problem determining encode size for red channel" << std::endl;
  }
  r_encoded.resize(*encoded_size);
  encoded_size = oned::ComputeSimple8bEncodeSize(g_copy.data(), g_copy.size());
  if (!encoded_size) {
    std::cout << "problem determining encode size for green channel"
              << std::endl;
  }
  g_encoded.resize(*encoded_size);
  encoded_size = oned::ComputeSimple8bEncodeSize(b_copy.data(), b_copy.size());
  if (!encoded_size) {
    std::cout << "problem determining encode size for blue channel"
              << std::endl;
  }
  b_encoded.resize(*encoded_size);

  oned::Simple8bStatus result = oned::Simple8bEncode(
      r_copy.data(), r_copy.size(), r_encoded.data(), r_encoded.size());
  if (result != oned::Simple8bStatus::kOk) {
    std::cout << "problem encoding red channel" << std::endl;
  }
  result = oned::Simple8bEncode(g_copy.data(), g_copy.size(), g_encoded.data(),
                                g_encoded.size());
  if (result != oned::Simple8bStatus::kOk) {
    std::cout << "problem encoding green channel" << std::endl;
  }
  result = oned::Simple8bEncode(b_copy.data(), b_copy.size(), b_encoded.data(),
                                b_encoded.size());
  if (result != oned::Simple8bStatus::kOk) {
    std::cout << "problem encoding blue channel" << std::endl;
  }

  std::cout << "Simple8b-encoded (red): " << std::endl;
  x = 0;
  for (int i = 0; i < r_encoded.size(); i++) {
    x++;
    std::cout << std::hex << r_encoded[i] << " ";
    if (x % 10 == 0) {
      std::cout << std::endl;
    }
  }
  std::cout << std::dec << std::endl << std::endl;
  std::cout << "Simple8b-encoded (green): " << std::endl;
  x = 0;
  for (int i = 0; i < g_encoded.size(); i++) {
    x++;
    std::cout << std::hex << g_encoded[i] << " ";
    if (x % 10 == 0) {
      std::cout << std::endl;
    }
  }
  std::cout << std::dec << std::endl;
  std::cout << "Simple8b-encoded (blue): " << std::endl;
  x = 0;
  for (int i = 0; i < b_encoded.size(); i++) {
    x++;
    std::cout << std::hex << b_encoded[i] << " ";
    if (x % 10 == 0) {
      std::cout << std::endl;
    }
  }
  std::cout << std::dec << std::endl;

  std::cout << "encoded size: "
            << (r_encoded.size() + g_encoded.size() + b_encoded.size()) *
                   sizeof(uint64_t)
            << std::endl;
}

int main(void) { LedExample(); }

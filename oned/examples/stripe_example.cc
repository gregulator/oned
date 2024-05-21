// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
//
// Compile and run from root of git repo with:
//
//   clang++ -I. --std=c++20 oned/examples/stripe_example.cc && ./a.out
//
#include "oned/stripe.hpp"

#include <iostream>
#include <vector>

struct RGB {
  uint8_t r, g, b; // red, green, blue
};

// This example creates a Stripe that views the `g` members in an array of
// `RGB` structs. It passes the stripe's `begin()` and `end()` methods to
// `std::min_element` to find the minimum green value.
uint8_t FindMinGreenValue() {
  std::array<RGB, 5> c = {
      RGB{0x00, 0x10, 0x20}, RGB{0x01, 0x11, 0x21}, RGB{0x02, 0x12, 0x22},
      RGB{0x03, 0x13, 0x23}, RGB{0x04, 0x14, 0x24},
  };

  // Create a stripe that is a view of the green channel.
  oned::Stripe<uint8_t> green_channel(&c[0].g, c.size(), sizeof(RGB));

  // We can now pass this stripe to methods that take iterators:
  return *std::min_element(green_channel.begin(), green_channel.end());
}

template <typename T> oned::Stripe<T> EvenElements(std::span<T> span) {
  return oned::Stripe(span, 2);
}

template <typename T> oned::Stripe<T> OddElements(std::span<T> span) {
  return oned::Stripe(span, 2);
}

// This example takes a span of integers and displays the elements with even
// indices followed by the elements with odd indices.
void ShowEvenOdd(std::span<int> ints) {
  oned::Stripe even_elements(ints, 2);
  oned::Stripe odd_elements(ints.last(ints.size() - 1), 2);
  std::cout << "Even elements: ";
  for (int even_element : even_elements) {
    std::cout << even_element << " ";
  }
  std::cout << std::endl << "Odd elements: ";
  for (int odd_element : odd_elements) {
    std::cout << odd_element << " ";
  }
  std::cout << std::endl;
}

void ShowEvenOddExample() {
  std::vector<int> ints{0, 100, 200, 300, 400, 500, 600, 700, 800};
  ShowEvenOdd(ints);
}

// This example uses a Stripe with a negative stride to traverse backwards
// through a vector of ints.
void PrintReversedStripe(oned::Stripe<int> ints) {
  std::cout << "Reverse ints: ";
  for (const int &value : ints) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
}

void ReverseStripeExample() {
  std::vector<int> ints = {0, 1, 2, 3, 4};
  oned::Stripe<int> reverse_ints(&ints[ints.size() - 1], ints.size(),
                                 -sizeof(int));
  PrintReversedStripe(reverse_ints);
}

// This example shows how the default-constructed stripe is empty.
void DefaultConstructedStripeIsEmpty() {
  oned::Stripe<int> stripe;
  std::cout << "The default-constructed stripe has size " << stripe.size()
            << " and stride " << stripe.stride() << " ";
  if (stripe.empty()) {
    std::cout << "and is empty." << std::endl;
  } else {
    std::cout << "and is not empty." << std::endl;
    ;
  }
}

int main() {
  std::cout << "Minimum green value is: 0x" << std::hex
            << (int)FindMinGreenValue() << std::dec << std::endl;

  ShowEvenOddExample();

  ReverseStripeExample();

  DefaultConstructedStripeIsEmpty();
}

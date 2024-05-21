# Oned - Timeseries Utilities

Oned (pronounced "1-D" as in "one-dimensional") is a work-in-progress C++
library for working with timeseries data, in development by Greg Prisament.

## Stripe User Guide

Oned contains a "stripe" library in the self-contained header file `oned/stripe.h`.

A `oned::Stripe` is similar to a `std::span` with the the addition of a stride
between elements. This lets us do neat things, like iterate over a particular
member field in an array-of-structs.

Example:

```
struct RGB {
  uint8_t r, g, b; // red, green, blue
};

uint8_t FindMinGreenValue() {
  std::array<RGB, 10> c = {...}  // Array of 10 RGB values.

  // Create a stripe that is a view of the green channel.
  oned::Stripe<uint8_t> stripe(&c[0].g, c.size(), sizeof(RGB));

  // We can now pass this stripe to methods that take iterators:
  return std::min_element(stripe.begin(), stripe.end());
}
```

### Installation

The Stripe library is entirely contained in the standalone `oned/stripe.h` header file. Just copy this file to your project to use it.

The Stripe library requires C++20 or later.

### Creating a Stripe

A `oned::Stripe` is a "view" container. It does not own the underlying data. It
is the user's responsibility to ensure that all data access via a Stripe is
valid.

A `oned::Stripe<T>` can be constructed using:

```
Stripe(T* data, size_t size, ssize_t stride)
```

Where:
 
1. `T* data` is a pointer to the first element of type T in the view.
2. `size_t size` is the number of elements in the view.
3. `ssize_t stride` is the number of bytes between the start of subsequent
elements.

You may alternatively create a `oned::Stripe` from a `std::span`, optionally skipping elements:

Example:
```
template <typename T>
oned::Stripe<T> EvenElements(std::span<T> span) {
  return oned::Stripe<T>(span, 2);
}

template <typename T>
oned::Stripe<T> OddElements(std::span<T> span) {
  return oned::Stripe<T>(span.last(span.size()-1, 2);
}
```

### Non-positive Stride
A `oned::Stripe` may have a stride that is 0, negative, or less than `sizoef(T)`. In all cases, the element address for index `n` is computed as following:

```
  ((uint8_t*)data) + n*stride
```

For example, if a stride of 0 is used, then the stripe behaves as if it has
`size` elements, but the memory address of each element is the same.

A `oned::Stripe<int32_t>` with stride of `-4` will start at `&data` and
progress downward through memory.

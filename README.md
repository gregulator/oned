# Oned - Timeseries Utilities

Oned (pronounced "1-D" as in "one-dimensional") is a work-in-progress C++
library for working with timeseries data, in development by Greg Prisament.

The library currently provides:

- **stripe.hpp** - Defines `Stripe` which is like a `std::span` with a stride;
  useful for extracting integer sequence views from various memory layouts.
- **delta.hpp** - Defines functions for delta-encoding and delta-decoding of
  integer sequences.

## Stripe User Guide

Oned contains a "stripe" library in the self-contained header file `oned/stripe.hpp`.

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

The Stripe library is entirely contained in the standalone `oned/stripe.hpp` header file. Just copy this file to your project to use it.

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
  return oned::Stripe(span, 2);
}

template <typename T>
oned::Stripe<T> OddElements(std::span<T> span) {
  return oned::Stripe(span.last(span.size()-1, 2);
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

## Delta-Encoding User Guide

Oned contains a library for delta-encoding sequences of integers in the header
file `oned/delta.hpp`.

For example, consider the sequence of integers:

```
ORIGINAL:      4  4  5  5  5  7  7  6  6  6
DELTA-ENCODED: 4  0  1  0  0  2  0 -1  0  0
```

Delta-encoding of timeseries data can turn many of the bits in
the original sequence to 0. This works particularly well on low-frequency
signal data. This is often used to make subsequent data compression more
effective.

Example:
```
std::vector<int> orig = {2, 5, 4, 10, 9};
std::vector<int> encoded;
encoded.resize(orig.size());
oned::DeltaEncode(orig.data, encoded.data, orig.size());
for (int value : encoded) {
  std::cout << value << " ";
}
std::cout << endl;
// OUTPUT:
// 2 3 -1 6 -1
```

### Installation

The Delta library is contained in the standalone `oned/delta.hpp` header file.
It requires the `oned/stripe.hpp` file as well. Just copy these files to your
project to use them.

The Delta library requires C++20 or later.

### Methods
The library provides:
- `DeltaEncode` and `DeltaDecode` functions for encoding/decoding sequences
 of integers.
- `GenericDeltaEncode` and `GenericDeltaDecode` functions for
 encoding/decoding arbitrary types.

The source and destinations can be specified with flat arrays or Stripes of
integer types.

### Full documentation

See `oned/delta.hpp` for full documentation. See
`oned/examples/delta_example.cc` for more examples.

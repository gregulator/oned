# OneD - Timeseries Utilities

OneD (pronounced "1-D" as in "one-dimensional") is a work-in-progress C++
library for working with timeseries data, in development by Greg Prisament.

The library currently provides:

Container views:

- **stripe.hpp** - Defines `Stripe` which is like a `std::span` with a stride;
  useful for extracting integer sequence views from various memory layouts.
- **chunks.hpp** - Defines `Chunks` which breaks up a large sequence of data in
  to smaller sequential block views.

Filter Codecs (Non-Compressing):

- **delta.hpp** - Defines functions for delta-encoding and decoding of
  sequences.
- **xor.hpp** - Defines functions for xor-encoding and decoding of integer
  sequences.

Compression Codecs:

- **runlength.hpp** - Defines functions for runlength-encoding and
  decoding of byte sequences.
- **simple8b.hpp** - Defines functions for simple8b-encoding and
  decoding of 64-bit word sequences.

## Building OneD with Bazel

OneD uses the [bazel](https://bazel.build/) build system. You can build all sources with:

```
bazel build ...
```

You can run example programs with, for example:

```
bazel run oned/examples:xor_example
```

Many of the OneD headers have minimal dependencies or are fully self-contained.
So if you are not using the bazel build system, you may be able to get away
with copying just the headers you need into your project.

## Stripe User Guide

OneD contains a "stripe" library in the self-contained header file `oned/stripe.hpp`.

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

The Stripe library is entirely contained in the standalone `oned/stripe.hpp`
header file. Just copy this file to your project to use it.

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
A `oned::Stripe` may have a stride that is 0, negative, or less than
`sizoef(T)`. In all cases, the element address for index `n` is computed as
following:

```
  ((uint8_t*)data) + n*stride
```

For example, if a stride of 0 is used, then the stripe behaves as if it has
`size` elements, but the memory address of each element is the same.

A `oned::Stripe<int32_t>` with stride of `-4` will start at `&data` and
progress downward through memory.

## Chunk User Guide

OneD contains a "chunks" library in the header file `oned/chunks.hpp`.

The `Chunks` class  conceptually breaks up a large sequence of data into
smaller sequential blocks, called "chunks". Each chunk is a view represented as
a `oned::Stripe<T>`. The underlying data may be a contiguous array or a Stripe.
Each chunk in a Chunks object is the same size, except for the last one, which
may be smaller than the others.

Example:
```

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
```

### Installation
The Chunks library is contained in the `oned/chunks.hpp` header file.  It
requires the `oned/stripe.hpp` file as well. Just copy these files to your
project to use them.

The Chunks library requires C++20 or later.

### Creating a Chunks

A `oned::Chunks` is a "view" container. It does not own the underlying data. It
is the user's responsibility to ensure that all data access via Chunks is
valid.

A `oned::Chunks<T>` can be constructed from a `std::vector<T>`, `std::span<T>` or `oned::Stripe<T>`:

```
  // Construct Chunks from a vector and chunk size.
  Chunks(std::vector<T>& data, size_t chunk_size);

  // Construct Chunks from a span and chunk size.
  Chunks(std::span<T> data, size_t chunk_size);

  // Construct Chunks from a Stripe and chunk size.
  Chunks(Stripe<T> data, size_t chunk_size);
```

### Accessing Individual Chunks

Each individual chunk are represented as `oned::Stripe<T>`, which is a view into the underlying data.

Individual chunks can be accessed with array-like indexing or with iterators.

```
  std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  Chunks chunks(v, 3);

  // Copy first chunk contents to an array.
  std::vector<int> first_chunk(chunks[0].begin(), chunks[0].end());
  // Contains 0, 1, 2

  // Print the first element of each chunk:
  for (oned::Stripe<int> chunk : chunks) {
    std::cout << chunk[0] << std::endl;
  }
  // OUTPUT:
  // 0
  // 3
  // 6
  // 9
```

## Delta-encoding User Guide

OneD contains a library for delta-encoding sequences of integers in the header
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
std::vector<int> encoded(orig.size());
oned::DeltaEncode(orig, encoded);
for (int value : encoded) {
  std::cout << value << " ";
}
std::cout << endl;
// OUTPUT:
// 2 3 -1 6 -1
```

The `oned::DeltaEncode` function takes any `std::sized_range`s as input and
output. The source and destinations must have the same size.

The source and destination may have arbitrary value types, as long as the
`DeltaEncodable` requirements are met (see delta.hpp for details). For a
lossless encoding, use the source and destination value types should be the
same and should both be integers.

### Installation

The Delta library is entirely contained in the standalone `oned/delta.hpp`
header file. Just copy this file to your project to use it.

The Delta library requires C++20 or later.

### Methods

See `oned/delta.hpp` for full documentation. See
`oned/examples/delta_example.cc` for more examples.

## Xor-encoding User Guide

OneD contains a library for xor-encoding sequences of integers in the header
file `oned/delta.hpp`.

For example, consider the sequence of integers:

```
ORIGINAL:      0xFF 0xF7 0xF3 0xF1 0xF0
XOR-ENCODED:   0xFF 0x08 0x04 0x02 0x01
```

Xor-encoding of timeseries data can turn many of the bits in
the original sequence to 0. This often works better than delta-encoding for
floating-point values.

Example:
```
  std::vector<int> orig = {0xFF, 0xF7, 0xF3, 0xF1, 0xF0};
  std::vector<int> encoded;
  encoded.resize(orig.size());
  oned::XorEncode(orig.data(), encoded.data(), orig.size());
  for (int value : encoded) {
    std::cout << std::hex << value << " ";
  }
  std::cout << std::dec << std::endl;
  // OUTPUT:
  // 0xFF 0x8 0x4 0x2 0x1
```

### Installation

The Xor library is contained in the `oned/xor.hpp` header file.  It
requires the `oned/stripe.hpp` file as well. Just copy these files to your
project to use them.

The Xor library requires C++20 or later.

### Methods
The library provides:
- `XorEncode` and `XorDecode` functions for encoding/decoding sequences
 of integers.
- `GenericXorEncode` and `GenericXorDecode` functions for
 encoding/decoding arbitrary types.

The source and destinations can be specified with flat arrays or Stripes of
integer types. To use floating-point types, reinterpret them as appropriately-sized integers:

    oned::XorDecode(oned::Reinterpret<uint64_t>(orig_double_stripe),
                    oned::Reinterpret<uint64_t>(dest_double_stripe));

// Just be aware that platform-specific floating-point representations may
// affect portability.

### Full documentation

See `oned/xor.hpp` for full documentation. See
`oned/examples/xor_example.cc` for more examples.

## Run-length-encoding User Guide

OneD contains a library for runlength-encoding of byte sequences in the header
file `oned/runlength.hpp`.

For example, consider the sequence of bytes:

```
ORIGINAL:           'A' 'A' 'A' 'A' 'B' 'B' 'B'
RUN-LENGTH-ENCODED:  4  'A'  3  'B'
```

Runlength-encoding is effective at compressing sequences that contain runs of
identical bytes. For example, it compresses 64-bit signed integers containing
values in the range (-256, 255) from 8 bytes down to 4 bytes. This makes it
well-suited for data that has been delta-encoded. However, data without
repetition is expanded to 2X the size, so it is not a good choice in that case.

Example:
```
std::vector<uint8_t> orig({'A', 'A', 'A', 'A', 'B', 'B', 'B'});
std::vector<uint8_t> encoded;
size(oned::ComputeRunlengthEncodeSize(orig.data(), orig.size()));
oned::RunlengthStatus status = oned::RunlengthEncode(orig.data(),
    orig.size(), encoded.data(), encoded.size());
if (status != oned::RunlengthStatus::kOk) {
  std::cout << "error code " << (int)status << std::endl;
} else {
  for (int i = 0; i < encoded.size(); i+=2) {
    std::cout << (int)encoded[i] << " '" << encoded[i+1] << "' ";
  }
  std::cout << std::endl;
  // OUTPUT:
  // 4 'A' 3 'B'
}
```

### Installation

The Runlength library is fully contained in standalone `oned/runlength.hpp`
header file. Just copy this file to your project to use it.

The Runlength library requires C++11 or later.

### Methods
The library provides:
- `ComputeRunlengthEncodeSize` and `ComputeRunlengthDecodeSize` functions
   for determining the size (in bytes) of the output buffer, prior to encoding
   or decoding.
- `RunlengthEncode` and `RunlengthDecode` functions for encoding/decoding
   byte sequences.

At this time only contiguous byte sequences are supported, but in the future we
may support other word sizes and input/output types.

### Full documentation

See `oned/runlength.hpp` for full documentation. See
`oned/examples/runlength_example.cc` for more examples.

## Simple8b-encoding User Guide

OneD contains a library for simple8b-encoding of 64-bit word sequences in the header
file `oned/simple8b.hpp`.

The simple8b compresses data by packing multiple integers into a single 64-bit
word. It is limited to values between 0 and 2^60-1.

```
ORIGINAL:           0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
SIMPLE8B-ENCODED:   0x5EDCBA9876543210
```

Runlength-encoding is effective at compressing sequences that can be
represented with varying bit widths. It is well-suited for data that has been
delta-encoded or xor-encoded.

Example:
```
{
  std::vector<uint64_t> v =
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  std::optional<size_t> encoded_size = 
      oned::ComputeSimple8bEncodeSize(v.data(), v.size());
  if (!encoded_size) {
    std::cout << " Error determining simple8b encoding size" << std::endl;
    return;
  }
  std::vector<uint64_t> encoded(*encoded_size);

  oned::Simple8bStatus result =
      oned::Simple8bEncode(v.data(), v.size(), encoded.data(), encoded.size());
  if (result != oned::Simple8bStatus::kOk) {
    std::cout << "Error encoding with simple8b" << std::endl;
    return;
  }

  for (uint64_t v : out) {
    std::cout << std::hex << v << std::endl; 
  }
  // OUTPUT:
  // 0x5EDCBA9876543210
}
```

### Installation

The Simple8b library is fully contained in standalone `oned/simple8b.hpp`
header file. Just copy this file to your project to use it.

The Simple8b library requires C++20 or later.

### Methods
The library provides:
- `ComputeSimple8bEncodeSize` and `ComputeSimple8bDecodeSize` functions for
  determining the size (in 64-bit words) of the output buffer, prior to
  encoding or decoding.
- `Simple8bEncode` and `Simple8bDecode` functions for encoding/decoding
   byte sequences.

At this time only contiguous sequences of 64-bit are supported, but in the
future we may support other word sizes and input/output types.

### Full documentation

See `oned/simple8b.hpp` for full documentation. See `oned/examples/simple8b.cc`
for more examples.

### Build and Test Instructions

Install Bazel and upgrade gcc ( 10 or above since you need c++20)
from root directory run bazel build ...
for testing run bazel test ...
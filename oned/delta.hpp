// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
#ifndef __ONED_DELTA_H__
#define __ONED_DELTA_H__

#include <ranges>

// This file defines functions for delta-encoding of integer sequences (and
// decoding):
//
// - `DeltaEncode` and `DeltaDecode` functions for encoding/decoding sequences
//   of integers.
// - `GenericDeltaEncode` and `GenericDeltaDecode` functions for
//   encoding/decoding arbitrary types.
//
// For an original sequence of values:
//
//    x0, x1, x2, x3, x4
//
// The encoded sequence will be:
//
//    x0, x1-x0, x2-x1, x3-x2, x4-x3
//
// Example:
//
//    std::vector<int> orig = {2, 5, 4, 10, 9};
//    std::vector<int> encoded;
//    encoded.resize(orig.size());
//    oned::DeltaEncode(orig.data, encoded.data, orig.size());
//    for (int value : encoded) {
//      std::cout << value << " ";
//    }
//    std::cout << endl;
//    // OUTPUT:
//    // 2 3 -1 6 -1
//
// Decoding works similarly:
//
//    oned::DeltaDecode(encoded.data, decoded.data, encoded.size());
//    for (int value : decoded) {
//      std::cout << value << " ";
//    }
//    std::cout << endl;
//    // OUTPUT:
//    // 2 5 4 10 9
//
// The source and destinations can be specified with flat arrays or sized
// ranges of integer types.
//
// Arbitrary data types can be encoded using GenericDeltaDecode:
//
//     std::vector<std::complex<double>> values = {...};
//     // Encode in-place.
//     oned::GenericDeltaEncode(values.data(), &value.data(), sizeof(values));
//     // Decode in-place.
//     oned::GenericDeltaDecode(values.data(), &value.data(), sizeof(values));
//     // Note that the round trip is lossy in this example due to
//     // floating-point rounding errors.
//
namespace oned {

// Generates a delta encoding of the first `num_elements` of `source` and
// writes it to `dest`. `source` and `dest` may be equal, in which case the
// encoding will be in-place. Problems will arise if `source[i]` overlaps in
// memory with `dest[j]` for `i > j`.  The `-` operation with operands of type
// `SourceT` must be supported. Both `SourceT` and the type resulting from
// subtraction must be assignable to `DestT`. Floating-point values may be used
// but the results will be lossy.
template <typename SourceT, typename DestT>
void GenericDeltaEncode(SourceT source, DestT dest, size_t num_elements) {
  if (num_elements <= 0) {
    return;
  }
  dest[0] = source[0];
  auto prev = source[0];
  for (size_t i = 1; i < num_elements; ++i) {
    auto old = source[i]; // Needed for in-place encoding.
    dest[i] = source[i] - prev;
    prev = old;
  }
}

// Delta encode a sequence of integers. Values are read from range `Source` and
// written to range `Dest`. The minimum of `source.size()` and `dest.size()` is
// used as the number of elements. `source` and `dest` may overlap, but
// problems will arise if `source[i]` overlaps in memory with `dest[j]` for `i
// > j`.
template <std::ranges::sized_range SourceR, std::ranges::sized_range DestR>
  requires std::is_integral_v<std::ranges::range_value_t<SourceR>> &&
           std::is_same_v<std::ranges::range_value_t<SourceR>,
                          std::ranges::range_value_t<DestR>>
void DeltaEncode(SourceR &&source, DestR &&dest) {
  GenericDeltaEncode<SourceR, DestR>(source, dest,
                                     std::min(source.size(), dest.size()));
}

// Decodes a delta-encoded `source` and writes the result to `dest`. `source`
// and `dest` may be equal, in which case the decoding will be in-place.
// Problems will arise if `source[i]` overlaps in memory with `dest[j]` for `i >
// j`. memory. The `+` operation with operands of type `SourceT` and `DestT`
// must be supported.  Both `SourceT` and the type resulting from addition must
// be assignable to `DestT`. Floating-point values may be used but the results
// will be lossy.
template <typename SourceT, typename DestT>
void GenericDeltaDecode(SourceT source, DestT dest, size_t num_elements) {
  if (num_elements <= 0) {
    return;
  }
  dest[0] = source[0];
  for (size_t i = 1; i < num_elements; ++i) {
    dest[i] = source[i] + dest[i - 1];
  }
}

// Decodes a sequence of delta-encoded integers. Values are read from range
// `Source` and written to range `Dest`. The minimum of `source.size()` and
// `dest.size()` is used as the number of elements.  `source` and `dest` may
// overlap, but problems will arise if `source[i]` overlaps in memory with
// `dest[j]` for `i > j`.
template <std::ranges::sized_range SourceR, std::ranges::sized_range DestR>
  requires std::is_integral_v<std::ranges::range_value_t<SourceR>> &&
           std::is_same_v<std::ranges::range_value_t<SourceR>,
                          std::ranges::range_value_t<DestR>>
void DeltaDecode(SourceR &&source, DestR &&dest) {
  GenericDeltaDecode<SourceR, DestR>(source, dest,
                                     std::min(source.size(), dest.size()));
}

} // namespace oned

#endif // __ONED_DELTA_H__

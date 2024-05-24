// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
#ifndef __ONED_DELTA_H__
#define __ONED_DELTA_H__

#include <ranges>
#include <string>

// This file defines functions for delta-encoding (and delta-decoding) of
// integer sequences:
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
//    std::vector<int> encoded(orig.size());
//    oned::DeltaEncode(orig, encoded);
//    for (int value : encoded) {
//      std::cout << value << " ";
//    }
//    std::cout << endl;
//    // OUTPUT:
//    // 2 3 -1 6 -1
//
// Decoding works similarly:
//
//    std::vector<int> decoded(encoded.size());
//    oned::DeltaDecode(encoded, decoded));
//    for (int value : decoded) {
//      std::cout << value << " ";
//    }
//    std::cout << endl;
//    // OUTPUT:
//    // 2 5 4 10 9
//
namespace oned {

// concept DeltaEncodable
//
// The DeltaEncodable concept specifies the requirements for delta-encoding a
// and delta-decoding a sequence.
template <typename SourceT, typename DestT>
concept DeltaEncodable =
    std::ranges::sized_range<SourceT> && std::ranges::sized_range<DestT> &&
    requires(std::ranges::range_value_t<SourceT> s,
             std::ranges::range_value_t<DestT> d) {
      d = s;
      d = s - d;
      d = s + d;
    };

// enum DeltaResult
//
// The result of a DeltaEncode or DeltaDecode operation.
enum class DeltaResult {
  kOk = 0,           // Success
  kSizeMismatch = 1, // The source and destination sizes mismatch.
};

// DeltaEncode()
//
// Delta encode a sequence of values. Values are read from range `source` and
// written to range `dest`. `source` and `dest` must have the same size.
// `source` and `dest` may overlap, but an incorrect encoding will result if
// `source[i]` overlaps in memory with `dest[j]` for `i > j`.
//
// Returns DeltaResult::kOk on success.
// Returns DeltaResult::kSizeMismatch if `source` and `dest` differ in size.
template <typename SourceR, typename DestR>
  requires DeltaEncodable<SourceR, DestR>
DeltaResult DeltaEncode(SourceR &&source, DestR &&dest);

// DeltaDecode()
//
// Decode a sequence of delta-encoded values. Values are read from range
// `source` and written to range `dest`. `source` and `dest` must have the same
// size.  `source` and `dest` may overlap, but an incorrect encoding will
// result if `source[i]` overlaps in memory with `dest[j]` for `i > j`.
//
// Returns DeltaResult::kOk on success.
// Returns DeltaResult::kSizeMismatch if `source` and `dest` differ in size.
template <typename SourceR, typename DestR>
  requires DeltaEncodable<SourceR, DestR>
DeltaResult DeltaDecode(SourceR &&source, DestR &&dest);

// IMPLEMENTATION BELOW

template <typename SourceR, typename DestR>
  requires DeltaEncodable<SourceR, DestR>
DeltaResult DeltaEncode(SourceR &&source, DestR &&dest) {
  if (std::ranges::size(source) != std::ranges::size(dest)) {
    return DeltaResult::kSizeMismatch;
  }
  auto s = source.begin();
  auto d = dest.begin();
  *d = *s;
  auto prev = *s;
  do {
    ++s;
    ++d;
    auto old = *s; // Needed for in-place encoding.
    *d = *s - prev;
    prev = old;
  } while (s != source.end());
  return DeltaResult::kOk;
}

template <typename SourceR, typename DestR>
  requires DeltaEncodable<SourceR, DestR>
DeltaResult DeltaDecode(SourceR &&source, DestR &&dest) {
  if (std::ranges::size(source) != std::ranges::size(dest)) {
    return DeltaResult::kSizeMismatch;
  }
  auto s = source.begin();
  auto d = dest.begin();
  *d = *s;
  auto prev = *d;
  do {
    ++s;
    ++d;
    *d = *s + prev;
    prev = *d;
  } while (s != source.end());
  return DeltaResult::kOk;
}

static_assert(DeltaEncodable<std::vector<int>, std::vector<double>>);
static_assert(!DeltaEncodable<std::vector<std::string>, std::vector<double>>);
static_assert(!DeltaEncodable<int *, std::vector<double>::iterator>);

} // namespace oned

#endif // __ONED_DELTA_H__

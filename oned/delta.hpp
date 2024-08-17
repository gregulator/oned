// Copyright (c) 2024 The OneD Authors
// See LICENSE file.
#ifndef __ONED_DELTA_H__
#define __ONED_DELTA_H__

#include <ranges>
#include <string>
#include <vector>
#include <immintrin.h>
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
// DeltaEncodeSIMD() is SIMD optimize version of DeltaEncode()
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
// DeltaDecodeSIMD() is SIMD optimize version of DeltaDecode()
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
DeltaResult DeltaEncodeSIMD(SourceR &&source, DestR &&dest) {
  if (std::ranges::size(source) != std::ranges::size(dest)) {
    return DeltaResult::kSizeMismatch;
  }
  const size_t size = std::ranges::size(source);
  const size_t simd_width = 8;
  size_t i = 0;

  dest[i] = source[i];
  int prev = source[i];
  i++;

  for (; i + simd_width <= size; i += simd_width) {
    auto old = source[i+simd_width - 1];
    __m256i src_chunk = _mm256_loadu_si256((__m256i*)&source[i]);
    __m256i prev_vec = _mm256_set_epi32(
        source[i + 6], source[i + 5], source[i + 4], source[i + 3], 
        source[i + 2], source[i + 1], source[i], prev
    );
    __m256i result = _mm256_sub_epi32(src_chunk, prev_vec);
    _mm256_storeu_si256((__m256i*)&dest[i], result);
    prev = old;
  }

  // Scalar encoding for the remaining elements
  for (; i < size; ++i) {
    auto old = source[i]; // Needed for in-place encoding.
    dest[i] = source[i] - prev;
    prev = old;
  }

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

template <typename SourceR, typename DestR>
requires DeltaEncodable<SourceR, DestR>
DeltaResult DeltaDecodeSIMD(SourceR &&source, DestR &&dest) {
    if (std::ranges::size(source) != std::ranges::size(dest)) {
        return DeltaResult::kSizeMismatch;
    }

    const size_t size = std::ranges::size(source);
    const size_t simd_width = 8;  // Width for SIMD operations (256-bit registers)
    const size_t block_size = 4;  // Block size for accumulation


    size_t i = 0;
    __m256i prefix_sum = _mm256_setzero_si256();

    // First pass: Compute prefix sums for each block
    for (; i + simd_width <= size; i += simd_width) {
        __m256i src_chunk = _mm256_loadu_si256((__m256i*)&source[i]);

        // Compute prefix sums for the current chunk
        __m256i prefix = _mm256_add_epi32(src_chunk, _mm256_slli_si256(src_chunk, 4));
        prefix = _mm256_add_epi32(prefix, _mm256_slli_si256(prefix, 8));

        // Store results
        _mm256_storeu_si256((__m256i*)&dest[i], prefix);

    }
    
    // Second pass: Accumulate prefix sums and propagate
    __m128i s = _mm_setzero_si128(); // Initialize block accumulator for 4-element blocks
    for (size_t j = 0; j + block_size < size; j += block_size){

        __m128i d = (__m128i) _mm_broadcast_ss((float*) &dest[j + 3]);
        __m128i x = _mm_load_si128((__m128i*) &dest[j]);
        x = _mm_add_epi32(s, x);
        _mm_store_si128((__m128i*) &dest[j], x);
        
        s = _mm_add_epi32(s, d);
        // Update prefix_sum based on the last element of the processed block
        prefix_sum = _mm256_set1_epi32(_mm_cvtsi128_si32(_mm_shuffle_epi32(s, 0xFF))); // Broadcast the last element of s
    }
    // Process remaining elements if size is not a perfect multiple of block_size
    for (; i < size; ++i) {
        dest[i] = source[i] + _mm256_extract_epi32(prefix_sum, 0);
        prefix_sum = _mm256_set1_epi32(dest[i]); // Update prefix_sum for the next element
    }
    return DeltaResult::kOk;
}

static_assert(DeltaEncodable<std::vector<int>, std::vector<double>>);
static_assert(!DeltaEncodable<std::vector<std::string>, std::vector<double>>);
static_assert(!DeltaEncodable<int *, std::vector<double>::iterator>);

} // namespace oned

#endif // __ONED_DELTA_H__

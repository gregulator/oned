// Copyright (c) 2024 The OneD Authors
// See LICENSE file.
#ifndef __ONED_SIMPLE8B_HPP__
#define __ONED_SIMPLE8B_HPP__

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <immintrin.h>
// This file defines functions for Simple8b encoding and decoding of integer
// sequences. The algorithm is as described by Ann and Moffat in "Index
// compression using 64-bit words", Softw. Pract. Exper. 2010; 40:131–147
//
// Simple8b packs a variable number of integers into 64-bit words. The 4 most
// significant bits of the 64-bit word is a "selector", which describes the
// packing according to the following table:
//
// ┌──────────────┬───────────────────────────────────────────────────────────┐
// │   Selector   │     0    1   2   3   4   5   6   7  8  9  0 11 12 13 14 15│
// ├──────────────┼───────────────────────────────────────────────────────────┤
// │     Bits     │     0    0   1   2   3   4   5   6  7  8 10 12 15 20 30 60│
// ├──────────────┼───────────────────────────────────────────────────────────┤
// │      N       │    240  120  60  30  20  15  12  10  8  7  6  5  4  3  2 1│
// ├──────────────┼───────────────────────────────────────────────────────────┤
// │   Wasted Bits│     60   60   0   0   0   0  12   0  4  4  0  0  0  0  0 0│
// └──────────────┴───────────────────────────────────────────────────────────┘
//
// It is capable of encoding unsigned integers with values betweeen 0 and (2^60
// -1), or signed integers between -2^59 and (2^59-1) in a single word.
//
// The functions `ComputeSimple8bEncodeSize` and `ComputeSimple8bDecodeSize`
// functions can be used to determine the size (in word count) of the output
// buffer, prior to encoding or decoding.
//
// The `Simple8bEncode` and `Simple8bDecode` functions perform the
// encoding/decoding. The input to encode, and the output to decode, may be any
// integral type.

namespace oned {

// enum Simple8bStatus
//
// Possible return status of simple8b encoding routines.
enum class Simple8bStatus {
  kOk = 0,
  // Operation failed because the output buffer is too small to hold the
  // results.
  kInsufficientSpace = 1,
  // Encoding failed because the input contains a value outside the min/max
  // range, which is between 0 and (2^60 -1) for unsigned integers and between
  // -2^59 and (2^59-1) for signed integers.
  kValueOutOfRange = 2,
};

// ComputeSimple8bEncodeSize
//
// Determine the size of the output buffer (number of 64-words) needed to
// simple8b encode `source`.
// Returns `std::nullopt` if source contains an out-of-range value.
template <std::integral SourceT>
std::optional<size_t> ComputeSimple8bEncodeSize(SourceT *source,
                                                size_t source_size);

// Simple8bEncode
//
// Simple8b encodes `source` and writes the result to `dest`. `source_size` is
// the number of integer words to encode, starting from `source`. `dest_size`
// is the number of 64-bit words in the output buffer. The behavior is
// undefined if if `source` and `dest` overlap.
//
// Returns Simple8bStatus::kOk on success.
// Returns Simple8bStatus::kInsufficientSpace if `dest_size` is not large
// enough to the hold the result.
// Returns Simple8bStatus::kValueOutOfRange if source contains a value outside
// the allowed range.
template <std::integral SourceT>
[[nodiscard]] Simple8bStatus Simple8bEncode(SourceT *source, size_t source_size,
                                            uint64_t *dest, size_t dest_size);

// ComputeSimple8bDecodeSize
//
// Determine the size of the output buffer (in number of words) needed to
// simple8b decode `source`.
[[nodiscard]] static inline size_t
ComputeSimple8bDecodeSize(uint64_t *source, size_t source_size);

// Simple8bDecode
//
// Simple8b decodes `source` and writes the result to `dest`. `source_size` is
// the number of 64-bit words to decode, starting from `source`. `dest_size` is
// the number of integer words in the output buffer. The behavior is undefined
// if `source` and `dest` overlap. The destination type must be large enough to
// hold all encoded values, and should match the signedness of the encoded
// source.
//
// Returns Simple8bStatus::kOk on success.
// Returns Simple8bStatus::kInsufficientSpace if `dest_size` is not large
// enough to the hold the result.
template <std::integral DestT>
[[nodiscard]] Simple8bStatus Simple8bDecode(uint64_t *source,
                                            size_t source_size, DestT *dest,
                                            size_t dest_size);

// IMPLEMENTATION BELOW

namespace simple8b_internal {

struct Simple8bBitPacking {
  uint8_t selector;
  uint8_t bitwidth;
  uint8_t count;
};

static inline Simple8bBitPacking GetBitPacking(uint8_t selector) {
  static Simple8bBitPacking bit_packings[] = {
      {.selector = 0, .bitwidth = 0, .count = 240},
      {.selector = 1, .bitwidth = 0, .count = 120},
      {.selector = 2, .bitwidth = 1, .count = 60},
      {.selector = 3, .bitwidth = 2, .count = 30},
      {.selector = 4, .bitwidth = 3, .count = 20},
      {.selector = 5, .bitwidth = 4, .count = 15},
      {.selector = 6, .bitwidth = 5, .count = 12},
      {.selector = 7, .bitwidth = 6, .count = 10},
      {.selector = 8, .bitwidth = 7, .count = 8},
      {.selector = 9, .bitwidth = 8, .count = 7},
      {.selector = 10, .bitwidth = 10, .count = 6},
      {.selector = 11, .bitwidth = 12, .count = 5},
      {.selector = 12, .bitwidth = 15, .count = 4},
      {.selector = 13, .bitwidth = 20, .count = 3},
      {.selector = 14, .bitwidth = 30, .count = 2},
      {.selector = 15, .bitwidth = 60, .count = 1},
  };
  return bit_packings[selector];
}
static const __m256i precomputed_shift_amounts[16][4] = {
    {_mm256_setzero_si256(), _mm256_setzero_si256(), _mm256_setzero_si256(), _mm256_setzero_si256()},
    {_mm256_set_epi64x(3, 2, 1, 0), _mm256_set_epi64x(7, 6, 5, 4), _mm256_set_epi64x(11, 10, 9, 8), _mm256_set_epi64x(15, 14, 13, 12)},
    {_mm256_set_epi64x(6, 4, 2, 0), _mm256_set_epi64x(14, 12, 10, 8), _mm256_set_epi64x(22, 20, 18, 16), _mm256_set_epi64x(30, 28, 26, 24)},
    {_mm256_set_epi64x(9, 6, 3, 0), _mm256_set_epi64x(21, 18, 15, 12), _mm256_set_epi64x(33, 30, 27, 24), _mm256_set_epi64x(45, 42, 39, 36)},
    {_mm256_set_epi64x(12, 9, 6, 3), _mm256_set_epi64x(28, 25, 22, 19), _mm256_set_epi64x(44, 41, 38, 35), _mm256_set_epi64x(60, 57, 54, 51)},
    {_mm256_set_epi64x(15, 12, 9, 6), _mm256_set_epi64x(35, 32, 29, 26), _mm256_set_epi64x(55, 52, 49, 46), _mm256_set_epi64x(75, 72, 69, 66)},
    {_mm256_set_epi64x(18, 15, 12, 9), _mm256_set_epi64x(42, 39, 36, 33), _mm256_set_epi64x(66, 63, 60, 57), _mm256_set_epi64x(90, 87, 84, 81)},
    {_mm256_set_epi64x(21, 18, 15, 12), _mm256_set_epi64x(49, 46, 43, 40), _mm256_set_epi64x(77, 74, 71, 68), _mm256_set_epi64x(105, 102, 99, 96)},
    {_mm256_set_epi64x(24, 21, 18, 15), _mm256_set_epi64x(56, 53, 50, 47), _mm256_set_epi64x(88, 85, 82, 79), _mm256_set_epi64x(120, 117, 114, 111)},
    {_mm256_set_epi64x(27, 24, 21, 18), _mm256_set_epi64x(63, 60, 57, 54), _mm256_set_epi64x(99, 96, 93, 90), _mm256_set_epi64x(135, 132, 129, 126)},
    {_mm256_set_epi64x(30, 27, 24, 21), _mm256_set_epi64x(70, 67, 64, 61), _mm256_set_epi64x(110, 107, 104, 101), _mm256_set_epi64x(150, 147, 144, 141)},
    {_mm256_set_epi64x(36, 30, 24, 18), _mm256_set_epi64x(84, 78, 72, 66), _mm256_set_epi64x(132, 126, 120, 114), _mm256_set_epi64x(180, 174, 168, 162)},
    {_mm256_set_epi64x(45, 36, 27, 18), _mm256_set_epi64x(105, 96, 87, 78), _mm256_set_epi64x(165, 156, 147, 138), _mm256_set_epi64x(225, 216, 207, 198)},
    {_mm256_set_epi64x(60, 45, 30, 15), _mm256_set_epi64x(140, 125, 110, 95), _mm256_set_epi64x(220, 205, 190, 175), _mm256_set_epi64x(300, 285, 270, 255)},
    {_mm256_set_epi64x(90, 60, 30, 0), _mm256_set_epi64x(210, 180, 150, 120), _mm256_set_epi64x(330, 300, 270, 240), _mm256_set_epi64x(450, 420, 390, 360)},
    {_mm256_set_epi64x(180, 120, 60, 0), _mm256_setzero_si256(), _mm256_setzero_si256(), _mm256_setzero_si256()}};

inline constexpr uint8_t kMaxSelector = 15;

struct EncodeWordsResult {
  bool ok;
  uint64_t *next_read;
  uint64_t encoded_value;
};

template <std::unsigned_integral SourceT>
EncodeWordsResult EncodeWords(SourceT *source, Simple8bBitPacking packing,
                              size_t max_count) {
  uint64_t out = ((uint64_t)packing.selector) << 60;
  size_t i;
  for (i = 0; i < packing.count; i++) {
    if (i >= max_count) {
      // End reached. Use more bits to avoid padded output.
      return EncodeWordsResult{.ok = false};
    }
    if (source[i] >= (((uint64_t)1) << packing.bitwidth)) {
      // Value is too big. Use more bits.
      return EncodeWordsResult{.ok = false};
    }
    out |= ((uint64_t)source[i]) << (packing.bitwidth * i);
  }
  return EncodeWordsResult{
      .ok = true,
      .next_read = (uint64_t *)&source[i],
      .encoded_value = out,
  };
}
template <std::unsigned_integral SourceT>
EncodeWordsResult EncodeWords_SIMD(SourceT *source, Simple8bBitPacking packing, size_t max_count) {
  constexpr size_t simd_width = 16;
    // Define shift amounts for each set of 4 elements  

    uint64_t out = ((uint64_t)packing.selector) << 60;
    size_t i = 0;

    for (; i + simd_width <= packing.count; i += simd_width) {
        // Load 16 elements from source in chunks of 4
        __m256i values[4];
        for (int j = 0; j < 4; ++j) {
            values[j] = _mm256_loadu_si256((__m256i*)(source + i + j * 4));
        }

        // Check if all values fit in bitwidth (if not, bail out)
        __m256i overflow_mask = _mm256_set1_epi16(((uint16_t)1 << packing.bitwidth) - 1);
        for (int j = 0; j < 4; ++j) {
            if (_mm256_movemask_epi8(_mm256_cmpgt_epi16(values[j], overflow_mask)) != 0) {
                return EncodeWordsResult{.ok = false};
            }
        }

        // Apply shifts and mask values to fit within the bitwidth
        __m256i combined = _mm256_setzero_si256();
        for (int j = 0; j < 4; ++j) {
            __m256i masked_values = _mm256_and_si256(values[j], overflow_mask);
            __m256i shifted = _mm256_sllv_epi64(masked_values, precomputed_shift_amounts[packing.bitwidth][j]);
            combined = _mm256_or_si256(combined, shifted);
        }

        // Store the combined 256-bit result directly to a 64-bit array
        uint64_t combined_result[4];
        _mm256_storeu_si256((__m256i*)combined_result, combined);

        // Accumulate the combined result into the final output
        out |= combined_result[0] | combined_result[1] | combined_result[2] | combined_result[3];
    }


  // Process remaining elements if any
  for (; i < packing.count; i++) {
    if (i >= max_count) {
      return EncodeWordsResult{.ok = false};
    }
    if (source[i] >= (((uint64_t)1) << packing.bitwidth)) {
      return EncodeWordsResult{.ok = false};
    }
    out |= ((uint64_t)source[i]) << (packing.bitwidth * i);
  }

  return EncodeWordsResult{
      .ok = true,
      .next_read = (uint64_t *)&source[i],
      .encoded_value = out,
  };
}



template <std::signed_integral SourceT>
EncodeWordsResult EncodeWords(SourceT *source, Simple8bBitPacking packing,
                              size_t max_count) {
  uint64_t out = ((uint64_t)packing.selector) << 60;
  size_t i;
  for (i = 0; i < packing.count; i++) {
    if (i >= max_count) {
      // End reached. Use more bits to avoid padded output.
      return EncodeWordsResult{.ok = false};
    }
    if (packing.bitwidth == 0 && source[i] != 0) {
      // Special handling for 0 needed in signed case.
      return EncodeWordsResult{.ok = false};
    }
    if (source[i] >= (((int64_t)1) << (packing.bitwidth - 1)) ||
        source[i] < -((int64_t)1 << (packing.bitwidth - 1))) {
      // Value is out of range. Use more bits.
      return EncodeWordsResult{.ok = false};
    }
    // Check this?
    out |= ((uint64_t)((int64_t)source[i]) &
            (((uint64_t)1 << packing.bitwidth) - 1))
           << (packing.bitwidth * i);
  }
  return EncodeWordsResult{
      .ok = true,
      .next_read = (uint64_t *)&source[i],
      .encoded_value = out,
  };
}
template <std::signed_integral SourceT>
EncodeWordsResult EncodeWords_SIMD(SourceT *source, Simple8bBitPacking packing, size_t max_count)
{
  constexpr size_t simd_width = 16;

  uint64_t out = ((uint64_t)packing.selector) << 60;
  size_t i = 0;

  for (; i + simd_width <= packing.count; i += simd_width)
  {
    __m256i values[4];
    __m256i shifted_values[4];

    for (int j = 0; j < 4; ++j)
    {
      values[j] = _mm256_loadu_si256((__m256i *)(source + i + j * 4));

      if (packing.bitwidth == 0)
      {
        if (_mm256_movemask_epi8(_mm256_cmpeq_epi16(values[j], _mm256_setzero_si256())) != 0xFFFF)
        {
          return EncodeWordsResult{.ok = false};
        }
        continue;
      }

      // Check if all values fit in bitwidth (if not, bail out)
      int64_t max_val = ((int64_t)1 << (packing.bitwidth - 1)) - 1;
      int64_t min_val = -((int64_t)1 << (packing.bitwidth - 1));
      __m256i max_mask = _mm256_set1_epi16(max_val);
      __m256i min_mask = _mm256_set1_epi16(min_val);
      __m256i overflow_mask = _mm256_set1_epi16(((int16_t)1 << packing.bitwidth) - 1);

      // Check if any values are out of range
      if (_mm256_movemask_epi8(_mm256_or_si256(_mm256_cmpgt_epi16(values[j], max_mask), _mm256_cmpgt_epi16(min_mask, values[j]))) != 0)
      {
        return EncodeWordsResult{.ok = false};
      }

      // Mask and shift each value appropriately
      __m256i masked_values = _mm256_and_si256(values[j], overflow_mask);
      shifted_values[j] = _mm256_sllv_epi64(masked_values, precomputed_shift_amounts[packing.bitwidth][j]);
    }

    // Extract and combine results from each set of shifted values
    uint64_t lanes[16];
    for (int j = 0; j < 4; ++j)
    {
      lanes[j * 4 + 0] = _mm256_extract_epi64(shifted_values[j], 0);
      lanes[j * 4 + 1] = _mm256_extract_epi64(shifted_values[j], 1);
      lanes[j * 4 + 2] = _mm256_extract_epi64(shifted_values[j], 2);
      lanes[j * 4 + 3] = _mm256_extract_epi64(shifted_values[j], 3);
    }

    uint64_t masks[4] = {0xFFF, 0xFFFFFF, 0xFFFFFFFFF, 0xFFFFFFFFFFFF};
    for (int j = 0; j < 16; ++j)
    {
      lanes[j] &= masks[j / 4];
      out |= lanes[j];
    }
  }

  // Process remaining elements if any
  for (; i < packing.count; i++)
  {
    if (i >= max_count)
    {
      return EncodeWordsResult{.ok = false};
    }
    if (packing.bitwidth == 0 && source[i] != 0)
    {
      return EncodeWordsResult{.ok = false};
    }
    if (source[i] >= (((int64_t)1) << (packing.bitwidth - 1)) ||
        source[i] < -((int64_t)1 << (packing.bitwidth - 1)))
    {
      return EncodeWordsResult{.ok = false};
    }
    out |= ((uint64_t)((int64_t)source[i]) &
            (((uint64_t)1 << packing.bitwidth) - 1))
           << (packing.bitwidth * i);
  }

  return EncodeWordsResult{
      .ok = true,
      .next_read = (uint64_t *)&source[i],
      .encoded_value = out,
  };
}

} // namespace simple8b_internal

template <std::integral SourceT>
[[nodiscard]] std::optional<size_t>
ComputeSimple8bEncodeSize(SourceT *source, size_t source_size) {
  SourceT *read = source;
  SourceT *read_end = &source[source_size];
  size_t dest_size = 0;
  while (read < read_end) {
    // Try possible encodings, starting with most compact.
    uint8_t i;
    for (i = 0; i <= simple8b_internal::kMaxSelector; i++) {
      simple8b_internal::Simple8bBitPacking packing =
          simple8b_internal::GetBitPacking(i);
      simple8b_internal::EncodeWordsResult result =
          simple8b_internal::EncodeWords(read, packing, (read_end - read));
      if (!result.ok) {
        // Try the next packing.
        continue;
      }
      read = (SourceT *)result.next_read;
      dest_size++;
      break;
    }
    if (i > simple8b_internal::kMaxSelector) {
      // No possible selector found.
      return std::nullopt;
    }
  }
  return dest_size;
}

template <std::integral SourceT>
[[nodiscard]] Simple8bStatus Simple8bEncode(SourceT *source, size_t source_size,
                                            uint64_t *dest, size_t dest_size) {
  SourceT *read = source;
  SourceT *read_end = &source[source_size];
  uint64_t *write = dest;
  uint64_t *write_end = &dest[dest_size];
  while (read < read_end) {
    if (write >= write_end) {
      // Hit end of destination before reading finished.
      return Simple8bStatus::kInsufficientSpace;
    }
    // Try possible encodings, starting with most compact.
    uint8_t i;
    for (i = 0; i <= simple8b_internal::kMaxSelector; i++) {
      simple8b_internal::Simple8bBitPacking packing =
          simple8b_internal::GetBitPacking(i);
      simple8b_internal::EncodeWordsResult result =
          simple8b_internal::EncodeWords(read, packing, (read_end - read));
      if (!result.ok) {
        // Try the next packing.
        continue;
      }
      read = (SourceT *)result.next_read;
      *write = result.encoded_value;
      write++;
      break;
    }
    if (i > simple8b_internal::kMaxSelector) {
      // No possible selector found.
      return Simple8bStatus::kValueOutOfRange;
    }
  }
  return Simple8bStatus::kOk;
}


//SIMD optimized version for encode

//SIMD
template <std::integral SourceT>
[[nodiscard]] std::optional<size_t>
ComputeSimple8bEncodeSize_SIMD(SourceT *source, size_t source_size) {
  SourceT *read = source;
  SourceT *read_end = &source[source_size];
  size_t dest_size = 0;
  while (read < read_end) {
    // Try possible encodings, starting with most compact.
    uint8_t i;
    for (i = 0; i <= simple8b_internal::kMaxSelector; i++) {
      simple8b_internal::Simple8bBitPacking packing =
          simple8b_internal::GetBitPacking(i);
      simple8b_internal::EncodeWordsResult result =
          simple8b_internal::EncodeWords_SIMD(read, packing, (read_end - read));
      if (!result.ok) {
        // Try the next packing.
        continue;
      }
      read = (SourceT *)result.next_read;
      dest_size++;
      break;
    }
    if (i > simple8b_internal::kMaxSelector) {
      // No possible selector found.
      return std::nullopt;
    }
  }
  return dest_size;
}

template <std::integral SourceT>
[[nodiscard]] Simple8bStatus Simple8bEncode_SIMD(SourceT *source, size_t source_size,
                                            uint64_t *dest, size_t dest_size) {
  SourceT *read = source;
  SourceT *read_end = &source[source_size];
  uint64_t *write = dest;
  uint64_t *write_end = &dest[dest_size];
  while (read < read_end) {
    if (write >= write_end) {
      // Hit end of destination before reading finished.
      return Simple8bStatus::kInsufficientSpace;
    }
    // Try possible encodings, starting with most compact.
    uint8_t i;
    for (i = 0; i <= simple8b_internal::kMaxSelector; i++) {
      simple8b_internal::Simple8bBitPacking packing =
          simple8b_internal::GetBitPacking(i);
      simple8b_internal::EncodeWordsResult result =
          simple8b_internal::EncodeWords_SIMD(read, packing, (read_end - read));
      if (!result.ok) {
        // Try the next packing.
        continue;
      }
      read = (SourceT *)result.next_read;
      *write = result.encoded_value;
      write++;
      break;
    }
    if (i > simple8b_internal::kMaxSelector) {
      // No possible selector found.
      return Simple8bStatus::kValueOutOfRange;
    }
  }
  return Simple8bStatus::kOk;
}


[[nodiscard]] static inline size_t
ComputeSimple8bDecodeSize(uint64_t *source, size_t source_size) {
  uint64_t *read = source;
  uint64_t *read_end = &source[source_size];
  size_t out = 0;
  while (read < read_end) {
    uint8_t selector = uint8_t(*read >> 60);
    simple8b_internal::Simple8bBitPacking packing =
        simple8b_internal::GetBitPacking(selector);
    out += packing.count;
    read++;
  }
  return out;
}

template <std::unsigned_integral DestT>
[[nodiscard]] Simple8bStatus Simple8bDecode(uint64_t *source,
                                            size_t source_size, DestT *dest,
                                            size_t dest_size) {
  uint64_t *read = source;
  uint64_t *read_end = &source[source_size];
  DestT *write = dest;
  DestT *write_end = &dest[dest_size];
  while (read < read_end) {
    uint8_t selector = uint8_t(*read >> 60);
    simple8b_internal::Simple8bBitPacking packing =
        simple8b_internal::GetBitPacking(selector);
    for (uint8_t i = 0; i < packing.count; i++) {
      if (write >= write_end) {
        return Simple8bStatus::kInsufficientSpace;
      }
      *write = (DestT)((*read >> (packing.bitwidth * i)) &
                       ((((uint64_t)1) << packing.bitwidth) - 1));
      write++;
    }
    read++;
  }
  return Simple8bStatus::kOk;
}

template <std::unsigned_integral DestT>
[[nodiscard]] Simple8bStatus Simple8bDecode_SIMD(uint64_t *source,
                                                 size_t source_size, DestT *dest,
                                                 size_t dest_size) {
  uint64_t *read = source;
  uint64_t *read_end = &source[source_size];
  DestT *write = dest;
  DestT *write_end = &dest[dest_size];

  constexpr size_t simd_width = 16; // Process 4 values at once (64 bits * 4 = 256 bits)
  
  while (read < read_end) {
    uint8_t selector = uint8_t(*read >> 60);  // Get selector from the most significant bits
    simple8b_internal::Simple8bBitPacking packing = simple8b_internal::GetBitPacking(selector);
    
    const __m256i shift_amounts = _mm256_set_epi64x(
        packing.bitwidth * 3, packing.bitwidth * 2, packing.bitwidth * 1, 0);
    const __m256i shift_amounts_1 = _mm256_set_epi64x(
        packing.bitwidth * 7, packing.bitwidth * 6, packing.bitwidth * 5, packing.bitwidth * 4);
    const __m256i shift_amounts_2 = _mm256_set_epi64x(
        packing.bitwidth * 11, packing.bitwidth * 10, packing.bitwidth * 9, packing.bitwidth * 8);
    const __m256i shift_amounts_3 = _mm256_set_epi64x(
        packing.bitwidth * 15, packing.bitwidth * 14, packing.bitwidth * 13, packing.bitwidth * 12);
        
    // SIMD-friendly processing: handle values in groups of 4
    size_t i = 0;
    for (; i + simd_width <= packing.count; i += simd_width) {
      if (write + simd_width > write_end) {
        return Simple8bStatus::kInsufficientSpace;
      }

      // Load the 64-bit encoded value into a 256-bit register (simulated as four 64-bit lanes)
      __m256i encoded_value = _mm256_set1_epi64x(*read);

      // Shift the encoded value to extract the packed elements
      
      __m256i shifted_values = _mm256_srlv_epi64(encoded_value, shift_amounts);
      __m256i shifted_values1 = _mm256_srlv_epi64(encoded_value, shift_amounts_1);
      __m256i shifted_values2 = _mm256_srlv_epi64(encoded_value, shift_amounts_2);
      __m256i shifted_values3 = _mm256_srlv_epi64(encoded_value, shift_amounts_3);
      // Mask out the irrelevant bits
      __m256i bitmask = _mm256_set1_epi64x((1ULL << packing.bitwidth) - 1);
      __m256i decoded_values = _mm256_and_si256(shifted_values, bitmask);
      __m256i decoded_values1 = _mm256_and_si256(shifted_values1, bitmask);
      __m256i decoded_values2 = _mm256_and_si256(shifted_values2, bitmask);
      __m256i decoded_values3 = _mm256_and_si256(shifted_values3, bitmask);
      // Store the decoded values to the destination
      _mm256_storeu_si256(reinterpret_cast<__m256i *>(write + 0), decoded_values);
      _mm256_storeu_si256(reinterpret_cast<__m256i *>(write + 4), decoded_values1);
      _mm256_storeu_si256(reinterpret_cast<__m256i *>(write + 8), decoded_values2);
      _mm256_storeu_si256(reinterpret_cast<__m256i *>(write + 12), decoded_values3);


      // Move the write pointer ahead by 4 (since we processed 4 values)
      write += simd_width;
    }

    // Handle the remaining values (if fewer than 4 values are left)
    for (; i < packing.count; i++) {
      if (write >= write_end) {
        return Simple8bStatus::kInsufficientSpace;
      }
      *write = (DestT)((*read >> (packing.bitwidth * i)) & ((1ULL << packing.bitwidth) - 1));
      write++;
    }

    read++;
  }

  return Simple8bStatus::kOk;
}


template <std::signed_integral DestT>
[[nodiscard]] Simple8bStatus Simple8bDecode(uint64_t *source,
                                            size_t source_size, DestT *dest,
                                            size_t dest_size) {
  uint64_t *read = source;
  uint64_t *read_end = &source[source_size];
  DestT *write = dest;
  DestT *write_end = &dest[dest_size];
  while (read < read_end) {
    uint8_t selector = uint8_t(*read >> 60);
    simple8b_internal::Simple8bBitPacking packing =
        simple8b_internal::GetBitPacking(selector);
    for (uint8_t i = 0; i < packing.count; i++) {
      if (write >= write_end) {
        return Simple8bStatus::kInsufficientSpace;
      }
      if (*read && (*read >> (packing.bitwidth * (i + 1) - 1) & 0x1)) {
        // negative
        *write = (DestT)((*read >> (packing.bitwidth * i)) |
                         (0xFFFFFFFFFFFFFFFF ^
                          ((((uint64_t)1) << packing.bitwidth) - 1)));
      } else {
        // positive
        *write = (DestT)((*read >> (packing.bitwidth * i)) &
                         ((((uint64_t)1) << packing.bitwidth) - 1));
      }
      write++;
    }
    read++;
  }
  return Simple8bStatus::kOk;
}

template <std::signed_integral DestT>
[[nodiscard]] Simple8bStatus Simple8bDecode_SIMD(uint64_t *source,
                                                 size_t source_size, DestT *dest,
                                                 size_t dest_size) {
    uint64_t *read = source;
    uint64_t *read_end = &source[source_size];
    DestT *write = dest;
    DestT *write_end = &dest[dest_size];

    constexpr size_t simd_width = 16; // Process 4 values at once (64 bits * 4 = 256 bits)

    while (read < read_end) {
        uint8_t selector = uint8_t(*read >> 60);
        simple8b_internal::Simple8bBitPacking packing = simple8b_internal::GetBitPacking(selector);

        // Load encoded data into a SIMD register
        const __m256i shift_amounts = _mm256_set_epi64x(
            packing.bitwidth * 3, packing.bitwidth * 2, packing.bitwidth * 1, 0);
        const __m256i shift_amounts_1 = _mm256_set_epi64x(
            packing.bitwidth * 7, packing.bitwidth * 6, packing.bitwidth * 5, packing.bitwidth * 4);
        const __m256i shift_amounts_2 = _mm256_set_epi64x(
            packing.bitwidth * 11, packing.bitwidth * 10, packing.bitwidth * 9, packing.bitwidth * 8);
        const __m256i shift_amounts_3 = _mm256_set_epi64x(
            packing.bitwidth * 15, packing.bitwidth * 14, packing.bitwidth * 13, packing.bitwidth * 12);


        __m256i encoded_value = _mm256_set1_epi64x(*read);

        size_t i = 0;
        for (; i + simd_width <= packing.count; i += simd_width) {
            if (write + simd_width > write_end) {
                return Simple8bStatus::kInsufficientSpace;
            }

            // Calculate shift amounts for extracting the values

            // Extract values from the encoded data
            __m256i shifted_values = _mm256_srlv_epi64(encoded_value, shift_amounts);
            __m256i shifted_values_1 = _mm256_srlv_epi64(encoded_value, shift_amounts_1);
            __m256i shifted_values_2 = _mm256_srlv_epi64(encoded_value, shift_amounts_2);
            __m256i shifted_values_3 = _mm256_srlv_epi64(encoded_value, shift_amounts_3);

            
            __m256i bitmask = _mm256_set1_epi64x((1ULL << packing.bitwidth) - 1);

            // Apply the bitmask to extract only the valid bits
            __m256i decoded_values = _mm256_and_si256(shifted_values, bitmask);
            __m256i decoded_values_1 = _mm256_and_si256(shifted_values_1, bitmask);
            __m256i decoded_values_2 = _mm256_and_si256(shifted_values_2, bitmask);
            __m256i decoded_values_3 = _mm256_and_si256(shifted_values_3, bitmask);
            
            //__m256i extracted_values = _mm256_and_si256(shifted_values, bitmask);

            // Create a mask for signed values
            __m256i sign_bit_mask = _mm256_set1_epi64x(1ULL << (packing.bitwidth - 1));
            decoded_values = _mm256_sub_epi64(_mm256_xor_si256(decoded_values, sign_bit_mask), sign_bit_mask);
            decoded_values_1 = _mm256_sub_epi64(_mm256_xor_si256(decoded_values_1, sign_bit_mask), sign_bit_mask);
            decoded_values_2 = _mm256_sub_epi64(_mm256_xor_si256(decoded_values_2, sign_bit_mask), sign_bit_mask);
            decoded_values_3 = _mm256_sub_epi64(_mm256_xor_si256(decoded_values_3, sign_bit_mask), sign_bit_mask);


            //__m256i sign_mask = _mm256_set1_epi64x(1ULL << (packing.bitwidth - 1));
            //__m256i is_negative = _mm256_cmpgt_epi64(extracted_values, sign_mask);

            // Sign extend the values
           _mm256_storeu_si256(reinterpret_cast<__m256i *>(write + 0), decoded_values);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(write + 4), decoded_values_1);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(write + 8), decoded_values_2);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(write + 12), decoded_values_3);

            // Move the write pointer forward by simd_width (16 elements)
            write += simd_width;
        }

        // Process any remaining values
        for (; i < packing.count; i++) {
            if (write >= write_end) {
              return Simple8bStatus::kInsufficientSpace;
            }
            if (*read && (*read >> (packing.bitwidth * (i + 1) - 1) & 0x1)) {
              // negative
              *write = (DestT)((*read >> (packing.bitwidth * i)) |(0xFFFFFFFFFFFFFFFF ^((((uint64_t)1) << packing.bitwidth) - 1)));
            } else {
              // positive
              *write = (DestT)((*read >> (packing.bitwidth * i)) &((((uint64_t)1) << packing.bitwidth) - 1));
            }
            write++;
        }

        read++;
    }

    return Simple8bStatus::kOk;
}

} // namespace oned

#endif // __ONED_SIMPLE8B_HPP__

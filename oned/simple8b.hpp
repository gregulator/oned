// Copyright (c) 2024 The OneD Authors
// See LICENSE file.
#ifndef __ONED_SIMPLE8B_HPP__
#define __ONED_SIMPLE8B_HPP__

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>

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

} // namespace oned

#endif // __ONED_SIMPLE8B_HPP__

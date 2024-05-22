// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
#ifndef __ONED_RUNLENGTH_HPP__
#define __ONED_RUNLENGTH_HPP__

#include <cstddef>
#include <cstdint>

// This file defines functions for run-length-encoding (and decoding) of byte
// sequences:
//
// - `ComputeRunlengthEncodeSize` and `ComputeRunlengthDecodeSize` functions
//   for determining the size (in bytes) of the output buffer, prior to encoding
//   or decoding.
// - `RunlengthEncode` and `RunlengthDecode` functions for encoding/decoding
//   byte sequences.
//
// As an example, the byte sequence:
//
//    {'A', 'A', 'A', 'A', 'B', 'B', 'B' };
//
// When encoded becomes:
//
//    {4, 'A', 3, 'B'}
//
// Example:
//    std::vector<uint8_t> orig({'A', 'A', 'A', 'A', 'B', 'B', 'B'});
//    std::vector<uint8_t> encoded;
//    encoded.resize(oned::ComputeRunlengthEncodeSize(orig.data(),
//    orig.size())); oned::RunlengthStatus status =
//    oned::RunlengthEncode(orig.data(),
//          orig.size(), encoded.data(), encoded.size());
//    if (status != oned::RunlengthStatus::kOk) {
//      std::cout << "error code " << (int)status << std::endl;
//      return;
//    }
//    for (int i = 0; i < encoded.size(); i+=2) {
//      std::cout << (int)encoded[i] << " '" << encoded[i+1] << "' ";
//    }
//    std::cout << std::endl;
//    // OUTPUT:
//    // 4 'A' 3 'B'
//
//
// Decoding works similarly:
//
//    std::vector<uint8_t> decoded;
//    decoded.resize(oned::ComputeRunlengthDecodeSize(encoded.data(),
//    encoded.size())); oned::RunlengthStatus status =
//    oned::RunlengthDecode(orig.data(),
//          orig.size(), encoded.data(), encoded.size());
//
// Note that sequences with no repeated bytes will double in size when encoded.
namespace oned {

// Possible return statuses of run-length encoding routines.
enum class RunlengthStatus {
  // Success.
  kOk = 0,
  // Operation failed because the output buffer is too small to hold the
  // results.
  kInsufficientSpace = 1,
  // The encoded sequence is malformed because it contains a run length of 0.
  kInvalidRunLength = 2,
  // The encoded sequence is malformed because it does not end with a run
  // value.
  kMissingRunValue = 3,
};

// Determine the size of the output buffer needed to run-length encode
// `source`.
[[nodiscard]] static inline size_t
ComputeRunlengthEncodeSize(uint8_t *source, size_t source_size) {
  size_t i = 0;
  size_t out_size = 0;
  while (i < source_size) {
    uint8_t run_value = source[i];
    uint8_t run_length = 1;
    size_t j = i + 1;
    while (j < source_size && source[j] == run_value && run_length < 255) {
      run_length++;
      j++;
    }
    i += run_length;
    out_size += 2;
  }
  return out_size;
}

// Run-length encodes `source` and write the result to `dest`.
// Undefined behavior results if `source` and `dest` overlap.
// Returns RunlengthStatus::kOk on success.
// Returns RunlengthStatus::kInsufficientSpace if `dest_size` is not large
// enough to hold the results.
// In the event of an error, `dest` may be partially written.
[[nodiscard]] static inline RunlengthStatus RunlengthEncode(uint8_t *source,
                                                            size_t source_size,
                                                            uint8_t *dest,
                                                            size_t dest_size) {
  size_t i = 0;
  size_t out_index = 0;
  while (i < source_size) {
    uint8_t run_value = source[i];
    uint8_t run_length = 1;
    size_t j = i + 1;
    while (j < source_size && source[j] == run_value && run_length < 255) {
      run_length++;
      j++;
    }
    i += run_length;
    // Output run_length.
    dest[out_index] = run_length;
    if (++out_index >= dest_size) {
      return RunlengthStatus::kInsufficientSpace;
    }
    // Output run_value.
    dest[out_index] = run_value;
    if (++out_index > dest_size) {
      return RunlengthStatus::kInsufficientSpace;
    }
  }
  return RunlengthStatus::kOk;
}

// Determine the size of the output buffer needed to run-length decode
// `source`.
[[nodiscard]] static inline size_t
ComputeRunlengthDecodeSize(uint8_t *source, size_t source_size) {
  size_t i = 0;
  size_t out_size = 0;
  while (i < source_size - 1) {
    uint8_t run_length = source[i];
    int j = 0;
    while (j < run_length) {
      out_size++;
      j++;
    }
    i += 2;
  }
  return out_size;
}

// Run-length decodes `source` and write the result to `dest`.
// Undefined behavior results if `source` and `dest` overlap.
// Returns RunlengthStatus::kOk on success.
// Returns RunlengthStatus::kInsufficientSpace if `dest_size` is not large
// enough to hold the results.  Returns RunlengthStatus::kInvalidRunLength or
// RunlengthStatus::kMissingRunValue if the input sequence is malformed.
// In the event of an error, `dest` may be partially written.
[[nodiscard]] static inline RunlengthStatus RunlengthDecode(uint8_t *source,
                                                            size_t source_size,
                                                            uint8_t *dest,
                                                            size_t dest_size) {
  size_t i = 0;
  size_t out_index = 0;

  while (i < source_size - 1) {
    uint8_t run_length = source[i];
    if (run_length == 0) {
      return RunlengthStatus::kInvalidRunLength;
    }
    uint8_t run_value = source[i + 1];
    int j = 0;
    if (out_index + run_length > dest_size) {
      return RunlengthStatus::kInsufficientSpace;
    }
    while (out_index < dest_size && j < run_length) {
      dest[out_index] = run_value;
      out_index++;
      j++;
    }
    i += 2;
  }
  if (i != source_size) {
    return RunlengthStatus::kMissingRunValue;
  }
  return RunlengthStatus::kOk;
}

} // namespace oned

#endif // __ONED_RUNLENGTH_HPP__

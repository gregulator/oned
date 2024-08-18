// Copyright (c) 2024 The OneD Authors
// See LICENSE file.
#ifndef __ONED_XOR_HPP__
#define __ONED_XOR_HPP__

#include "oned/stripe.hpp"
#include <immintrin.h>
// This file defines functions for xor-encoding (and decoding) of integer
// sequences.
//
// - `XorEncode` and `XorDecode` functions for encoding/decoding sequences of
//   integers.
// - `GenericXorEncode` and `GenericXorDecode` functions for
//   encoding/decoding arbitrary types.
//
// For an original sequence of values:
//
//    x0, x1, x2, x3, x4
//
// The encoded sequence will be:
//
//    x0, x1^x0, x2^x1, x3^x2, x4^x3
//
// Example:
//
//    std::vector<int> orig = {0xFF, 0xF7, 0xF3, 0xF1, 0xF0};
//    std::vector<int> encoded;
//    encoded.resize(orig.size());
//    oned::XorEncode(orig.data(), encoded.data(), orig.size());
//    for (int value : encoded) {
//      std::cout << std::hex << value << " ";
//    }
//    std::cout << std::dec << std::endl;
//    // OUTPUT:
//    // 0xFF 0x8 0x4 0x2 0x1
//
// Decoding works similarly:
//
//    std::vector<int> decoded;
//    decoded.resize(encoded.size());
//    oned::XorDecode(encoded.data(), decoded.data(), encoded.size());
//    for (int value : decoded) {
//      std::cout << std::hex << value << " ";
//    }
//    std::cout << std::dec << std::endl;
//    // OUTPUT:
//    // 0xFF 0xF7 0xF3 0xF1 0xF0
//
// The source and destinations can be specified with flat arrays or Stripes of
// integer types. To encode floating-point values, reinterpret them as
// appropriately-sized integers:
//
//    oned::XorDecode(oned::Reinterpret<uint64_t>(orig_double_stripe),
//                    oned::Reinterpret<uint64_t>(dest_double_stripe));
//
// Just be aware that platform-specific floating-point representations may
// affect portability.
//
// To use a std::span, first convert it to a Stripe:
//
//    XorEncode(Stripe(source_span), Stripe(dest_span));
//
// Arbitrary data types can be encoded using GenericXorDecode, as long as they
// implement the xor (^) and assignment (=) operators:
//
//     std::vector<std::byte>> values = {...};
//     // Encode in-place.
//     oned::GenericXorEncode(values.data(), &value.data(), sizeof(values));
//     // Decode in-place.
//     oned::GenericXorDecode(values.data(), &value.data(), sizeof(values));
//
namespace oned {

// Generates a xor encoding of the first `num_elements` of `source` and writes
// it to `dest`. `source` and `dest` may be equal, in which case the encoding
// will be in-place. Problems will arise if `source[i]` overlaps in memory with
// `dest[j]` for `i > j`.  The `^` operation with operands of type `SourceT`
// must be supported. Both `SourceT` and the return type of `SourceT ^ SourceT`
// must be assignable to `DestT`.
template <typename SourceT, typename DestT>
void GenericXorEncode1(SourceT source, DestT dest, size_t num_elements) {
  if (num_elements <= 0) {
    return;
  }
  dest[0] = source[0];
  auto prev = source[0];
  for (size_t i = 1; i < num_elements; ++i) {
    auto old = source[i]; // Needed for in-place encoding.
    dest[i] = source[i] ^ prev;
    prev = old;
  }
}
template <typename SourceT, typename DestT>
void GenericXorEncode(SourceT source, DestT dest, size_t num_elements) {
  if (num_elements <= 0) {
    return;
  }
  dest[0] = source[0];
  int prev = source[0];

  size_t i = 1;
  const size_t simd_width = 8;

  for (; i + simd_width <= num_elements; i += simd_width) {
    auto old = source[i+simd_width - 1];
    __m256i curr_values = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&source[i]));
    __m256i prev_values = _mm256_set_epi32(
        source[i + 6], source[i + 5], source[i + 4], source[i + 3], 
        source[i + 2], source[i + 1], source[i], prev
    );
    __m256i xor_result = _mm256_xor_si256(curr_values, prev_values);
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(&dest[i]), xor_result);
    prev = old;
  }
  for (; i < num_elements; ++i) {
    int curr_value = source[i];
    dest[i] = curr_value ^ prev;
    prev = curr_value;
  }
}
// Xor encode a sequence of integral values from a flat array to flat array.
// `source` and `dest` may be equal, in which case the encoded will be
// in-place. Problems will arise if `source[i]` overlaps in memory with
// `dest[j]` for `i > j`.
template <typename T> void XorEncode(T *source, T *dest, size_t num_elements) {
  static_assert(std::is_integral<T>::value,
                "The type T must be an integral type. Use GenericXorEncode for "
                "arbitrary types");
  GenericXorEncode<T *, T *>(source, dest, num_elements);
}

// Xor encode a sequence of integral values from a stripe to a flat array. The
// `source.size()` is used as the number of elements. `source` and `dest` may
// overlap, but problems will arise if `source[i]` overlaps in memory with
// `dest[j]` for `i > j`.
template <typename T> void XorEncode(Stripe<T> source, T *dest) {
  static_assert(std::is_integral<T>::value,
                "The type T must be an integral type. Use GenericXorEncode "
                "for arbitrary types");
  GenericXorEncode<Stripe<T>, T *>(source, dest, source.size());
}

// Xor encode a sequence of integral values from a flat array to a stripe. The
// `dest.size()` is used as the number of elements. `source` and `dest` may
// overlap, but problems will arise if `source[i]` overlaps in memory with
// `dest[j]` for `i > j`.
template <typename T> void XorEncode(T *source, Stripe<T> dest) {
  static_assert(std::is_integral<T>::value,
                "The type T must be an integral type. Use GenericXorEncode "
                "for arbitrary types");
  GenericXorEncode<T *, Stripe<T>>(source, dest, dest.size());
}

// Xor encode a sequence of integral values from a stripe to a stripe. The
// minimum of `source.size()` and `dest.size()` is used as the number of
// elements. `source` and `dest` may overlap, but problems will arise if
// `source[i]` overlaps in memory with `dest[j]` for `i > j`.
template <typename T> void XorEncode(Stripe<T> source, Stripe<T> dest) {
  static_assert(std::is_integral<T>::value,
                "The type T must be an integral type. Use GenericXorEncode "
                "for arbitrary types");
  GenericXorEncode<Stripe<T>, Stripe<T>>(source, dest,
                                         std::min(source.size(), dest.size()));
}

// Decodes a xor-encoded `source` and writes the result to `dest`. `source` and
// `dest` may be equal, in which case the decoding will be in-place. Problems
// will arise if `source[i]` overlaps in memory with `dest[j]` for `i > j`.
// memory. The `^` operation with operands of type `SourceT` and `DestT` must
// be supported.  Both `SourceT` and the type resulting from the xor operation
// must be assignable to `DestT`.
template <typename SourceT, typename DestT>
void GenericXorDecode1(SourceT source, DestT dest, size_t num_elements) {
  if (num_elements <= 0) {
    return;
  }
  dest[0] = source[0];
  for (size_t i = 1; i < num_elements; ++i) {
    dest[i] = source[i] ^ dest[i - 1];
  }
}

template <typename SourceT, typename DestT>
void GenericXorDecode(SourceT source, DestT dest, size_t num_elements) {
    if (num_elements <= 0) {
        return;
    }
    size_t i = 0;
    const size_t simd_width = 8;
    const size_t block_size = 4;
    __m256i prefix_sum = _mm256_setzero_si256();

    while (i + simd_width <= num_elements) {
        __m256i src_chunk = _mm256_loadu_si256((__m256i*)&source[i]);
        __m256i temp = _mm256_xor_si256(src_chunk, _mm256_slli_si256(src_chunk, 4));
        temp = _mm256_xor_si256(temp, _mm256_slli_si256(temp, 8));
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(&dest[i]), temp);  
        i += simd_width;
    }

    __m128i s = _mm_setzero_si128(); // The 256-bit version of this instruction performs this byte shift independently within two 128-bit lanes, which is typical to AVX since we need accumulating
    for (size_t j = 0; j + block_size < num_elements; j += block_size){

        __m128i d = (__m128i) _mm_broadcast_ss((float*) &dest[j + 3]);
        __m128i x = _mm_load_si128((__m128i*) &dest[j]);
        x = _mm_xor_si128(s, x);
        _mm_store_si128((__m128i*) &dest[j], x);
        s = _mm_xor_si128(s, d);
        prefix_sum = _mm256_set1_epi32(_mm_cvtsi128_si32(_mm_shuffle_epi32(s, 0xFF)));
    }

    for (; i < num_elements; ++i) {
        dest[i] = source[i]^ _mm256_extract_epi32(prefix_sum, 0);
        prefix_sum = _mm256_set1_epi32(dest[i]); // Update prefix_sum for the next element
    }
}
// Decodes a sequence of xor-encoded integral values from a flat array to flat
// array. `source` and `dest` may be equal, in which case the encoded will be
// in-place. Problems will arise if `source[i]` overlaps in memory with
// `dest[j]` for `i > j`.
template <typename T> void XorDecode(T *source, T *dest, size_t num_elements) {
  static_assert(std::is_integral<T>::value,
                "The type T must be an integral type. Use GenericXorDecode "
                "for arbitrary types");
  GenericXorDecode<T *, T *>(source, dest, num_elements);
}

// Decodes a sequence of xor-encoded integral values from a stripe to a flat
// array. The `source.size()` is used as the number of elements. `source` and
// `dest` may overlap, but problems will arise if `source[i]` overlaps in
// memory with `dest[j]` for `i > j`.
template <typename T> void XorDecode(Stripe<T> source, T *dest) {
  static_assert(std::is_integral<T>::value,
                "The type T must be an integral type. Use GenericXorDecode "
                "for arbitrary types");
  GenericXorDecode<Stripe<T>, T *>(source, dest, source.size());
}

// Decodes a sequence of xor-encoded integral values from a flat array to a
// stripe. The `dest.size()` is used as the number of elements.  `source` and
// `dest` may overlap, but problems will arise if `source[i]` overlaps in memory
// with `dest[j]` for `i > j`.
template <typename T> void XorDecode(T *source, Stripe<T> dest) {
  static_assert(std::is_integral<T>::value,
                "The type T must be an integral type. Use GenericXorDecode "
                "for arbitrary types");
  GenericXorDecode<T *, Stripe<T>>(source, dest, dest.size());
}

// Decodes a sequence of xor-encoded integral values from a stripe to a stripe.
// The minimum of `source.size()` and `dest.size()` is used as the number of
// elements.  `source` and `dest` may overlap, but problems will arise if
// `source[i]` overlaps in memory with `dest[j]` for `i > j`.
template <typename T> void XorDecode(Stripe<T> source, Stripe<T> dest) {
  static_assert(std::is_integral<T>::value,
                "The type T must be an integral type. Use GenericXorDecode "
                "for arbitrary types");
  GenericXorDecode<Stripe<T>, Stripe<T>>(source, dest,
                                         std::min(source.size(), dest.size()));
}

} // namespace oned

#endif // __ONED_XOR_HPP__

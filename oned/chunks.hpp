// Copyright (c) 2024 The OneD Authors
// See LICENSE file.
#ifndef __ONED_CHUNKS_HPP__
#define __ONED_CHUNKS_HPP__

#include "oned/stripe.hpp"

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>
// This header defines the `Chunks` class which conceptually breaks up a large
// sequence of data into smaller sequential blocks, called "chunks". Each chunk
// is a view represented as a `oned::Stripe<T>`. The underlying data may be a
// contiguous array or a Stripe. Each chunk in a Chunks object is the same
// size, except for the last one, which may be smaller than the others.
//
// Example:
//
//    std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//    for (oned::Stripe<int> chunk : oned::Chunks(v, 3)) {
//      std::cout << "Chunk: ";
//      for (int x : chunk) {
//        std::cout << x << " ";
//      }
//      std::cout << std::endl;
//    }
//    // OUTPUT:
//    // Chunk: 0 1 2
//    // Chunk: 3 4 5
//    // Chunk: 6 7 8
//    // Chunk: 9

namespace oned {

template <typename T> class Chunks {
public:
  // The default-constructed Chunks is empty with 0 chunks and chunk size of 1.
  Chunks() : data_{Stripe<T>()}, chunk_size_(1) {}

  // Default copy & move operations can be used and are lightweight. Keep in
  // mind that the "views" are copied but not the underlying data.
  Chunks(const Chunks &other) = default;
  Chunks &operator=(const Chunks &other) = default;
  Chunks(Chunks &&other) = default;
  Chunks &operator=(Chunks &&other) = default;

  // Construct Chunks from a vector and chunk size.
  Chunks(std::vector<T> &data, size_t chunk_size)
      : data_(data.data(), data.size(), sizeof(T)), chunk_size_(chunk_size) {}

  // Construct Chunks from a span and chunk size.
  Chunks(std::span<T> data, size_t chunk_size)
      : data_(Stripe<T>(data)), chunk_size_(chunk_size) {}

  // Construct Chunks from a Stripe and chunk size.
  Chunks(Stripe<T> data, size_t chunk_size)
      : data_(data), chunk_size_(chunk_size) {}

  // Custom iterator.
  class Iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = ssize_t;
    using value_type = const Stripe<T>;
    using pointer = value_type;
    using reference = value_type;
    Iterator(const Chunks *chunks, ssize_t index)
        : chunks_(chunks), index_(index) {}

    reference operator*() const { return (*chunks_)[index_]; }
    pointer operator->() { return (*chunks_)[index_]; }

    // Prefix increment
    Iterator &operator++() {
      index_++;
      return *this;
    }

    // Postfix increment
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    // Prefix decrement
    Iterator &operator--() {
      index_--;
      return *this;
    }

    // Postfix decrement
    Iterator operator--(int) {
      Iterator tmp = *this;
      --(*this);
      return tmp;
    }

    friend bool operator==(const Iterator &a, const Iterator &b) {
      return (a.chunks_ == b.chunks_) && (a.index_ == b.index_);
    };
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return (a.chunks_ != b.chunks_) || (a.index_ != b.index_);
    };

  private:
    const Chunks *chunks_;
    ssize_t index_;
  };

  using iterator = Iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;

  // Returns an iterator pointing to the first element of this span, or `end()`
  // if the span is empty.
  constexpr iterator begin() const noexcept { return Iterator(this, 0); }

  // Returns an iterator to the element following the last element of the
  // stripe. This element acts as a placeholder; attempting to access it results
  // in undefined behavior.
  constexpr iterator end() const noexcept { return Iterator(this, size()); }

  // Returns a reverse iterator pointing to the last element at the end of this
  // span, or `rend()` if the span is empty.
  constexpr reverse_iterator rbegin() const noexcept {
    return reverse_iterator(end());
  }

  // Returns a reverse iterator pointing just before the first element
  // at the beginning of this span. This pointer acts as a placeholder;
  // attempting to access its element results in undefined behavior.
  constexpr reverse_iterator rend() const noexcept {
    return reverse_iterator(begin());
  }

  class StripeProxy {
  public:
      StripeProxy(const Chunks *chunks, size_t index)
          : chunks_(chunks), index_(index) {}

      auto begin() const { return (*chunks_)[index_].begin(); }
      auto end() const { return (*chunks_)[index_].end(); }
      operator Stripe<T>() const { return (*chunks_)[index_]; }
  private:
      const Chunks *chunks_;
      size_t index_;
  };

  // Access the first chunk. Calling front on an empty Chunks results in
  // undefined behavior.
  constexpr StripeProxy front() const { return StripeProxy(this, 0); }
  // Access the last chunk. Calling back on an empty Chunks results in
  // undefined behavior.
  constexpr StripeProxy back() const { return StripeProxy(this, size() - 1); }


  // Returns a view of the chunk with index `pos` with bounds checking. If pos
  // is not within the valid range, an exception std::out_of_range is thrown.
  constexpr Stripe<T> at(size_t pos) const {
    if (pos >= size()) {
      throw std::out_of_range(
          "chunk access at position " + std::to_string(pos) +
          " out of bounds (size = " + std::to_string(size()) + ")");
    }
    return (*this)[pos];
  }

  // Access the chunk at position `index`, without safety checks.
  constexpr Stripe<T> operator[](size_t index) const {
    if (index == size() - 1) {
      return Stripe<T>(&data_[index * chunk_size_], last_chunk_size(),
                       data_.stride());
    }
    return Stripe<T>(&data_[index * chunk_size_], chunk_size_, data_.stride());
  }

  // Returns the number of chunks in the Chunks object.
  constexpr size_t size() const noexcept {
    return (data_.size() + chunk_size_ - 1) / chunk_size_;
  };

  // Returns the size of each full-sized chunk, as passed to the constructor.
  constexpr size_t chunk_size() const noexcept { return chunk_size_; };

  // Returns the size of the last chunk, which may be smaller than the other
  // chunks.
  constexpr size_t last_chunk_size() const noexcept {
    size_t last_chunk_size = data_.size() % chunk_size_;
    if (last_chunk_size == 0) {
      last_chunk_size = chunk_size_;
    }
    return last_chunk_size;
  };

  // Returns true if size is 0, false otherwise.
  [[nodiscard]] constexpr bool empty() const noexcept { return (size() == 0); }

  // Obtains a Chunks that contains the first `count` chunks of this
  // object. The behavior is undefined if `count > size()`.
  constexpr Chunks<T> first(size_t count) const { return subchunks(0, count); }

  // Obtains a Chunks that contains the last `count` chunks of this
  // object. The behavior is undefined if `count > size()`.
  constexpr Chunks<T> last(size_t count) const {
    return subchunks(size() - count, count);
  }

  // Obtains a Chunks that contains `count` chunks from this object, starting
  // at offset `offset`. The chunk size remains unchanged. The behavior is
  // undefined if `count + offset > size()`.
  constexpr Chunks<T> subchunks(size_t offset, size_t count) const {
    return Chunks<T>(data_.substripe(offset * chunk_size_, count * chunk_size_),chunk_size_);
  }

private:
  // The underlying data sequence.
  Stripe<T> data_;
  // The chunk size (aka number of elements in each chunk).
  size_t chunk_size_;
};

} // namespace oned

#endif // __ONED_CHUNK_HPP__

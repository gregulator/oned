// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
#ifndef __ONED_STRIPE_H__
#define __ONED_STRIPE_H__

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <span>
#include <stdexcept>
#include <string>

// This header defines the concept of a "stripe", which is a non-owning view
// into a sequence of data, similar to a std::span, with the addition of a
// stride between elements. A stripe can be used for skipping over elements in
// an array, or for accessing a particular field in an array of structs.
// Compared to std::mdspan, stripe has a simpler interface as it only handles
// 1-dimensional data.
//
// A stripe is considered to have `size` number of elements at memory locations:
//
//      ((uint8_t*)data) + index*stride
//
// For example, consider extracting the green channel from an array of RGB
// values:
//
//     struct RGB {
//       uint8_t r, g, b;
//     };
//     std::array<RGB, 3> c;  // Array of 3 RGB values.
//
// The data is layed out in memory like this:
//
//     c[0].r c[0].g c[0].b c[1].r c[1].g c[1].b c[2].r c[2].g c[2].g
//
// Using a stripe, we can easiy access just the green channel:
//
//            c[0].g               c[1].g               c[2].g
//
// In code this would like:
//
//     Stripe<uint8_t> stripe(&c[0].g, c.size(), sizeof(RGB));
//
// We can now use the stripe as we would a std::span:
//
//   for (uint8_t g : stripe) {
//     cout << g << std::end;
//   }
//
// This library permits arbitrary stride values, including negative, zero, and
// stride values smaller than the element size. It is the user's responsibility
// to ensure the validity of data accessed using stripes.
//
// If a stride of 0 is used, then the stripe behaves as if it has `size`
// elements, but the memory address of each element is the same.

namespace oned {

template <typename T> class Stripe {
public:
  // The default-constructed Stripe is empty with a stride of 0.
  Stripe() : data_(nullptr), size_(0), stride_(0) {}

  // Default copy & move operations can be used and are lightweight. Keep in
  // mind that the "view" is copied but not the underlying data.
  Stripe(const Stripe &other) = default;
  Stripe &operator=(const Stripe &other) = default;
  Stripe(Stripe &&other) = default;
  Stripe &operator=(Stripe &&other) = default;

  // Construct a Stripe from a data pointer, size and stride.
  //
  // `data` points to the first element of type T.
  // `size` is the number of elements of type T in the Stripe.
  // `stride` is the number of bytes between subsequent elements.
  Stripe(T *data, size_t size, ssize_t stride)
      : data_(data), size_(size), stride_(stride) {}

  // Construct a Stripe from a std::span.
  explicit Stripe(std::span<T> span)
      : data_(span.data()), size_(span.size()), stride_(sizeof(T)) {}

  // Construct a Stripe from a std::span which skips over elements. The result
  // includes only those elements at indices that are multiples of `skip`.
  Stripe(std::span<T> span, size_t skip)
      : data_(span.data()), size_(1 + (span.size() - 1) / skip),
        stride_(skip * sizeof(T)) {}

  // Custom iterator. This iterator keeps track of the current index (rather
  // than pointer) to allow for iteration through stripes with 0-stride.
  class Iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = ssize_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    Iterator() : stripe_(nullptr), index_(0) {}

    Iterator(const Stripe *stripe, ssize_t index)
        : stripe_(stripe), index_(index) {}

    reference operator*() { return (*stripe_)[index_]; }
    pointer operator->() { return &(*stripe_)[index_]; }

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
      return &a.stripe_[a.index_] == &b.stripe_[b._index_];
    };
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return &a.stripe_[a.index_] != &b.stripe_[b.index_];
    };

  private:
    const Stripe *stripe_;
    ssize_t index_;
  };

  using iterator = Iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;

  // Returns an iterator pointing to the first element of this span, or `end()`
  // if the span is empty.
  constexpr iterator begin() noexcept { return Iterator(this, 0); }

  // Returns an iterator to the element following the last element of the
  // stripe. This element acts as a placeholder; attempting to access it results
  // in undefined behavior.
  constexpr iterator end() noexcept { return Iterator(this, size_); }

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

  // Access the first element in the stripe. Calling front on an empty stripe
  // results in undefined behavior.
  constexpr T &front() const { return (*this)[0]; }

  // Access the last element in the stripe. Calling back on an empty stripe
  // results in undefined behavior.
  constexpr T &back() const { return (*this)[size_ - 1]; }

  // Returns a reference to the element at specified position with bounds
  // checking. If pos is not within the range of the stripe, an exception
  // std::out_of_range is thrown.
  constexpr T &at(size_t pos) const {
    if (pos >= size_) {
      throw std::out_of_range(
          "stripe access at position " + std::to_string(pos) +
          " out of bounds (size = " + std::to_string(size_) + ")");
    }
    return *(reinterpret_cast<T *>((reinterpret_cast<uint8_t *>(data_)) +
                                   stride_ * pos));
  }

  // Access the element at position `index` in the stripe, without safety
  // checks.
  constexpr T &operator[](size_t index) const {
    return *(reinterpret_cast<T *>((reinterpret_cast<uint8_t *>(data_)) +
                                   stride_ * index));
  }

  // Returns a pointer to the beginning of the sequence.
  constexpr T *data() const noexcept { return data_; };

  // Returns the number of elements in the stripe.
  constexpr size_t size() const noexcept { return size_; };

  // Returns the number of elements in the stripe.
  constexpr size_t stride() const noexcept { return stride_; };

  // Returns true if size is 0, false otherwise.
  [[nodiscard]] constexpr bool empty() const noexcept { return (size_ == 0); }

  // Obtains a stripe that is a view over the first `count` elements of this
  // span. The behavior is undefined if `count > size()`.
  constexpr Stripe<T> first(size_t count) const { return substripe(0, count); }

  // Obtains a stripe that is a view over the last `count` elements of this
  // span. The behavior is undefined if `count > size()`.
  constexpr Stripe<T> last(size_t count) const {
    return substripe(size_ - count, count);
  }

  // Obtains a stripe that is a view over the Count elements of this stripe
  // starting at offset Offset. The stride remains unchanged. The behavior is
  // undefined if `count + offset > size()`.
  constexpr Stripe<T> substripe(size_t offset, size_t count) const {
    return Stripe<T>(&(*this)[offset], count, stride_);
  }

private:
  T *data_;        // non-owning pointer to underlying data
  size_t size_;    // number of elements
  ssize_t stride_; // number of bytes between subsequent elements
};

// Reinterpret()
//
// Returns a new Stripe that is identical to the original, except for how its
// values are interpreted.  The returned Stripe has the same data pointer, size
// and stride as the original Stripe but the values are interpreted as values
// of type `NewT` instead of type `T`.
template <typename NewT> Stripe<NewT> Reinterpret(auto stripe) {
  return Stripe<NewT>(reinterpret_cast<NewT *>(stripe.data()), stripe.size(),
                      stripe.stride());
}

static_assert(std::input_or_output_iterator<Stripe<int>::iterator>);

} // namespace oned

#endif // __ONED_STRIPE_H__

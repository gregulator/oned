// stripe_test.cpp
#include "oned/stripe.hpp"
#include <gtest/gtest.h>
#include <cstdint>
#include <array>

// Test default constructor
TEST(StripeTestConstructors, DefaultConstructor) {
    // This test verifies that the default constructor initializes
    // the Stripe object with nullptr for data, 0 for size, and 0 for stride.
    oned::Stripe<int> stripe;

    EXPECT_EQ(stripe.data(), nullptr);
    EXPECT_EQ(stripe.size(), 0);
    EXPECT_EQ(stripe.stride(), 0);
}
// Test parameterized constructor
TEST(StripeTestConstructors, ParameterizedConstructor) {
    // This test verifies that the parameterized constructor initializes
    // the Stripe object with the given data pointer, size, and stride.
    int data[5] = {1, 2, 3, 4, 5};
    size_t size = 5;
    ssize_t stride = sizeof(int);

    oned::Stripe<int> stripe(data, size, stride);

    EXPECT_EQ(stripe.data(), data);
    EXPECT_EQ(stripe.size(), size);
    EXPECT_EQ(stripe.stride(), stride);
}
// Test span constructor
TEST(StripeTestConstructors, SpanConstructor) {
    // This test verifies that the span constructor initializes
    // the Stripe object with the data from the given std::span.
    std::array<int, 5> data = {1, 2, 3, 4, 5};
    std::span<int> span(data);

    oned::Stripe<int> stripe(span);

    EXPECT_EQ(stripe.data(), data.data());
    EXPECT_EQ(stripe.size(), data.size());
    EXPECT_EQ(stripe.stride(), sizeof(int));
}


// Test operator[]
TEST(StripeTestOperators, SubscriptOperator) {
    int data[5] = {1, 2, 3, 4, 5};
    oned::Stripe<int> stripe(data, 5, sizeof(int));
    EXPECT_EQ(stripe[0], 1);
    EXPECT_EQ(stripe[1], 2);
    EXPECT_EQ(stripe[2], 3);
    EXPECT_EQ(stripe[3], 4);
    EXPECT_EQ(stripe[4], 5);
}
// Test at() with bounds checking
TEST(StripeTestOperators, AtMethod) {
    int data[5] = {1, 2, 3, 4, 5};
    oned::Stripe<int> stripe(data, 5, sizeof(int));

    EXPECT_EQ(stripe.at(0), 1);
    EXPECT_EQ(stripe.at(1), 2);
    EXPECT_EQ(stripe.at(2), 3);
    EXPECT_EQ(stripe.at(3), 4);
    EXPECT_EQ(stripe.at(4), 5);

    EXPECT_THROW(stripe.at(5), std::out_of_range);
}
// Test front()
TEST(StripeTestOperators, FrontMethod) {
    int data[5] = {1, 2, 3, 4, 5};
    oned::Stripe<int> stripe(data, 5, sizeof(int));

    EXPECT_EQ(stripe.front(), 1);
}
// Test back()
TEST(StripeTestOperators, BackMethod) {
    int data[5] = {1, 2, 3, 4, 5};
    oned::Stripe<int> stripe(data, 5, sizeof(int));

    EXPECT_EQ(stripe.back(), 5);
}

// Test iteration
TEST(StripeTestIteration, Iterator) {
    int data[5] = {1, 2, 3, 4, 5};
    //test with const to ensure we allow it
    const oned::Stripe<int> stripe(data, 5, sizeof(int));

    std::vector<int> result;
    for (int value : stripe) {
        result.push_back(value);
    }

    EXPECT_EQ(result, std::vector<int>({1, 2, 3, 4, 5}));
}
// Test reverse iteration
TEST(StripeTestIteration, ReverseIterator) {
    int data[5] = {1, 2, 3, 4, 5};
    const oned::Stripe<int> stripe(data, 5, sizeof(int));

    std::vector<int> result;
    for (auto it = stripe.rbegin(); it != stripe.rend(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, std::vector<int>({5, 4, 3, 2, 1}));
}

// Test empty()
TEST(StripeTestMethod, EmptyMethod) {
    oned::Stripe<int> stripe;
    EXPECT_TRUE(stripe.empty());

    int data[5] = {1, 2, 3, 4, 5};
    oned::Stripe<int> non_empty_stripe(data, 5, sizeof(int));
    EXPECT_FALSE(non_empty_stripe.empty());
}
// Test substripe()
TEST(StripeTestMethod, SubstripeMethod) {
    int data[5] = {1, 2, 3, 4, 5};
    oned::Stripe<int> stripe(data, 5, sizeof(int));

    oned::Stripe<int> sub = stripe.substripe(1, 3);
    EXPECT_EQ(sub.size(), 3);
    EXPECT_EQ(sub[0], 2);
    EXPECT_EQ(sub[1], 3);
    EXPECT_EQ(sub[2], 4);
}

// Test first method
TEST(StripeTestMethod, First) {
    // This test verifies that the first method returns a stripe
    // that is a view over the first `count` elements of the original stripe.
    int data[5] = {1, 2, 3, 4, 5};
    oned::Stripe<int> stripe(data, 5, sizeof(int));
    auto firstStripe = stripe.first(3);

    std::vector<int> result;
    for (auto it = firstStripe.begin(); it != firstStripe.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, std::vector<int>({1,2,3}));

    EXPECT_EQ(firstStripe.size(), 3);
    EXPECT_EQ(firstStripe.stride(), sizeof(int));
    EXPECT_EQ(firstStripe[0], 1);
    EXPECT_EQ(firstStripe[1], 2);
    EXPECT_EQ(firstStripe[2], 3);
    EXPECT_THROW(firstStripe.at(3), std::out_of_range);
}
// Test last method
TEST(StripeTestMethod, Last) {
    // This test verifies that the last method returns a stripe
    // that is a view over the last `count` elements of the original stripe.
    int data[5] = {1, 2, 3, 4, 5};
    oned::Stripe<int> stripe(data, 5, sizeof(int));

    auto lastStripe = stripe.last(3);

    EXPECT_EQ(lastStripe.size(), 3);
    EXPECT_EQ(lastStripe.stride(), sizeof(int));
    EXPECT_EQ(lastStripe[0], 3);
    EXPECT_EQ(lastStripe[1], 4);
    EXPECT_EQ(lastStripe[2], 5);

    // Verify out-of-bounds access throws an exception
    EXPECT_THROW(lastStripe.at(3), std::out_of_range);
}


// Test reinterpret
TEST(StripeTestReinterpret, Reinterpret) {
    int32_t data[4] = {0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10};
    oned::Stripe<int32_t> stripe(data, 4, sizeof(int32_t));

    auto byte_stripe = oned::Reinterpret<uint8_t>(stripe);
    EXPECT_EQ(byte_stripe.size(), 4);
    EXPECT_EQ(byte_stripe.stride(), sizeof(int32_t));
    EXPECT_EQ(byte_stripe[0], 0x04);
    EXPECT_EQ(byte_stripe[1], 0x08);
    EXPECT_EQ(byte_stripe[2], 0x0C);
    EXPECT_EQ(byte_stripe[3], 0x10);
}
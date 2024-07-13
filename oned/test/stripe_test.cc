// stripe_test.cpp
#include "oned/stripe.hpp"
#include <gtest/gtest.h>
#include <cstdint>
#include <array>

// Test default constructor
TEST(StripeTest, DefaultConstructor) {
    // This test verifies that the default constructor initializes
    // the Stripe object with nullptr for data, 0 for size, and 0 for stride.
    oned::Stripe<int> stripe;

    EXPECT_EQ(stripe.data(), nullptr);
    EXPECT_EQ(stripe.size(), 0);
    EXPECT_EQ(stripe.stride(), 0);
}

// Test parameterized constructor
TEST(StripeTest, ParameterizedConstructor) {
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
TEST(StripeTest, SpanConstructor) {
    // This test verifies that the span constructor initializes
    // the Stripe object with the data from the given std::span.
    std::array<int, 5> data = {1, 2, 3, 4, 5};
    std::span<int> span(data);

    oned::Stripe<int> stripe(span);

    EXPECT_EQ(stripe.data(), data.data());
    EXPECT_EQ(stripe.size(), data.size());
    EXPECT_EQ(stripe.stride(), sizeof(int));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

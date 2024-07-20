#include <gtest/gtest.h>
#include "oned/xor.hpp"

// Helper function to compare two vectors
template <typename T>
bool vectorsEqual(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

TEST(XorEncodeDecodeTest, BasicFunctionality) {
    std::vector<int> orig = {0xFF, 0xF7, 0xF3, 0xF1, 0xF0};
    std::vector<int> encoded(orig.size());
    std::vector<int> decoded(orig.size());

    oned::XorEncode(orig.data(), encoded.data(), orig.size());
    oned::XorDecode(encoded.data(), decoded.data(), encoded.size());

    EXPECT_TRUE(vectorsEqual(orig, decoded));
}

// Test XorEncode and XorDecode with a Stripe of integers
TEST(XorTest, XorEncodeDecodeStripe) {
    std::vector<int> original = {0xFF, 0xF7, 0xF3, 0xF1, 0xF0};

    oned::Stripe<int> originalStripe(original.data(), original.size(), sizeof(int));
    oned::Stripe<int> encodedStripe(original.data(), original.size(), sizeof(int));

    oned::XorEncode(originalStripe, encodedStripe);
    oned::XorDecode(encodedStripe, originalStripe);
    ASSERT_EQ(original.data(), encodedStripe.data());
}
TEST(XorEncodeDecodeTest, EmptyArray) {
    std::vector<int> empty;
    std::vector<int> encoded;
    std::vector<int> decoded;

    oned::XorEncode(empty.data(), encoded.data(), empty.size());
    oned::XorDecode(empty.data(), decoded.data(), empty.size());

    EXPECT_TRUE(empty.empty());
}

TEST(XorEncodeDecodeTest, EdgeCases) {
    std::vector<int> singleElement = {0xFF};
    std::vector<int> encoded(singleElement.size());
    std::vector<int> decoded(singleElement.size());

    oned::XorEncode(singleElement.data(), encoded.data(), singleElement.size());
    oned::XorDecode(encoded.data(), decoded.data(), encoded.size());

    EXPECT_TRUE(vectorsEqual(singleElement, decoded));

    std::vector<int> twoElements = {0xFF, 0x00};
    std::vector<int> encodedTwo(twoElements.size());
    std::vector<int> decodedTwo(twoElements.size());

    oned::XorEncode(twoElements.data(), encodedTwo.data(), twoElements.size());
    oned::XorDecode(encodedTwo.data(), decodedTwo.data(), encodedTwo.size());

    EXPECT_TRUE(vectorsEqual(twoElements, decodedTwo));
}

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "oned/delta.hpp"
#include "oned/stripe.hpp"
// Test for DeltaEncode with int vector
TEST(DeltaTest, DeltaEncodeIntVector) {
    std::vector<int> orig = {2, 5, 4, 10, 9};
    std::vector<int> encoded(orig.size());
    auto result = oned::DeltaEncode(orig, encoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(encoded, (std::vector<int>{2, 3, -1, 6, -1}));
}

// Test for DeltaDecode with int vector
TEST(DeltaTest, DeltaDecodeIntVector) {
    std::vector<int> encoded = {2, 3, -1, 6, -1};
    std::vector<int> decoded(encoded.size());
    auto result = oned::DeltaDecode(encoded, decoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(decoded, (std::vector<int>{2, 5, 4, 10, 9}));
}

// Test for size mismatch in DeltaEncode
TEST(DeltaTest, DeltaEncodeSizeMismatch) {
    std::vector<int> orig = {1, 2, 3};
    std::vector<int> encoded(2);  // Different size

    auto result = oned::DeltaEncode(orig, encoded);
    EXPECT_EQ(result, oned::DeltaResult::kSizeMismatch);
}

// Test for size mismatch in DeltaDecode
TEST(DeltaTest, DeltaDecodeSizeMismatch) {
    std::vector<int> encoded = {1, 2, 3};
    std::vector<int> decoded(2);  // Different size

    auto result = oned::DeltaDecode(encoded, decoded);
    EXPECT_EQ(result, oned::DeltaResult::kSizeMismatch);
}

// Test for in-place DeltaEncode
TEST(DeltaTest, InPlaceDeltaEncode) {
    std::vector<int> data = {2, 5, 4, 10, 9};

    auto result = oned::DeltaEncode(data, data);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(data, (std::vector<int>{2, 3, -1, 6, -1}));
}

// Test for in-place DeltaDecode
TEST(DeltaTest, InPlaceDeltaDecode) {
    std::vector<int> data = {2, 3, -1, 6, -1};

    auto result = oned::DeltaDecode(data, data);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(data, (std::vector<int>{2, 5, 4, 10, 9}));
}

struct RGB {
  uint8_t r, g, b; // red, green, blue
};

TEST(DeltaEncodingTest, EncodeDecodeRGB) {
  std::array<RGB, 5> c = {
      RGB{0x32, 0x91, 0xA8}, // Teal
      RGB{0x32, 0x91, 0xA8}, RGB{0x32, 0x91, 0xA8},
      RGB{0x32, 0x91, 0xA8}, RGB{0x32, 0x91, 0xA8},
  };
  oned::Stripe<uint8_t> red_channel(&c[0].r, c.size(), sizeof(RGB));
  oned::Stripe<uint8_t> green_channel(&c[0].g, c.size(), sizeof(RGB));
  oned::Stripe<uint8_t> blue_channel(&c[0].b, c.size(), sizeof(RGB));
  std::vector<uint8_t> encoded(red_channel.size());

  EXPECT_EQ(oned::DeltaEncode(red_channel, encoded), oned::DeltaResult::kOk);
  EXPECT_EQ(encoded, (std::vector<uint8_t>{0x32, 0, 0, 0, 0}));

  EXPECT_EQ(oned::DeltaEncode(green_channel, encoded), oned::DeltaResult::kOk);
  EXPECT_EQ(encoded, (std::vector<uint8_t>{0x91, 0, 0, 0, 0}));

  EXPECT_EQ(oned::DeltaEncode(blue_channel, encoded), oned::DeltaResult::kOk);
  EXPECT_EQ(encoded, (std::vector<uint8_t>{0xA8, 0, 0, 0, 0}));
}
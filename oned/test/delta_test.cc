#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "oned/delta.hpp"
#include "oned/stripe.hpp"
// Test for DeltaEncode with int vector
TEST(DeltaTest, DeltaEncodeIntVectorNormal) {
    std::vector<int> orig = {2, 5, 4, 10, 9};
    std::vector<int> encoded(orig.size());
    auto result = oned::DeltaEncode(orig, encoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(encoded, (std::vector<int>{2, 3, -1, 6, -1}));
}

TEST(DeltaTest, DeltaEncodeSIMDIntVectorNormal) {
    std::vector<int> orig = {2, 5, 4, 10, 9};
    std::vector<int> encoded(orig.size());
    auto result = oned::DeltaEncodeSIMD(orig, encoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(encoded, (std::vector<int>{2, 3, -1, 6, -1}));
}
//All test cases with data length > 8
TEST(DeltaTest, DeltaEncodeIntVector) {
    std::vector<int> orig = {2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3};
    std::vector<int> encoded(orig.size());
    auto result = oned::DeltaEncode(orig, encoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(encoded, (std::vector<int>{2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2}));
}

TEST(DeltaTest, DeltaEncodeSIMDIntVector) {
    std::vector<int> orig = {2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3};
    std::vector<int> encoded(orig.size());
    auto result = oned::DeltaEncodeSIMD(orig, encoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(encoded, (std::vector<int>{2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2}));
}

// Test for DeltaDecode with int vector
TEST(DeltaTest, DeltaDecodeIntVector) {
    std::vector<int> encoded = {2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2};
    std::vector<int> decoded(encoded.size());
    auto result = oned::DeltaDecode(encoded, decoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(decoded, (std::vector<int>{2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3}));
}

TEST(DeltaTest, DeltaDecodeSIMDIntVector) {
    std::vector<int> encoded = {2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2};
    std::vector<int> decoded(encoded.size());
    auto result = oned::DeltaDecodeSIMD(encoded, decoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(decoded, (std::vector<int>{2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3}));
}

// Test for size mismatch in DeltaEncode
TEST(DeltaTest, DeltaEncodeSizeMismatch) {
    std::vector<int> orig =  {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> encoded(8);  // Different size

    auto result = oned::DeltaEncode(orig, encoded);
    EXPECT_EQ(result, oned::DeltaResult::kSizeMismatch);
}

TEST(DeltaTest, DeltaEncodeSIMDSizeMismatch) {
    std::vector<int> orig =  {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> encoded(8);  // Different size

    auto result = oned::DeltaEncodeSIMD(orig, encoded);
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
    std::vector<int> data = {2, 5, 4, 10, 9,13,17,16, 32,5,3};

    auto result = oned::DeltaEncode(data, data);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(data, (std::vector<int>{2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2}));
}
// Test for in-place DeltaEncode
TEST(DeltaTest, InPlaceDeltaEncodeSIMD) {
    std::vector<int> data = {2, 5, 4, 10, 9,13,17,16, 32,5,3};

    auto result = oned::DeltaEncodeSIMD(data, data);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(data, (std::vector<int>{2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2}));
}
// Test for in-place DeltaDecode
TEST(DeltaTest, InPlaceDeltaDecode) {
    std::vector<int> data = {2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2};

    auto result = oned::DeltaDecode(data, data);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(data, (std::vector<int>{2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3}));
}

TEST(DeltaTest, InPlaceDeltaDecodeSIMD) {
    std::vector<int> data = {2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2};

    auto result = oned::DeltaDecodeSIMD(data, data);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(data, (std::vector<int>{2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3}));
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

TEST(DeltaEncodingTest, EncodeDecodeSIMDRGB) {
  std::array<RGB, 5> c = {
      RGB{0x32, 0x91, 0xA8}, // Teal
      RGB{0x32, 0x91, 0xA8}, RGB{0x32, 0x91, 0xA8},
      RGB{0x32, 0x91, 0xA8}, RGB{0x32, 0x91, 0xA8},
  };
  oned::Stripe<uint8_t> red_channel(&c[0].r, c.size(), sizeof(RGB));
  oned::Stripe<uint8_t> green_channel(&c[0].g, c.size(), sizeof(RGB));
  oned::Stripe<uint8_t> blue_channel(&c[0].b, c.size(), sizeof(RGB));
  std::vector<uint8_t> encoded(red_channel.size());

  EXPECT_EQ(oned::DeltaEncodeSIMD(red_channel, encoded), oned::DeltaResult::kOk);
  EXPECT_EQ(encoded, (std::vector<uint8_t>{0x32, 0, 0, 0, 0}));

  EXPECT_EQ(oned::DeltaEncodeSIMD(green_channel, encoded), oned::DeltaResult::kOk);
  EXPECT_EQ(encoded, (std::vector<uint8_t>{0x91, 0, 0, 0, 0}));

  EXPECT_EQ(oned::DeltaEncodeSIMD(blue_channel, encoded), oned::DeltaResult::kOk);
  EXPECT_EQ(encoded, (std::vector<uint8_t>{0xA8, 0, 0, 0, 0}));
}


// Test for DeltaEncode with int vector (length > 24)
TEST(DeltaTest, DeltaEncodeIntVectorLarge) {
    std::vector<int> orig = {2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3, 8, 7, 11, 15, 20, 30, 50, 75, 100, 150, 200, 250, 300, 350, 400,401};
    std::vector<int> encoded(orig.size());
    auto result = oned::DeltaEncode(orig, encoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(encoded, (std::vector<int>{2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2, 5, -1, 4, 4, 5, 10, 20, 25, 25, 50, 50, 50, 50, 50, 50,1}));
}

TEST(DeltaTest, DeltaEncodeSIMDIntVectorLarge) {
    std::vector<int> orig = {2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3, 8, 7, 11, 15, 20, 30, 50, 75, 100, 150, 200, 250, 300, 350, 400};
    std::vector<int> encoded(orig.size());
    auto result = oned::DeltaEncodeSIMD(orig, encoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(encoded, (std::vector<int>{2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2, 5, -1, 4, 4, 5, 10, 20, 25, 25, 50, 50, 50, 50, 50, 50}));
}

// Test for DeltaDecode with int vector (length > 24)
TEST(DeltaTest, DeltaDecodeIntVectorLarge) {
    std::vector<int> encoded = {2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2, 5, -1, 4, 4, 5, 10, 20, 25, 25, 50, 50, 50, 50, 50, 50};
    std::vector<int> decoded(encoded.size());
    auto result = oned::DeltaDecode(encoded, decoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(decoded, (std::vector<int>{2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3, 8, 7, 11, 15, 20, 30, 50, 75, 100, 150, 200, 250, 300, 350, 400}));
}

TEST(DeltaTest, DeltaDecodeSIMDIntVectorLarge) {
    std::vector<int> encoded = {2, 3, -1, 6, -1, 4, 4, -1, 16, -27, -2, 5, -1, 4, 4, 5, 10, 20, 25, 25, 50, 50, 50, 50, 50, 50};
    std::vector<int> decoded(encoded.size());
    auto result = oned::DeltaDecodeSIMD(encoded, decoded);
    EXPECT_EQ(result, oned::DeltaResult::kOk);
    EXPECT_EQ(decoded, (std::vector<int>{2, 5, 4, 10, 9, 13, 17, 16, 32, 5, 3, 8, 7, 11, 15, 20, 30, 50, 75, 100, 150, 200, 250, 300, 350, 400}));
}
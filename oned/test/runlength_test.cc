#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include "oned/runlength.hpp"

TEST(RunlengthTest, EncodeDecode) {
    std::vector<uint8_t> orig = {'A', 'A', 'A', 'A', 'B', 'B', 'B'};

    // Encode
    std::vector<uint8_t> encoded(oned::ComputeRunlengthEncodeSize(orig.data(), orig.size()));
    oned::RunlengthStatus encodeStatus = oned::RunlengthEncode(orig.data(), orig.size(), encoded.data(), encoded.size());
    EXPECT_EQ(encodeStatus, oned::RunlengthStatus::kOk);

    // Decode 
    std::vector<uint8_t> decoded(oned::ComputeRunlengthDecodeSize(encoded.data(), encoded.size()));
    oned::RunlengthStatus decodeStatus = oned::RunlengthDecode(encoded.data(), encoded.size(), decoded.data(), decoded.size());
    EXPECT_EQ(decodeStatus, oned::RunlengthStatus::kOk);

    // Verify decoded data matches original data
    EXPECT_EQ(orig, decoded);
}

TEST(RunlengthTest, EncodeInsufficientSpace) {
    std::vector<uint8_t> orig = {'A', 'A', 'A', 'A', 'B', 'B', 'B'};
    size_t encoded_size = oned::ComputeRunlengthEncodeSize(orig.data(), orig.size());
    std::vector<uint8_t> encoded(encoded_size - 1); // Intentionally insufficient space

    // Attempt to encode with insufficient space
    oned::RunlengthStatus status = oned::RunlengthEncode(orig.data(), orig.size(), encoded.data(), encoded.size());
    EXPECT_EQ(status, oned::RunlengthStatus::kInsufficientSpace);
}

TEST(RunlengthTest, DecodeInsufficientSpace) {
    std::vector<uint8_t> orig = {'A', 'A', 'A', 'A', 'B', 'B', 'B'};

    // Encode data
    std::vector<uint8_t> encoded(oned::ComputeRunlengthEncodeSize(orig.data(), orig.size()));
    oned::RunlengthEncode(orig.data(), orig.size(), encoded.data(), encoded.size());

    // Decode with insufficient space
    std::vector<uint8_t> decoded(oned::ComputeRunlengthDecodeSize(encoded.data(), encoded.size()));
    std::vector<uint8_t> insufficient_space(decoded.size() - 1);
    oned::RunlengthStatus status = oned::RunlengthDecode(encoded.data(), encoded.size(), insufficient_space.data(), insufficient_space.size());
    EXPECT_EQ(status, oned::RunlengthStatus::kInsufficientSpace);
}

TEST(RunlengthTest, DecodeInvalidRunLength) {
    std::vector<uint8_t> invalid_encoded = {0, 'A', 3, 'B'}; // Invalid because of run length 0
    std::vector<uint8_t> decoded(4);

    // Attempt to decode with invalid run length
    oned::RunlengthStatus status = oned::RunlengthDecode(invalid_encoded.data(), invalid_encoded.size(), decoded.data(), decoded.size());
    EXPECT_EQ(status, oned::RunlengthStatus::kInvalidRunLength);
}

TEST(RunlengthTest, DecodeMissingRunValue) {
    std::vector<uint8_t> invalid_encoded = {3, 'A', 2}; // Missing run value
    std::vector<uint8_t> decoded(5);

    // Attempt to decode with missing run value
    oned::RunlengthStatus status = oned::RunlengthDecode(invalid_encoded.data(), invalid_encoded.size(), decoded.data(), decoded.size());
    EXPECT_EQ(status, oned::RunlengthStatus::kMissingRunValue);
}

TEST(RunlengthTest, ComputeEncodeSizeEdgeCases) {
    std::vector<uint8_t> single_element = {'A'};
    std::vector<uint8_t> two_elements = {'A', 'B'};

    EXPECT_EQ(oned::ComputeRunlengthEncodeSize(single_element.data(), single_element.size()), 2);
    EXPECT_EQ(oned::ComputeRunlengthEncodeSize(two_elements.data(), two_elements.size()), 4);
}

TEST(RunlengthTest, ComputeDecodeSizeEdgeCases) {
    std::vector<uint8_t> encoded = {4, 'A', 3, 'B'};
    EXPECT_EQ(oned::ComputeRunlengthDecodeSize(encoded.data(), encoded.size()), 7);
}


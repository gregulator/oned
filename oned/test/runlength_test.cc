#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include "oned/runlength.hpp"
#include <random>

std::vector<uint8_t> GenerateRandomBytes(size_t size) {
    std::vector<uint8_t> data(size);
    std::default_random_engine gen;
    std::uniform_int_distribution<> dis(0, 255);
    
    size_t i = 0;
    while (i < size) {
        // Generate a random value
        uint8_t value = static_cast<uint8_t>(dis(gen));
        
        // Generate a run of 20 to 40 consecutive identical values
        size_t run_length = std::min<size_t>(size - i, 20 + (dis(gen) % 21));
        std::fill_n(data.begin() + i, run_length, value);
        i += run_length;

        // Optionally, generate a few random non-repeated values in between the runs
        size_t random_length = std::min<size_t>(size - i, dis(gen) % 10);
        for (size_t j = 0; j < random_length; ++j) {
            data[i++] = static_cast<uint8_t>(dis(gen));
        }
    }

    return data;
}

TEST(RunlengthTest, EncodeDecode) {
    std::vector<uint8_t> orig = {
        'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
        'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
        'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C',
        'D', 'D', 'D', 'D'
    };

    // Encode
    std::vector<uint8_t> encoded(oned::ComputeRunlengthEncodeSize(orig.data(), orig.size()));
    oned::RunlengthStatus encodeStatus = oned::RunlengthEncodeSIMD(orig.data(), orig.size(), encoded.data(), encoded.size());
    EXPECT_EQ(encodeStatus, oned::RunlengthStatus::kOk);

    // Decode 
    std::vector<uint8_t> decoded(oned::ComputeRunlengthDecodeSize(encoded.data(), encoded.size()));
    oned::RunlengthStatus decodeStatus = oned::RunlengthDecodeSIMD(encoded.data(), encoded.size(), decoded.data(), decoded.size());
    EXPECT_EQ(decodeStatus, oned::RunlengthStatus::kOk);

    // Verify decoded data matches original data
    //EXPECT_EQ(encoded,decoded);
    EXPECT_EQ(orig, decoded);
}

TEST(RunlengthTest, EncodeInsufficientSpace) {
    std::vector<uint8_t> orig = {'A', 'A', 'A', 'A', 'B', 'B', 'B','C','C'};
    size_t encoded_size = oned::ComputeRunlengthEncodeSize(orig.data(), orig.size());
    std::vector<uint8_t> encoded(encoded_size - 1); // Intentionally insufficient space

    // Attempt to encode with insufficient space
    oned::RunlengthStatus status = oned::RunlengthEncode(orig.data(), orig.size(), encoded.data(), encoded.size());
    EXPECT_EQ(status, oned::RunlengthStatus::kInsufficientSpace);
}

TEST(RunlengthTest, DecodeInsufficientSpace) {
    std::vector<uint8_t> orig = {'A', 'A', 'A', 'A', 'B', 'B', 'B','C','C'};

    // Encode data
    std::vector<uint8_t> encoded(oned::ComputeRunlengthEncodeSize(orig.data(), orig.size()));
    oned::RunlengthStatus temp = oned::RunlengthEncode(orig.data(), orig.size(), encoded.data(), encoded.size());

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
    std::vector<uint8_t> encoded = {4, 'A', 3, 'B',2,'C'};
    EXPECT_EQ(oned::ComputeRunlengthDecodeSize(encoded.data(), encoded.size()), 9);
}


// Parametrized Test Fixture
class RunlengthTest : public ::testing::TestWithParam<size_t> {};

// Test case that runs for different sizes
TEST_P(RunlengthTest, EncodeDecodeN) {
    size_t size = GetParam();
    
    std::vector<uint8_t> orig = GenerateRandomBytes(size);

    // Encode
    std::vector<uint8_t> encoded(oned::ComputeRunlengthEncodeSize(orig.data(), orig.size()));
    oned::RunlengthStatus encodeStatus = oned::RunlengthEncodeSIMD(orig.data(), orig.size(), encoded.data(), encoded.size());
    EXPECT_EQ(encodeStatus, oned::RunlengthStatus::kOk);

    // Decode
    std::vector<uint8_t> decoded(oned::ComputeRunlengthDecodeSize(encoded.data(), encoded.size()));
    oned::RunlengthStatus decodeStatus = oned::RunlengthDecodeSIMD(encoded.data(), encoded.size(), decoded.data(), decoded.size());
    EXPECT_EQ(decodeStatus, oned::RunlengthStatus::kOk);

    // Verify decoded data matches original data
    EXPECT_EQ(orig, decoded);
}

// Define the test sizes: 128, 256, 512, ..., 128000
INSTANTIATE_TEST_SUITE_P(
    RunlengthSizes,
    RunlengthTest,
    ::testing::Values( 128,256, 512, 1024, 2048, 16384, 32768, 128000)
);
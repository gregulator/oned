#include "oned/simple8b.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <limits>

TEST(Simple8bTest, BasicUnsignedEncodingDecoding) {
    std::vector<uint64_t> data = {1, 2, 3, 4, 5, 6, 7, 8};
    auto encoded_size_opt = oned::ComputeSimple8bEncodeSize(data.data(), data.size());
    ASSERT_TRUE(encoded_size_opt.has_value()) << "Failed to compute encoding size.";
    
    size_t encoded_size = encoded_size_opt.value();
    std::vector<uint64_t> encoded(encoded_size);
    
    oned::Simple8bStatus encode_status = oned::Simple8bEncode(data.data(), data.size(), encoded.data(), encoded.size());
    EXPECT_EQ(encode_status, oned::Simple8bStatus::kOk) << "Encoding failed.";
    
    std::vector<uint64_t> decoded(oned::ComputeSimple8bDecodeSize(encoded.data(), encoded.size()));
    oned::Simple8bStatus decode_status = oned::Simple8bDecode(encoded.data(), encoded.size(), decoded.data(), decoded.size());
    EXPECT_EQ(decode_status, oned::Simple8bStatus::kOk) << "Decoding failed.";
    EXPECT_EQ(decoded, data) << "Decoded data does not match original.";
}

TEST(Simple8bTest, BasicSignedEncodingDecoding) {
    std::vector<int64_t> signed_data = {-2, -1, 0, 1, 2, 3, 4, 5};
    auto signed_encoded_size_opt = oned::ComputeSimple8bEncodeSize(signed_data.data(), signed_data.size());
    ASSERT_TRUE(signed_encoded_size_opt.has_value()) << "Failed to compute encoding size for signed data.";
    
    size_t signed_encoded_size = signed_encoded_size_opt.value();
    std::vector<uint64_t> signed_encoded(signed_encoded_size);
    
    oned::Simple8bStatus signed_encode_status = oned::Simple8bEncode(signed_data.data(), signed_data.size(), signed_encoded.data(), signed_encoded.size());
    EXPECT_EQ(signed_encode_status, oned::Simple8bStatus::kOk) << "Encoding signed data failed.";
    
    std::vector<int64_t> signed_decoded(oned::ComputeSimple8bDecodeSize(signed_encoded.data(), signed_encoded.size()));
    oned::Simple8bStatus signed_decode_status = oned::Simple8bDecode(signed_encoded.data(), signed_encoded.size(), signed_decoded.data(), signed_decoded.size());
    EXPECT_EQ(signed_decode_status, oned::Simple8bStatus::kOk) << "Decoding signed data failed.";
    EXPECT_EQ(signed_decoded, signed_data) << "Decoded signed data does not match original.";
}


TEST(Simple8bTest, InsufficientSpace) {
    std::vector<uint64_t> insufficient_space_data = {1, 2, 3, 4, 5};
    auto insufficient_space_encoded_size_opt = oned::ComputeSimple8bEncodeSize(insufficient_space_data.data(), insufficient_space_data.size());
    ASSERT_TRUE(insufficient_space_encoded_size_opt.has_value()) << "Failed to compute encoding size for insufficient space test.";
    
    size_t insufficient_space_encoded_size = insufficient_space_encoded_size_opt.value();
    std::vector<uint64_t> insufficient_space_encoded(insufficient_space_encoded_size - 1); // Not enough space
    
    oned::Simple8bStatus insufficient_space_encode_status = oned::Simple8bEncode(insufficient_space_data.data(), insufficient_space_data.size(), insufficient_space_encoded.data(), insufficient_space_encoded.size());
    EXPECT_EQ(insufficient_space_encode_status, oned::Simple8bStatus::kInsufficientSpace) << "Encoding with insufficient space should fail.";
}


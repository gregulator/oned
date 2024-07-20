#include "oned/chunks.hpp"
#include "oned/stripe.hpp"
#include <gtest/gtest.h>
#include <vector>

using namespace oned;

// Test default constructor
TEST(ChunksTestConstructor, DefaultConstructor) {
    Chunks<int> chunks;
    EXPECT_EQ(chunks.size(), 0);
    EXPECT_TRUE(chunks.empty());
}

// Test constructor with vector
TEST(ChunksTestConstructor, VectorConstructor) {
    std::vector<int> data = {1, 2, 3, 4, 5};
    Chunks<int> chunks(data, 2);

    EXPECT_EQ(chunks.size(), 3);
    EXPECT_EQ(chunks.chunk_size(), 2);
    EXPECT_FALSE(chunks.empty());

    EXPECT_EQ(chunks[0][0], 1);
    EXPECT_EQ(chunks[0][1], 2);
    EXPECT_EQ(chunks[1][0], 3);
    EXPECT_EQ(chunks[1][1], 4);
    EXPECT_EQ(chunks[2][0], 5);
}

// Test constructor with span
TEST(ChunksTestConstructor, SpanConstructor) {
    std::vector<int> data = {1, 2, 3, 4, 5};
    std::span<int> span_data(data);
    Chunks<int> chunks(span_data, 2);

    EXPECT_EQ(chunks.size(), 3);
    EXPECT_EQ(chunks.chunk_size(), 2);
    EXPECT_FALSE(chunks.empty());

    EXPECT_EQ(chunks[0][0], 1);
    EXPECT_EQ(chunks[0][1], 2);
    EXPECT_EQ(chunks[1][0], 3);
    EXPECT_EQ(chunks[1][1], 4);
    EXPECT_EQ(chunks[2][0], 5);
}

// Test constructor with Stripe
TEST(ChunksTestConstructor, StripeConstructor) {
    int data[5] = {1, 2, 3, 4, 5};
    Stripe<int> stripe(data, 5, sizeof(int));
    Chunks<int> chunks(stripe, 2);

    EXPECT_EQ(chunks.size(), 3);
    EXPECT_EQ(chunks.chunk_size(), 2);
    EXPECT_FALSE(chunks.empty());

    EXPECT_EQ(chunks[0][0], 1);
    EXPECT_EQ(chunks[0][1], 2);
    EXPECT_EQ(chunks[1][0], 3);
    EXPECT_EQ(chunks[1][1], 4);
    EXPECT_EQ(chunks[2][0], 5);
}

// Test iterators
TEST(ChunksTestIterators, Iterators) {
    std::vector<int> data = {1, 2, 3, 4, 5};
    Chunks<int> chunks(data, 2);

    std::vector<std::vector<int>> expected_chunks = {{1, 2}, {3, 4}, {5}};

    size_t idx = 0;
    for (auto it = chunks.begin(); it != chunks.end(); ++it, ++idx) {
        std::vector<int> chunk_data;
        for (auto elem : *it) {
            chunk_data.push_back(elem);
        }
        EXPECT_EQ(chunk_data, expected_chunks[idx]);
    }

    idx = expected_chunks.size();
    for (auto it = chunks.rbegin(); it != chunks.rend(); ++it, --idx) {
        std::vector<int> chunk_data;
        for (auto elem : *it) {
            chunk_data.push_back(elem);
        }
        EXPECT_EQ(chunk_data, expected_chunks[idx - 1]);
    }
}

// Test first method
TEST(ChunksTestMethod, First) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8};
    Chunks<int> chunks(data, 2);

    oned::Chunks<int> firstChunks = chunks.first(2);

    EXPECT_EQ(firstChunks.size(), 2);
    EXPECT_EQ(firstChunks.chunk_size(), 2);

    EXPECT_EQ(firstChunks[0][0], 1);
    EXPECT_EQ(firstChunks[0][1], 2);
    EXPECT_EQ(firstChunks[1][0], 3);
    EXPECT_EQ(firstChunks[1][1], 4);
}
// Test last method
TEST(ChunksTestMethod, Last) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8};
    Chunks<int> chunks(data, 2);

    auto lastChunks = chunks.last(2);

    EXPECT_EQ(lastChunks.size(), 2);
    EXPECT_EQ(lastChunks.chunk_size(), 2);

    EXPECT_EQ(lastChunks[0][0], 5);
    EXPECT_EQ(lastChunks[0][1], 6);
    EXPECT_EQ(lastChunks[1][0], 7);
    EXPECT_EQ(lastChunks[1][1], 8);
}
// Test subchunks method
TEST(ChunksTestMethod, Subchunks) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8};
    Chunks<int> chunks(data, 2);

    auto subChunks = chunks.subchunks(1, 2);

    EXPECT_EQ(subChunks.size(), 2);
    EXPECT_EQ(subChunks.chunk_size(), 2);

    EXPECT_EQ(subChunks[0][0], 3);
    EXPECT_EQ(subChunks[0][1], 4);
    EXPECT_EQ(subChunks[1][0], 5);
    EXPECT_EQ(subChunks[1][1], 6);
}
// Test at method with bounds checking
TEST(ChunksTestMethod, At) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8};
    Chunks<int> chunks(data, 2);

    EXPECT_EQ(chunks.at(0)[0], 1);
    EXPECT_EQ(chunks.at(0)[1], 2);
    EXPECT_EQ(chunks.at(1)[0], 3);
    EXPECT_EQ(chunks.at(1)[1], 4);

    EXPECT_THROW(chunks.at(4), std::out_of_range);
}

TEST(ChunksTestMethod, Empty) {
    std::vector<int> data = {};
    Chunks<int> chunks(data, 2);

    EXPECT_TRUE(chunks.empty());
    EXPECT_EQ(chunks.size(), 0);
}

TEST(ChunksTestMethod, Front) {
    std::vector<int> data = {1, 2, 3, 4, 5};
    Chunks<int> chunks(data, 2);
    const oned::Stripe<int>  str = chunks.front();
    //ISSUE cannot directly use chunks.front() and chain it with begin()
    //Can use proxy to hold return value i
    std::vector<int> front_chunk(str.begin(), str.end());
    EXPECT_EQ(front_chunk, (std::vector<int>{1, 2}));
}
TEST(ChunksTestMethod, Back) {
    std::vector<int> data = {1, 2, 3, 4, 5};
    Chunks<int> chunks(data, 2);
    const oned::Stripe<int>  str = chunks.back();
    std::vector<int> back_chunk(str.begin(), str.end());
    EXPECT_EQ(back_chunk, (std::vector<int>{5}));
}
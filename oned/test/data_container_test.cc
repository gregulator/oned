#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "oned/data_container.hpp"

TEST(DataContainerTest, AddChannel) {
    DataContainer container;

    // Add a channel with type int and name "ChannelA"
    auto channelA = container.AddChannel<int>("ChannelA");
    ASSERT_NE(channelA, nullptr);
    EXPECT_EQ(channelA->Description().name, "ChannelA");
    EXPECT_EQ(channelA->Description().type_id, typeid(int).name());

    // Add another channel with type double and name "ChannelB"
    auto channelB = container.AddChannel<double>("ChannelB");
    ASSERT_NE(channelB, nullptr);
    EXPECT_EQ(channelB->Description().name, "ChannelB");
    EXPECT_EQ(channelB->Description().type_id, typeid(double).name());

    // Retrieve channel by name and verify
    auto retrievedChannelA = container.GetChannel("ChannelA");
    ASSERT_NE(retrievedChannelA, nullptr);
    EXPECT_EQ(retrievedChannelA->Description().name, "ChannelA");
    EXPECT_EQ(retrievedChannelA->Description().type_id, typeid(int).name());

    auto retrievedChannelB = container.GetChannel("ChannelB");
    ASSERT_NE(retrievedChannelB, nullptr);
    EXPECT_EQ(retrievedChannelB->Description().name, "ChannelB");
    EXPECT_EQ(retrievedChannelB->Description().type_id, typeid(double).name());
}

TEST(DataContainerTest, AddChunk) {
    DataContainer container;

    // Add a chunk to the container
    auto chunk = container.AddChunk();
    ASSERT_NE(chunk, nullptr);
    EXPECT_EQ(chunk->Index(), 0);

    // Add another chunk to the container
    auto chunk2 = container.AddChunk();
    ASSERT_NE(chunk2, nullptr);
    EXPECT_EQ(chunk2->Index(), 1);

    // Verify the chunks
    auto chunks = container.Chunks();
    EXPECT_EQ(chunks.size(), 2);
    EXPECT_EQ(chunks[0].Index(), 0);
    EXPECT_EQ(chunks[1].Index(), 1);
}

TEST(DataContainerTest, DataAccess) {
    DataContainer container;

    // Add a channel with type int and name "ChannelA"
    auto channelA = container.AddChannel<int>("ChannelA");
    ASSERT_NE(channelA, nullptr);

    // Add a chunk to the container
    auto chunk = container.AddChunk();
    ASSERT_NE(chunk, nullptr);
    EXPECT_EQ(chunk->Index(), 0);

    // Add data to the channel in the chunk
    auto data = channelA->MutableData<int>(0);
    ASSERT_NE(data, nullptr);
    data->push_back(1);
    data->push_back(2);
    data->push_back(3);

    // Retrieve the data and verify
    auto retrievedData = channelA->Data<int>(0);
    EXPECT_EQ(retrievedData.size(), 3);
    EXPECT_EQ(retrievedData[0], 1);
    EXPECT_EQ(retrievedData[1], 2);
    EXPECT_EQ(retrievedData[2], 3);
}

TEST(DataContainerTest, HeterogeneousDataAccess) {
    DataContainer container;

    // Add channels with different types
    auto channelA = container.AddChannel<int>("ChannelA");
    auto channelB = container.AddChannel<double>("ChannelB");
    auto channelC = container.AddChannel<std::string>("ChannelC");

    // Add a chunk to the container
    auto chunk = container.AddChunk();
    ASSERT_NE(chunk, nullptr);
    EXPECT_EQ(chunk->Index(), 0);

    // Add data to each channel in the chunk
    auto dataA = channelA->MutableData<int>(0);
    auto dataB = channelB->MutableData<double>(0);
    auto dataC = channelC->MutableData<std::string>(0);
    ASSERT_NE(dataA, nullptr);
    ASSERT_NE(dataB, nullptr);
    ASSERT_NE(dataC, nullptr);
    dataA->push_back(1);
    dataA->push_back(2);
    dataA->push_back(3);
    dataB->push_back(1.1);
    dataB->push_back(2.2);
    dataB->push_back(3.3);
    dataC->push_back("one");
    dataC->push_back("two");
    dataC->push_back("three");

    // Retrieve and verify the data
    auto retrievedDataA = channelA->Data<int>(0);
    auto retrievedDataB = channelB->Data<double>(0);
    auto retrievedDataC = channelC->Data<std::string>(0);
    EXPECT_EQ(retrievedDataA.size(), 3);
    EXPECT_EQ(retrievedDataA[0], 1);
    EXPECT_EQ(retrievedDataA[1], 2);
    EXPECT_EQ(retrievedDataA[2], 3);
    EXPECT_EQ(retrievedDataB.size(), 3);
    EXPECT_EQ(retrievedDataB[0], 1.1);
    EXPECT_EQ(retrievedDataB[1], 2.2);
    EXPECT_EQ(retrievedDataB[2], 3.3);
    EXPECT_EQ(retrievedDataC.size(), 3);
    EXPECT_EQ(retrievedDataC[0], "one");
    EXPECT_EQ(retrievedDataC[1], "two");
    EXPECT_EQ(retrievedDataC[2], "three");
}

TEST(ChannelDescriptionTest, AddChannelWithDescription) {
    DataContainer container;

    // Add a channel with a specific description
    ChannelDescription description = ChannelDescription::Make<int>("ChannelA");
    description.encoding_stack = {"encoding1", "encoding2"};
    auto channelA = container.AddChannel(description);
    ASSERT_NE(channelA, nullptr);

    // Verify the channel description
    EXPECT_EQ(channelA->Description().name, "ChannelA");
    EXPECT_EQ(channelA->Description().type_id, typeid(int).name());
    EXPECT_EQ(channelA->Description().encoding_stack.size(), 2);
    EXPECT_EQ(channelA->Description().encoding_stack[0], "encoding1");
    EXPECT_EQ(channelA->Description().encoding_stack[1], "encoding2");
}
TEST(DataContainerTest, ChunksConstMethod) {
    DataContainer container;

    // Add chunks to the container
    container.AddChunk();
    container.AddChunk();

    // Use the const Chunks method to retrieve chunks
    const DataContainer& constContainer = container;
    auto chunks = constContainer.Chunks();
    EXPECT_EQ(chunks.size(), 2);
    EXPECT_EQ(chunks[0].Index(), 0);
    EXPECT_EQ(chunks[1].Index(), 1);
}

TEST(DataContainerTest, ChunksMethod) {
    DataContainer container;

    // Add chunks to the container
    container.AddChunk();
    container.AddChunk();

    // Use the non-const Chunks method to retrieve chunks
    auto chunks = container.Chunks();
    EXPECT_EQ(chunks.size(), 2);
    EXPECT_EQ(chunks[0].Index(), 0);
    EXPECT_EQ(chunks[1].Index(), 1);

    // Modify chunks and verify
    auto data = container.MutableData<int>("ChannelA", 0);
    ASSERT_NE(data, nullptr);
    data->push_back(42);
    auto retrievedData = container.Data<int>("ChannelA", 0);
    EXPECT_EQ(retrievedData.size(), 1);
    EXPECT_EQ(retrievedData[0], 42);
}

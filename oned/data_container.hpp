// Copyright (c) 2024 The OneD Authors
// See LICENSE file.
#ifndef __ONED_DATA_CONTAINER_HPP__
#define __ONED_DATA_CONTAINER_HPP__

#include <any>
#include <span>
#include <string>
#include <vector>
#include <unordered_map>
// This library provides a container that holds a collection of heterogenous
// timeseries data. The `DataContainer` class holds timeseries data that is
// organized into channels and chunks.
//
// Conceptually, the data in a DataContainer is organized as followed:
//
//                Chunk 0   | Chunk 1   | Chunk 2  ...
//   Channel A    <data>    | <data>    | <data>
//   Channel B    <data>    | <data>    | <data>
//   Channel C    <data>    | <data>    | <data>
//    ...
//
// Where:
// - Each Channel represents a sequence of data with a name, type and other
//   metadata.
// - Each Chunk represents a block of data across all channels. For example,
// the data in a chunk may correspond to the range of samples that fall within
// a particular time range.
// - Each (Channel, Chunk) pair has variable-length data associated with it.
//
// for (const auto& chunk : data_countainer.Chunks()) {
//   for (const auto& channel : data_container.Channels()) {
//     if (channel.HasType<int32_t>()) {
//       std::vector<int32_t>* data = data_container.Data<int>(chunk, channel);
//     }
//   }
// }

// A ChannelDescription holds metadata information about a Channel.
struct ChannelDescription {
  template <typename T>
  // Create a ChannelDescription for a channel with type <T> and name `name`.
  static ChannelDescription Make(const std::string &name) {
    return Make<T>(name, std::vector<std::string>({}));
  }
  // Create a ChannelDescription for a channel with type <T>,  name `name` and
  // an application-specific list of strings describing how the channel's data
  // has been encoded.
  template <typename T>
  static ChannelDescription
  Make(const std::string &name,
       const std::vector<std::string> &encoding_stack) {
    return ChannelDescription{
        .name = name,
        .type_id = typeid(T).name(),
        .encoding_stack = encoding_stack,
    };
  }
  // String name of the channel.
  std::string name;
  // Compiler-specific type id of the channel's data.
  std::string type_id;
  // Application-specific list of strings describing how the channel's data has
  // been encoded.
  std::vector<std::string> encoding_stack;
};

using ChunkIndex = ssize_t;

// class DataContainer
//
// A DataContainer holds a collection of heterogenous data organized into
// channels and chunks.
class DataContainer {
public:
  // View the data for a (channel, chunk) pair. The type_id of T must match the
  // type_id in that channel's ChannelDescription.
  template <typename T>
  std::span<const T> Data(const std::string &channel, ChunkIndex chunk) const {
    // Return an empty span if necessary. Since this method is const we
    // shouldn't do lazy init here.
    if (data_[channel].size() < chunk + 1) {
      return std::span<const T>();
    }
    return std::span<const T>(
        std::any_cast<std::vector<T> &>(data_.at(channel)[chunk]));
  }

  // Get a reference to the mutable data for a (channel, chunk) pair. The
  // type_id of T must match the type_id in that channel's ChannelDescription.
  template <typename T>
  std::vector<T> *MutableData(const std::string &channel, ChunkIndex chunk) {
    // Lazily create the data array if necessary.
    if (data_[channel].size() < chunk + 1) {
      data_[channel].resize(chunk + 1);
    }
    if (!data_[channel][chunk].has_value()) {
      data_[channel][chunk] = std::vector<T>();
    }

    return &std::any_cast<std::vector<T> &>(data_[channel][chunk]);
  }

  // Channel methods.
  class Channel {
  public:
    const ChannelDescription &Description() const { return description_; };

    // Returns true if this channel has data of type T.
    template <typename T> bool HasType() const {
      return (typeid(T).name() == description_.type_id);
    }

    // View the data for `chunk` in this channel. The type_id of T must match
    // the type_id in this channel's description.
    template <typename T> std::span<const T> Data(ChunkIndex chunk) const {
      return parent_->Data<T>(Description().name, chunk);
    }

    // Get a reference to the mutable data for a `channel` in this chunk. The
    // type_id of T must match the type_id in that channel's
    // ChannelDescription.
    template <typename T> std::vector<T> *MutableData(ChunkIndex chunk) {
      return parent_->MutableData<T>(Description().name, chunk);
    }

  private:
    Channel(DataContainer *parent, const ChannelDescription &description)
        : parent_(parent), description_(description) {}

    DataContainer *parent_;
    ChannelDescription description_;
    friend class DataContainer;
  };

  // Adds a channel of type `ChannelT` to this DataContainer. The channel's
  // data in each chunk will initially be empty. Takes O(1) time.
  template <typename ChannelT> Channel *AddChannel(const std::string &name) {
    channels_.insert(
        {name, Channel(this, ChannelDescription::Make<ChannelT>(name))});
    return &channels_.at(name);
  }

  // Adds a channel to this DataContainer using `description`. The channel's
  // data in each chunk will initially be empty. Takes O(1) time.
  Channel *AddChannel(const ChannelDescription &description) {
    channels_.insert({description.name, Channel(this, description)});
    return &channels_.at(description.name);
  }

  // Gets a channel by name, returning nullptr if no such channel is found in
  // this data container.
  Channel *GetChannel(const std::string &name) {
    auto it = channels_.find(name);
    if (it == channels_.end()) {
      return nullptr;
    }
    return &it->second;
  }

  // Returns a reference to all channels in this data container, as an
  // unordered map keyed by channel name.
  const std::unordered_map<std::string, Channel> &Channels() const {
    return channels_;
  }

  // Chunk methods.

  // A DataContainer::Chunk represents a block of data across all channels
  class Chunk {
  public:
    // Get the index of this chunk.
    ChunkIndex Index() const { return index_; }

    // View the data for `channel` in this chunk. The type_id of T must match
    // the type_id in that channel's ChannelDescription.
    template <typename T>
    std::span<const T> Data(const std::string &channel) const {
      return parent_->Data<T>(channel, Index());
    }

    // Get a reference to the mutable data for a `channel` in this chunk. The
    // type_id of T must match the type_id in that channel's
    // ChannelDescription.
    template <typename T>
    std::vector<T> *MutableData(const std::string &channel, ChunkIndex chunk) {
      return parent_->MutableData<T>(channel, Index());
    }

  private:
    Chunk(DataContainer *parent, ChunkIndex index)
        : parent_(parent), index_(index) {}
    DataContainer *parent_;
    ChunkIndex index_;

    friend class DataContainer;
  };

  // Adds a new Chunk to the end of the data container and returns a pointer to
  // it. The chunk's data is initially empty for all channels.
  Chunk *AddChunk() {
    chunks_.emplace_back(Chunk(this, chunks_.size()));
    return &chunks_.back();
  }

  // Get reference to all chunks in this data container.
  std::span<const Chunk> Chunks() const { return chunks_; }
  std::span<Chunk> Chunks() { return chunks_; }

private:
  std::vector<Chunk> chunks_;
  std::unordered_map<std::string, Channel> channels_;

  // Underlying data.
  // Each map key is a channel name.
  // Each map value is a vector corresponding to the list of chunks.
  // Each vector entry is an any-casted vector<T> holding the data for that
  // (channel, chunk).
  mutable std::unordered_map<std::string, std::vector<std::any>> data_;
};

#endif // __ONED_DATA_CONTAINER_HPP__

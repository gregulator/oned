// Copyright (c) 2024 Greg Prisament
// See LICENSE file.
//
// Compile and run from root of git repo with:
//
//   clang++ -I. --std=c++20 oned/examples/data_container_example.cc && ./a.out
//
#include "oned/data_container.hpp"

#include <iostream>
#include <vector>

struct RGB {
  double r, g, b;
};

void DataContainerExample() {
  std::cout
      << ChannelDescription::Make<std::vector<int>>(std::string("hi")).type_id
      << std::endl;
  DataContainer data;
  DataContainer::Channel *channel = data.AddChannel<RGB>("rgb");
  DataContainer::Channel *channel2 = data.AddChannel<double>("doubles");
  DataContainer::Chunk *chunk = data.AddChunk();
  *channel->MutableData<RGB>(0) =
      std::vector<RGB>{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};

  for (const DataContainer::Chunk &chunk : data.Chunks()) {
    for (const RGB &rgb : chunk.Data<RGB>("rgb")) {
      std::cout << rgb.r << " " << rgb.g << " " << rgb.b << " " << std::endl;
    }
  }
  for (const auto &[name, channel] : data.Channels()) {
    for (const DataContainer::Chunk &chunk : data.Chunks()) {
      if (channel.HasType<RGB>()) {
        for (const RGB &rgb : channel.Data<RGB>(chunk.Index())) {
          std::cout << rgb.r << " " << rgb.g << " " << rgb.b << " "
                    << std::endl;
        }
      }
    }
  }

  const DataContainer::Channel &channel3 = data.Channels().at("doubles");
  for (const DataContainer::Chunk &chunk : data.Chunks()) {
    std::cout << channel3.Data<double>(chunk.Index()).size() << std::endl;
  }

  std::cout << "Channel \"doubles\" ";
  if (data.GetChannel("doubles") == nullptr) {
    std::cout << "does not exist" << std::endl;
  } else {
    std::cout << "exists" << std::endl;
  }

  std::cout << "Channel \"doublez\" ";
  if (data.GetChannel("doublez") == nullptr) {
    std::cout << "does not exist" << std::endl;
  } else {
    std::cout << "exists" << std::endl;
  }
}

int main() { DataContainerExample(); }

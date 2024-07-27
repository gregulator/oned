#include <benchmark/benchmark.h>
#include "oned/chunks.hpp"
#include <vector>
#include <numeric>
// Function to generate a test sequence
std::vector<int> GenerateTestSequence(size_t size) {
  std::vector<int> data(size);
  std::iota(data.begin(), data.end(), 0);  // Simple increasing sequence
  return data;
}

// Benchmark for constructing Chunks from a vector
static void BM_ConstructChunks(benchmark::State &state) {
  auto size = static_cast<size_t>(state.range(0));
  auto chunk_size = static_cast<size_t>(state.range(1));
  auto data = GenerateTestSequence(size);

  for (auto _ : state) {
    oned::Chunks<int> chunks(data, chunk_size);
    benchmark::DoNotOptimize(chunks);
  }
}

// Benchmark for iterating over Chunks
static void BM_IterateChunks(benchmark::State &state) {
  auto size = static_cast<size_t>(state.range(0));
  auto chunk_size = static_cast<size_t>(state.range(1));
  auto data = GenerateTestSequence(size);
  oned::Chunks<int> chunks(data, chunk_size);

  for (auto _ : state) {
    for (const auto& chunk : chunks) {
      benchmark::DoNotOptimize(chunk);
    }
  }
}

// Register the benchmarks
BENCHMARK(BM_ConstructChunks)->Ranges({{8, 8 << 10}, {2, 64}});
BENCHMARK(BM_IterateChunks)->Ranges({{8, 8 << 10}, {2, 64}});

BENCHMARK_MAIN();

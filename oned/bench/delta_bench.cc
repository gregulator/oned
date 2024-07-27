#include <benchmark/benchmark.h>
#include <vector>
#include "oned/delta.hpp"

// Function to generate a test sequence
std::vector<int> GenerateTestSequence(size_t size) {
  std::vector<int> data(size);
  for (size_t i = 0; i < size; ++i) {
    data[i] = static_cast<int>(i * 2); // Simple increasing sequence
  }
  return data;
}

static void BM_DeltaEncode(benchmark::State &state) {
  auto size = static_cast<size_t>(state.range(0));
  auto source = GenerateTestSequence(size);
  std::vector<int> dest(size);

  for (auto _ : state) {
    oned::DeltaEncode(source, dest);
  }
  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * size * sizeof(int));
}

static void BM_DeltaDecode(benchmark::State &state) {
  auto size = static_cast<size_t>(state.range(0));
  auto source = GenerateTestSequence(size);
  std::vector<int> encoded(size);
  std::vector<int> dest(size);

  oned::DeltaEncode(source, encoded); // Encode first to get a valid encoded sequence

  for (auto _ : state) {
    oned::DeltaDecode(encoded, dest);
  }
  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * size * sizeof(int));
}

// Register the functions as benchmarks
BENCHMARK(BM_DeltaEncode)->Range(8, 8 << 10); // Test with sizes from 8 to 8*1024
BENCHMARK(BM_DeltaDecode)->Range(8, 8 << 10);

BENCHMARK_MAIN();

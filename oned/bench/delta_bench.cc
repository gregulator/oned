#include <benchmark/benchmark.h>
#include <vector>
#include "oned/delta.hpp"


std::vector<int> GenerateTestSequence(size_t size) {
  std::vector<int> data(size);
  for (size_t i = 0; i < size; ++i) {
    data[i] = static_cast<int>(i * 2);
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

// Benchmark for the SIMD version of DeltaEncode
static void BM_DeltaEncodeSIMD(benchmark::State &state) {
  auto size = static_cast<size_t>(state.range(0));
  auto source = GenerateTestSequence(size);
  std::vector<int> dest(size);

  for (auto _ : state) {
    oned::DeltaEncodeSIMD(source, dest);
  }
  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * size * sizeof(int));
}

static void BM_DeltaDecode(benchmark::State &state) {
  auto size = static_cast<size_t>(state.range(0));
  auto source = GenerateTestSequence(size);
  std::vector<int> encoded(size);
  std::vector<int> dest(size);

  oned::DeltaEncode(source, encoded);

  for (auto _ : state) {
    oned::DeltaDecode(encoded, dest);
  }
  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * size * sizeof(int));
}
static void BM_DeltaDecodeSIMD(benchmark::State &state) {
  auto size = static_cast<size_t>(state.range(0));
  auto source = GenerateTestSequence(size);
  std::vector<int> encoded(size);
  std::vector<int> dest(size);

  oned::DeltaEncode(source, encoded);

  for (auto _ : state) {
    oned::DeltaDecodeSIMD(encoded, dest);
  }
  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * size * sizeof(int));
}

BENCHMARK(BM_DeltaEncode)->Range(8, 8 << 14)->Complexity();
BENCHMARK(BM_DeltaEncodeSIMD)->Range(8, 8 << 14)->Complexity();
BENCHMARK(BM_DeltaDecode)->Range(8, 8 << 14)->Complexity();
BENCHMARK(BM_DeltaDecodeSIMD)->Range(8, 8 << 14)->Complexity();

BENCHMARK_MAIN();
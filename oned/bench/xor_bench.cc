#include <benchmark/benchmark.h>
#include <vector>
#include <numeric>
#include "oned/stripe.hpp"
#include "oned/xor.hpp"

using namespace oned;

static void BM_XorEncodeFlatArray(benchmark::State &state) {
  std::vector<int> source(state.range(0));
  std::vector<int> dest(state.range(0));
  std::iota(source.begin(), source.end(), 0);

  for (auto _ : state) {
    XorEncode(source.data(), dest.data(), source.size());
    benchmark::DoNotOptimize(dest);
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_XorEncodeFlatArray)->Range(8, 8 << 14)->Complexity();

static void BM_XorDecodeFlatArray(benchmark::State &state) {
  std::vector<int> source(state.range(0));
  std::vector<int> dest(state.range(0));
  std::iota(source.begin(), source.end(), 0);
  XorEncode(source.data(), dest.data(), source.size());

  for (auto _ : state) {
    XorDecode(dest.data(), source.data(), dest.size());
    benchmark::DoNotOptimize(source);
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_XorDecodeFlatArray)->Range(8, 8 << 14)->Complexity();

static void BM_XorEncodeStripe(benchmark::State &state) {
  std::vector<int> source(state.range(0));
  std::vector<int> dest(state.range(0));
  std::iota(source.begin(), source.end(), 0);
  Stripe<int> sourceStripe(source.data(), source.size(), sizeof(int));
  Stripe<int> destStripe(dest.data(), dest.size(), sizeof(int));

  for (auto _ : state) {
    XorEncode(sourceStripe, destStripe);
    benchmark::DoNotOptimize(dest);
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_XorEncodeStripe)->Range(8, 8 << 14)->Complexity();

static void BM_XorDecodeStripe(benchmark::State &state) {
  std::vector<int> source(state.range(0));
  std::vector<int> dest(state.range(0));
  std::iota(source.begin(), source.end(), 0);
  Stripe<int> sourceStripe(source.data(), source.size(), sizeof(int));
  Stripe<int> destStripe(dest.data(), dest.size(), sizeof(int));
  XorEncode(sourceStripe, destStripe);

  for (auto _ : state) {
    XorDecode(destStripe, sourceStripe);
    benchmark::DoNotOptimize(source);
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_XorDecodeStripe)->Range(8, 8 << 14)->Complexity();

BENCHMARK_MAIN();

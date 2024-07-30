#include <benchmark/benchmark.h>
#include <vector>
#include <numeric>
#include "oned/stripe.hpp"

using namespace oned;

static void BM_StripeCreation(benchmark::State &state) {
  std::vector<int> data(state.range(0));
  for (auto _ : state) {
    Stripe<int> stripe(data.data(), data.size(), sizeof(int));
    benchmark::DoNotOptimize(stripe);
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_StripeCreation)->Range(8, 8 << 14)->Complexity();

static void BM_StripeIteration(benchmark::State &state) {
  std::vector<int> data(state.range(0));
  std::iota(data.begin(), data.end(), 0);
  Stripe<int> stripe(data.data(), data.size(), sizeof(int));

  for (auto _ : state) {
    for (int value : stripe) {
      benchmark::DoNotOptimize(value);
    }
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_StripeIteration)->Range(8, 8 << 14)->Complexity();

static void BM_StripeRandomAccess(benchmark::State &state) {
  std::vector<int> data(state.range(0));
  std::iota(data.begin(), data.end(), 0);
  Stripe<int> stripe(data.data(), data.size(), sizeof(int));

  for (auto _ : state) {
    for (size_t i = 0; i < stripe.size(); ++i) {
      int value = stripe[i];
      benchmark::DoNotOptimize(value);
    }
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_StripeRandomAccess)->Range(8, 8 << 14)->Complexity();

static void BM_StripeReinterpret(benchmark::State &state) {
  struct RGB { uint8_t r, g, b; };
  std::vector<RGB> data(state.range(0));

  for (auto _ : state) {
    Stripe<RGB> stripe(data.data(), data.size(), sizeof(RGB));
    auto greenStripe = Reinterpret<uint8_t>(Stripe<uint8_t>(&data[0].g, data.size(), sizeof(RGB)));
    benchmark::DoNotOptimize(greenStripe);
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_StripeReinterpret)->Range(8, 8 << 14)->Complexity();

BENCHMARK_MAIN();

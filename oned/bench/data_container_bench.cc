#include <benchmark/benchmark.h>
#include "oned/data_container.hpp"

// Benchmark adding channels to the DataContainer
static void BM_AddChannel(benchmark::State& state) {
    for (auto _ : state) {
        DataContainer container;
        for (int i = 0; i < state.range(0); ++i) {
            container.AddChannel<int>("channel_" + std::to_string(i));
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AddChannel)->RangeMultiplier(2)->Range(1, 1 << 10)->Complexity();

// Benchmark adding chunks to the DataContainer
static void BM_AddChunk(benchmark::State& state) {
    DataContainer container;
    container.AddChannel<int>("channel");
    for (auto _ : state) {
        container.AddChunk();
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AddChunk)->RangeMultiplier(2)->Range(1, 1 << 10)->Complexity();

// Benchmark accessing data in the DataContainer
static void BM_AccessData(benchmark::State& state) {
    DataContainer container;
    auto channelA = container.AddChannel<int>("channel");
    auto* chunk = container.AddChunk();
    std::vector<int>* data = channelA->MutableData<int>(0);

    for (int i = 0; i < state.range(0); ++i) {
        data->push_back(i);
    }

    for (auto _ : state) {
        auto span = chunk->Data<int>("channel");
        benchmark::DoNotOptimize(span.data());
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AccessData)->Range(8, 8 << 10)->Complexity();

BENCHMARK_MAIN();

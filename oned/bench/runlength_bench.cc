#include <benchmark/benchmark.h>
#include "oned/runlength.hpp"
#include <vector>
#include <random>

// Function to generate random byte sequences
std::vector<uint8_t> GenerateRandomBytes(size_t size) {
    std::vector<uint8_t> data(size);
    std::default_random_engine gen;
    std::uniform_int_distribution<> dis(0, 255);
    for (auto& byte : data) {
        byte = static_cast<uint8_t>(dis(gen));
    }
    return data;
}

// Benchmark for run_length encoding
static void BM_RunlengthEncode(benchmark::State &state) {
    auto size = static_cast<size_t>(state.range(0));
    auto data = GenerateRandomBytes(size);
    std::vector<uint8_t> encoded(oned::ComputeRunlengthEncodeSize(data.data(), data.size()));

    for (auto _ : state) {
        oned::RunlengthStatus status = oned::RunlengthEncode(data.data(), data.size(), encoded.data(), encoded.size());
        benchmark::DoNotOptimize(encoded);
        if (status != oned::RunlengthStatus::kOk) {
            state.SkipWithError("Encoding failed");
        }
    }
    state.SetComplexityN(state.range(0));
}

// Benchmark for run_length decoding
static void BM_RunlengthDecode(benchmark::State &state) {
    auto size = static_cast<size_t>(state.range(0));
    auto data = GenerateRandomBytes(size);
    std::vector<uint8_t> encoded(oned::ComputeRunlengthEncodeSize(data.data(), data.size()));
    oned::RunlengthStatus status = oned::RunlengthEncode(data.data(), data.size(), encoded.data(), encoded.size());

    if (status != oned::RunlengthStatus::kOk) {
        state.SkipWithError("Initial encoding failed");
    }

    std::vector<uint8_t> decoded(oned::ComputeRunlengthDecodeSize(encoded.data(), encoded.size()));

    for (auto _ : state) {
        status = oned::RunlengthDecode(encoded.data(), encoded.size(), decoded.data(), decoded.size());
        benchmark::DoNotOptimize(decoded);
        if (status != oned::RunlengthStatus::kOk) {
            state.SkipWithError("Decoding failed");
        }
    }
    state.SetComplexityN(state.range(0));
}


BENCHMARK(BM_RunlengthEncode)->RangeMultiplier(2)->Range(64, 128000)->Complexity();
BENCHMARK(BM_RunlengthDecode)->RangeMultiplier(2)->Range(64, 128000)->Complexity();

BENCHMARK_MAIN();

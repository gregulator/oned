#include <benchmark/benchmark.h>
#include "oned/runlength.hpp"
#include <vector>
#include <random>

// Function to generate random byte sequences
std::vector<uint8_t> GenerateRandomBytes(size_t size) {
    std::vector<uint8_t> data(size);
    std::default_random_engine gen;
    std::uniform_int_distribution<> dis(0, 255);
    
    size_t i = 0;
    while (i < size) {
        // Generate a random value
        uint8_t value = static_cast<uint8_t>(dis(gen));
        
        // Generate a run of 20 to 40 consecutive identical values
        size_t run_length = std::min<size_t>(size - i, 20 + (dis(gen) % 21));
        std::fill_n(data.begin() + i, run_length, value);
        i += run_length;

        // Optionally, generate a few random non-repeated values in between the runs
        size_t random_length = std::min<size_t>(size - i, dis(gen) % 10);
        for (size_t j = 0; j < random_length; ++j) {
            data[i++] = static_cast<uint8_t>(dis(gen));
        }
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
static void BM_RunlengthEncodeSIMD(benchmark::State &state) {
    auto size = static_cast<size_t>(state.range(0));
    auto data = GenerateRandomBytes(size);
    std::vector<uint8_t> encoded(oned::ComputeRunlengthEncodeSize(data.data(), data.size()));

    for (auto _ : state) {
        oned::RunlengthStatus status = oned::RunlengthEncodeSIMD(data.data(), data.size(), encoded.data(), encoded.size());
        benchmark::DoNotOptimize(encoded);
        if(status == oned::RunlengthStatus::kInsufficientSpace){
            state.SkipWithError("Space error");
        }
        else if (status != oned::RunlengthStatus::kOk) {
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

static void BM_RunlengthDecodeSIMD(benchmark::State &state) {
    auto size = static_cast<size_t>(state.range(0));
    auto data = GenerateRandomBytes(size);
    std::vector<uint8_t> encoded(oned::ComputeRunlengthEncodeSize(data.data(), data.size()));
    oned::RunlengthStatus status = oned::RunlengthEncodeSIMD(data.data(), data.size(), encoded.data(), encoded.size());

    if (status != oned::RunlengthStatus::kOk) {
        state.SkipWithError("Initial encoding failed");
    }

    std::vector<uint8_t> decoded(oned::ComputeRunlengthDecodeSize(encoded.data(), encoded.size()));

    for (auto _ : state) {
        status = oned::RunlengthDecodeSIMD(encoded.data(), encoded.size(), decoded.data(), decoded.size());
        benchmark::DoNotOptimize(decoded);
        if (status != oned::RunlengthStatus::kOk) {
            state.SkipWithError("Decoding failed");
        }
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_RunlengthEncode)->RangeMultiplier(2)->Range(64, 128000)->Complexity();
BENCHMARK(BM_RunlengthEncodeSIMD)->RangeMultiplier(2)->Range(64, 128000)->Complexity();
BENCHMARK(BM_RunlengthDecode)->RangeMultiplier(2)->Range(64, 128000)->Complexity();
BENCHMARK(BM_RunlengthDecodeSIMD)->RangeMultiplier(2)->Range(64, 128000)->Complexity();

BENCHMARK_MAIN();

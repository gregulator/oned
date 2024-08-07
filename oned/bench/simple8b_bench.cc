#include <benchmark/benchmark.h>
#include "oned/simple8b.hpp"
#include <vector>
#include <random>
#include <cstdint>


std::vector<uint64_t> generate_random_unsigned(size_t size, uint64_t max_value) {
    std::vector<uint64_t> data(size);
    std::default_random_engine gen;
    std::uniform_int_distribution<uint64_t> dis(0, max_value);

    for (size_t i = 0; i < size; ++i) {
        data[i] = dis(gen);
    }
    return data;
}

std::vector<int64_t> generate_random_signed(size_t size, int64_t min_value, int64_t max_value) {
    std::vector<int64_t> data(size);
    std::default_random_engine gen;
    std::uniform_int_distribution<int64_t> dis(min_value, max_value);

    for (size_t i = 0; i < size; ++i) {
        data[i] = dis(gen);
    }
    return data;
}

static void BM_Simple8bEncodeUnsigned(benchmark::State& state) {
    auto data = generate_random_unsigned(state.range(0), (1ULL << 60) - 1);
    size_t encoded_size = oned::ComputeSimple8bEncodeSize(data.data(), data.size()).value();
    std::vector<uint64_t> encoded(encoded_size);

    for (auto _ : state) {
        oned::Simple8bEncode(data.data(), data.size(), encoded.data(), encoded.size());
    }
    state.SetComplexityN(state.range(0));
}

static void BM_Simple8bDecodeUnsigned(benchmark::State& state) {
    auto data = generate_random_unsigned(state.range(0), (1ULL << 60) - 1);
    size_t encoded_size = oned::ComputeSimple8bEncodeSize(data.data(), data.size()).value();
    std::vector<uint64_t> encoded(encoded_size);
    oned::Simple8bEncode(data.data(), data.size(), encoded.data(), encoded.size());
    size_t decoded_size = oned::ComputeSimple8bDecodeSize(encoded.data(), encoded.size());
    std::vector<uint64_t> decoded(decoded_size);

    for (auto _ : state) {
        oned::Simple8bDecode(encoded.data(), encoded.size(), decoded.data(), decoded.size());
    }
    state.SetComplexityN(state.range(0));
}

static void BM_Simple8bEncodeSigned(benchmark::State& state) {
    auto data = generate_random_signed(state.range(0), -(1LL << 59), (1LL << 59) - 1);
    size_t encoded_size = oned::ComputeSimple8bEncodeSize(data.data(), data.size()).value();
    std::vector<uint64_t> encoded(encoded_size);

    for (auto _ : state) {
        oned::Simple8bEncode(data.data(), data.size(), encoded.data(), encoded.size());
    }
    state.SetComplexityN(state.range(0));
}

static void BM_Simple8bDecodeSigned(benchmark::State& state) {
    auto data = generate_random_signed(state.range(0), -(1LL << 59), (1LL << 59) - 1);
    size_t encoded_size = oned::ComputeSimple8bEncodeSize(data.data(), data.size()).value();
    std::vector<uint64_t> encoded(encoded_size);
    oned::Simple8bEncode(data.data(), data.size(), encoded.data(), encoded.size());
    size_t decoded_size = oned::ComputeSimple8bDecodeSize(encoded.data(), encoded.size());
    std::vector<int64_t> decoded(decoded_size);

    for (auto _ : state) {
        oned::Simple8bDecode(encoded.data(), encoded.size(), decoded.data(), decoded.size());
    }
    state.SetComplexityN(state.range(0));
}


BENCHMARK(BM_Simple8bEncodeUnsigned)->Range(1<<10, 1<<20)->Complexity();
BENCHMARK(BM_Simple8bDecodeUnsigned)->Range(1<<10, 1<<20)->Complexity();
BENCHMARK(BM_Simple8bEncodeSigned)->Range(1<<10, 1<<20)->Complexity();
BENCHMARK(BM_Simple8bDecodeSigned)->Range(1<<10, 1<<20)->Complexity();

BENCHMARK_MAIN();

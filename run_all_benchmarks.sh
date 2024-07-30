benchmarks=(
    "delta_bench"
    "chunks_bench"
    "data_container_bench"
    "runlength_bench"
    "simple8b_bench"
    "stripe_bench"
    "xor_bench"
)

output_file="BENCHMARK.md"

if [ -f "$output_file" ]; then
    rm "$output_file"
fi

# Run each benchmark
for bench in "${benchmarks[@]}"; do
    echo  "Running $bench..." >> BENCHMARK.mb
    bazel run --compilation_mode=opt oned/bench:$bench >> BENCHMARK.mb
    echo "$bench completed. " >> BENCHMARK.mb
    echo '===========================================================================================================================' >> BENCHMARK.mb
done

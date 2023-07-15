cmake_minimum_required(VERSION 3.13)
project(FrequencyCounter C CXX)
set (CMAKE_CXX_STANDARD 14)

set(SOURCES
    "main.cpp"
)

set(BENCHMARK_SOURCES
    "benchmark.cpp"
)

set(CMAKE_CXX_FLAGS "-O3 -pg")

add_executable("freq" ${SOURCES})
add_executable("benchmark" ${BENCHMARK_SOURCES})

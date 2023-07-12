cmake_minimum_required(VERSION 3.15)
project(FrequencyCounter C CXX)
set (CMAKE_CXX_STANDARD 14)

set(SOURCES
    "main.cpp"
)

add_executable("freq" ${SOURCES})

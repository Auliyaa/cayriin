cmake_minimum_required(VERSION 3.21.0)

project(kinucnc VERSION 1.0.0 LANGUAGES CXX)

# setup toolchain
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# options
option(CAYRIIN_TESTS "enable unit tests compilation (requires gtest)" ON)
option(CAYRIIN_SAMPLES "enable samples compilation" ON)

# dependencies
find_package(OpenMP REQUIRED)

# subfolders
add_subdirectory(rawvideo)

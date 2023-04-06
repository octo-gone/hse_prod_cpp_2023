cmake_minimum_required(VERSION 3.10)

project(inline_vector VERSION 1.0)

# C++ configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wpedantic -Werror)

enable_testing()
include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(projects/invec_wrapper)
add_subdirectory(testsuite/libsrc/inline_vector)
add_subdirectory(testsuite/invec_wrapper)
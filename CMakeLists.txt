cmake_minimum_required(VERSION 3.15)
project(Permutation_Cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(permutation_test
    src/permutation.cpp
    test/test_permutation.cpp
)
target_include_directories(permutation_test PRIVATE include)

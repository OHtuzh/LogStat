cmake_minimum_required(VERSION 3.24.2)
project(LogStats)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(test)


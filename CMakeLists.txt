cmake_minimum_required (VERSION 2.8)
project (reflect)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

find_package (LLVM REQUIRED)
find_package (Clang REQUIRED)

set (CMAKE_CXX_STANDARD 11)

set(REFLECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(examples)



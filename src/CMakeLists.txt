cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++14)
project(fastunfolding)
add_executable(fastunfolding fastunfolding.cpp)
add_executable(fastunfolding_3c fastunfolding_3c.cpp)
add_executable(fastunfolding_selfloop fastunfolding_selfloop.cpp)
SET(EXECUTABLE_OUTPUT_PATH ../)
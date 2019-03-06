cmake_minimum_required(VERSION 3.1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(test_main test_main.cc test_queue.cc catch.hpp)

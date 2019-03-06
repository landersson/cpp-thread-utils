project(thread-utils)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(test_queue examples/test_queue.cc)
target_link_libraries(test_queue fmt pthread)

add_executable(test_worker examples/test_worker.cc)
target_link_libraries(test_worker fmt pthread)

add_subdirectory(test)

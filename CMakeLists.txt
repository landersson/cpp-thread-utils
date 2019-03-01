project(thread-utils)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
#add_subdirectory(fmt)

add_executable(test_worker test_worker.cc)
target_link_libraries(test_worker fmt pthread)

add_executable(test_queue test_queue.cc)
target_link_libraries(test_queue fmt pthread)

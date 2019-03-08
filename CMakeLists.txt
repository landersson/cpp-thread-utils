project(thread-utils)
cmake_minimum_required(VERSION 3.10)

add_library(thread-utils INTERFACE)
target_include_directories(thread-utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(test_queue examples/test_queue.cc)
target_link_libraries(test_queue thread-utils fmt pthread)

add_executable(test_worker examples/test_worker.cc)
target_link_libraries(test_worker thread-utils fmt pthread)

add_subdirectory(test)

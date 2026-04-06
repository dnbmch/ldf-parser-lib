cmake_minimum_required(VERSION 3.14)
project(ldf_signal_dump_example LANGUAGES CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_BINARY_DIR}/ldfparser)

add_executable(ldf_signal_dump main.cpp)
target_link_libraries(ldf_signal_dump PRIVATE ldfextract)

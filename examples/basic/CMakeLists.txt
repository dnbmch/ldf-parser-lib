cmake_minimum_required(VERSION 3.14)
project(ldf_basic_example LANGUAGES CXX)

# Pull in ldf-parser as a subdirectory.
# Adjust the path if you place this example elsewhere.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_BINARY_DIR}/ldfparser)

add_executable(ldf_basic main.cpp)
target_link_libraries(ldf_basic PRIVATE ldfextract)

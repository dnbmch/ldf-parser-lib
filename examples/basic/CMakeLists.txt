cmake_minimum_required(VERSION 3.15)
project(ldf_basic_example LANGUAGES CXX)

find_package(ldfparser CONFIG REQUIRED)
add_executable(ldf_basic main.cpp)
target_link_libraries(ldf_basic PRIVATE ldfparser::ldfparser)

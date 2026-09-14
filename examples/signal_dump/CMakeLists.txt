cmake_minimum_required(VERSION 3.15)
project(ldf_signal_dump_example LANGUAGES CXX)

find_package(ldfparser CONFIG REQUIRED)
add_executable(ldf_signal_dump main.cpp)
target_link_libraries(ldf_signal_dump PRIVATE ldfparser::ldfparser)

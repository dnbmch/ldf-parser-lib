cmake_minimum_required(VERSION 3.15)
project(ldf_json_example LANGUAGES CXX)

find_package(ldfparser CONFIG REQUIRED)
add_executable(ldf_json main.cpp)
target_link_libraries(ldf_json PRIVATE ldfparser::ldfparser)

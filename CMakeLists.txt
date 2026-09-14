cmake_minimum_required(VERSION 3.15)
project(ldfparser-lib LANGUAGES CXX)

find_package(ldfparser CONFIG REQUIRED)

# ---------------------------------------------------------------------------
# Examples
# ---------------------------------------------------------------------------
option(LDF_BUILD_EXAMPLES "Build example applications" ON)

if(LDF_BUILD_EXAMPLES)
    add_executable(ldf_basic examples/basic/main.cpp)
    target_link_libraries(ldf_basic PRIVATE ldfparser::ldfparser)

    add_executable(ldf_json examples/json_export/main.cpp)
    target_link_libraries(ldf_json PRIVATE ldfparser::ldfparser)

    add_executable(ldf_signal_dump examples/signal_dump/main.cpp)
    target_link_libraries(ldf_signal_dump PRIVATE ldfparser::ldfparser)
endif()

cmake_minimum_required(VERSION 3.15)
project(ldfextract-lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0091 NEW)

# ---------------------------------------------------------------------------
# Find protobuf (needed for linking)
# ---------------------------------------------------------------------------
find_package(Protobuf CONFIG QUIET)
if(NOT TARGET protobuf::libprotobuf)
    find_package(Protobuf REQUIRED)
endif()
find_package(absl CONFIG QUIET)

# ---------------------------------------------------------------------------
# Prebuilt static library
# ---------------------------------------------------------------------------
if(NOT LDF_LIB_DIR)
    set(LDF_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib" CACHE PATH
        "Directory containing prebuilt ldfextract library and generated .pb.h files")
endif()
get_filename_component(LDF_LIB_DIR "${LDF_LIB_DIR}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

find_library(LDF_LIB_PATH NAMES ldfextract PATHS "${LDF_LIB_DIR}" NO_DEFAULT_PATH)
if(NOT LDF_LIB_PATH)
    if(EXISTS "${LDF_LIB_DIR}/libldfextract.a")
        set(LDF_LIB_PATH "${LDF_LIB_DIR}/libldfextract.a")
    elseif(EXISTS "${LDF_LIB_DIR}/ldfextract.lib")
        set(LDF_LIB_PATH "${LDF_LIB_DIR}/ldfextract.lib")
    else()
        message(FATAL_ERROR "Could not find ldfextract library in ${LDF_LIB_DIR}")
    endif()
endif()

add_library(ldfextract STATIC IMPORTED GLOBAL)
set_target_properties(ldfextract PROPERTIES
    IMPORTED_LOCATION "${LDF_LIB_PATH}"
)

target_include_directories(ldfextract INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LDF_LIB_DIR}
)

target_link_libraries(ldfextract INTERFACE protobuf::libprotobuf)

# ---------------------------------------------------------------------------
# Examples
# ---------------------------------------------------------------------------
option(LDF_BUILD_EXAMPLES "Build example applications" ON)

if(LDF_BUILD_EXAMPLES)
    add_executable(ldf_basic examples/basic/main.cpp)
    target_link_libraries(ldf_basic PRIVATE ldfextract)

    add_executable(ldf_json examples/json_export/main.cpp)
    target_link_libraries(ldf_json PRIVATE ldfextract)

    add_executable(ldf_signal_dump examples/signal_dump/main.cpp)
    target_link_libraries(ldf_signal_dump PRIVATE ldfextract)
endif()

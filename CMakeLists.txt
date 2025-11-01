# CMake minimum version requirement
cmake_minimum_required(VERSION 3.16)

# Project name and version
project(CppTemplate VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Collect all source files from src directory
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.cc" "src/*.cxx")

# Create executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

# Set target properties
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add examples subdirectory if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/examples/CMakeLists.txt)
    add_subdirectory(examples)
endif()

# Print build information
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
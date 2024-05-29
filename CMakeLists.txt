cmake_minimum_required(VERSION 3.10)  # Set the minimum version of CMake required
project(MyC_HashTable)  # Set the project name

# Define the source directory
set(SRC_DIR "${PROJECT_SOURCE_DIR}/MyC_HashTable")

# Collect all .c and .h files under the src directory
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c" "${SRC_DIR}/*.h")

# Add an executable based on the collected source files
add_executable(MyC_HashTable ${SRC_FILES})
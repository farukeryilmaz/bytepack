cmake_minimum_required(VERSION 3.12)

# Set your project name and version
project(BytePack)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add your library
add_library(bytepack INTERFACE)
target_include_directories(bytepack INTERFACE ${PROJECT_SOURCE_DIR}/include)

# Add Catch2 for unit testing
add_subdirectory(extern/Catch2)

# Enable testing and add the test directory
enable_testing()
add_subdirectory(test)
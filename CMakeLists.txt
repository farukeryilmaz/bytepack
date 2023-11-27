cmake_minimum_required(VERSION 3.12)

project(BytePack)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler specific warning flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion -Wsign-conversion)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Weverything)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /w14640 /permissive-)
endif()

add_library(bytepack INTERFACE)
target_include_directories(bytepack INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(extern/Catch2)

enable_testing()
add_subdirectory(test)

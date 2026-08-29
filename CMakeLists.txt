cmake_minimum_required(VERSION 3.12)

project(BytePack)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(bytepack INTERFACE)
target_include_directories(bytepack INTERFACE ${PROJECT_SOURCE_DIR}/include)

# Option for building tests. Pass -DBYTEPACK_BUILD_TESTS=ON to cmake to enable tests building and running.
option(BYTEPACK_BUILD_TESTS "Build the BytePack tests" OFF)
if(BYTEPACK_BUILD_TESTS)
    message("Build the BytePack tests")

    # Catch2 v2 supports these policies but declares an older CMake policy
    # floor. Set it from the parent project without modifying the submodule.
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 4.0)
        if(NOT DEFINED CMAKE_POLICY_VERSION_MINIMUM OR CMAKE_POLICY_VERSION_MINIMUM VERSION_LESS 3.10)
            set(CMAKE_POLICY_VERSION_MINIMUM 3.10)
        endif()
    endif()

    add_subdirectory(extern/Catch2)
    enable_testing()
    add_subdirectory(test)
endif()

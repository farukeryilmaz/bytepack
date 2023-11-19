# Add test executable
add_executable(test_bytepack test_bytepack.cpp)

# Include the Catch2 header
target_include_directories(test_bytepack PRIVATE ${PROJECT_SOURCE_DIR}/extern/Catch2/single_include)

# Link against your library
target_link_libraries(test_bytepack PRIVATE bytepack)

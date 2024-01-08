add_executable(BytePackTests catch2_main.cpp)

target_include_directories(BytePackTests PRIVATE ${PROJECT_SOURCE_DIR}/extern/Catch2/single_include)

target_link_libraries(BytePackTests PRIVATE bytepack)

target_sources(BytePackTests 
    PRIVATE
        fundamental_type_test.cpp
        fixed_width_type_test.cpp
        string_test.cpp
        basic_array_type_test.cpp
        std_containers_test.cpp
        usecase1_test.cpp
)

# Compiler specific warning flags for tests
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion -Wsign-conversion)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /w14640 /permissive-)
endif()

add_test(NAME BytePackTests COMMAND BytePackTests)
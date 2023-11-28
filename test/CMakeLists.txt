add_executable(BytePackTests catch2_main.cpp)

target_include_directories(BytePackTests PRIVATE ${PROJECT_SOURCE_DIR}/extern/Catch2/single_include)

target_link_libraries(BytePackTests PRIVATE bytepack)

target_sources(BytePackTests 
    PRIVATE
        fundamental_type_test.cpp
        string_test.cpp
        basic_array_type_test.cpp
        usecase1_test.cpp
)
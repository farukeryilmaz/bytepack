# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## 0.1.0 - 2024-01-10
### Added
- **Library Introduction:** BytePack, a simple C++20 header-only library for efficient and flexible binary serialization, primarily designed for network communication.
- **Key Features:**
    - Header-only implementation, requiring just the inclusion of `bytepack.hpp`.
    - Configurable endianness support (default: big-endian, network byte order).
    - No reliance on custom encoding, such as IDL or metadata, enhancing interoperability.
    - Exception-free design for predictable error handling.
    - Avoidance of preprocessor macros, focusing on clean and maintainable code.
    - Flexible buffer management using a non-owning mutable buffer concept.
    - Cross-platform compatibility tested on Windows, GNU/Linux, and macOS.
- **Data Types Support:**
    - Fundamental types like `char`, `int`, `double`, etc. ([more](https://en.cppreference.com/w/cpp/language/types))
    - Fixed-width integer types such as `std::uint8_t`, `std::int64_t`, etc._ ([more](https://en.cppreference.com/w/cpp/types/integer))
    - Enumerated types (`enum`, `enum class`).
    - Standard containers (`std::vector`, `std::array`).
    - Standard strings (`std::string`, `std::string_view`).
    - C-style arrays such as _`char[]`, `int[]`, `double[]`, etc._
- **Core Classes:**
    - `binary_stream`: Main class for serializing and deserializing data.
    - `buffer_view`: Non-owning mutable class for buffer management.
- **Documentation:**
    - Motivation & design philosophy, installation & testing, user guide & API, examples using ASIO and Qt, and contribution guidelines.
- **Practical Usage Examples:**
  - Demonstrations of serialization/deserialization in different scenarios, including using default and little-endian configurations, and handling various data types.
- **Community-Driven Roadmap Draft:**
    - An open roadmap inviting community feedback on future development directions and feature considerations.
- **License:**
    - MIT License for open and permissive usage.

### Known Issues and Limitations
- Certain types in C++, such as `std::size_t` and `long int` can vary in size across different architectures and platforms. Utilization of `fixed-width integer types` from `<cstdint>` is recommended for uniform data representation across different platforms and architectures.
- Current lack of support for `std::wstring` and `std::wstring_view` due to variations in `wchar_t` size across platforms. Users are advised to use `std::string` and `std::string_view` for string serialization.
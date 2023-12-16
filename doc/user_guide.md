# User Guide & API Reference
## 1. Introduction:
This document provides an overview of the `BytePack` binary serialization library.
> For installation, read [Installation & Running Tests](installation_and_test.md)

## 2. Supported Data Types
- **Fundamental types** such as _`char`, `int`, `double`, etc._ ([more](https://en.cppreference.com/w/cpp/language/types))
- **Fixed width integer types** such as _`std::uint8_t`, `std::int64_t`, etc._ ([more](https://en.cppreference.com/w/cpp/types/integer))
- **Enumerated types:** _`enum`, `enum class`_
- **Standard Containers:** _`std::vector` and `std::array`_
- **Standard Strings:** _`std::string` and `std::string_view`_
- **C-style arrays** such as _`char[]`, `int[]`, `double[]`, etc._

## 3. Classes
- `binary_stream`: The main class for serializing and deserializing data.
- `buffer_view`: A non-owning mutable class that represents a buffer to provide an interface to access binary data without owning it. It encapsulates a pointer to data and its size. It does not manage the lifetime of the underlying data.

### 3.1 binary_stream
The `binary_stream` class can be instantiated either with a non-owning buffer (_buffer_view_) or a specified size in bytes. When initialized with a size, the binary_stream autonomously allocates and manages the buffer's lifecycle. However, if it is constructed with a user-supplied buffer, it is the user's responsibility to ensure the proper deletion or freeing of the buffer.

**Example Instantiation:**
- Default buffer **endianness** is `big-endian`. If you want to configure as `little-endian`, instantiate as `bytepack::binary_stream<std::endian::little> stream(..`
- Create a stream with a new internal buffer of the specified size in bytes:
  1. `bytepack::binary_stream stream(1024);`
- Create a stream using a user-supplied buffer:
  1. `bytepack::buffer_view buffer(ptr, size);`
  2. `bytepack::binary_stream stream(buffer);`

**Serialization Methods:** _(one method to serialize them all: `stream.write(var);`)_
- Serialize basic types: `stream.write(value);`
- Serialize C-style arrays: `stream.write(arr);`
- Serialize _std::string_ and _std:string_view_:
  - Serialize string with size prefixed (Prepends the string length. Default type of prefix is _std::size_t_): 
    - `stream.write(str);`
  - Serialize string with size prefixed (Prepends the string length. Specify type of prefix): 
    - e.g. `stream.write<std::uint8_t>(str);`
  - Serialize string with null terminated (writes `'\0'` to buffer after the string):
    - `stream.write(str, bytepack::StringMode::NullTerminated);`
- Serialize _std::array_: `stream.write(arr);`
- Serialize _std::vector_:
  - Serialize vector with size prefixed (default type of prefix is _std::size_t_): 
    - `stream.write(vec);`
  - Serialize vector with size prefixed (specify type of prefix): 
    - e.g. `stream.write<std::uint16_t>(vec);`
  - Serialize vector with fixed size (no size prefix): 
    - e.g. `stream.write(vec, 2);`

**Deserialize Methods:** _(one method to deserialize them all: `stream.read(var);`)_
- Deserialize basic types: `stream.read(value);`
- Deserialize C-style arrays: `stream.read(arr);`
- Deserialize _std::string_ and _std:string_view_:
  - Deserialize size prefixed strings (default type of prefix is _std::size_t_): 
    - `stream.read(str);`
  - Deserialize size prefixed strings (specify type of prefix): 
    - e.g. `stream.read<std::uint8_t>(str);`
  - Deserialize null terminated strings (reads until `'\0'` is reached. _It won't exceed the buffer_):
    - `stream.read(str, bytepack::StringMode::NullTerminated);`
- Deserialize _std::array_: `stream.read(arr);`
- Deserialize _std::vector_:
  - Deserialize size prefixed vectors (default type of prefix is _std::size_t_): 
    - `stream.read(vec);`
  - Deserialize size prefixed vectors (specify type of prefix): 
    - e.g. `stream.read<std::uint16_t>(vec);`
  - Deserialize fixed size vectors (no size prefix): 
    - e.g. `stream.read(vec, 2);`

### 3.2 buffer_view
_(incomplete)_
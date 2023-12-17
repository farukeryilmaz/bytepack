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
> **namespace**: Library specific implementations (class, concept, etc.) are under `bytepack` namespace.
- `binary_stream`: The main class for serializing and deserializing data.
- `buffer_view`: A non-owning mutable class that represents a buffer to provide an interface to access binary data without owning it. It encapsulates a pointer to data and its size. It does not manage the lifetime of the underlying data.

## bytepack::**binary_stream**
The `binary_stream` class can be instantiated either with a non-owning buffer (_buffer_view_) or a specified size in bytes. When initialized with a size, the binary_stream autonomously allocates and manages the buffer's lifecycle. However, if it is constructed with a user-supplied buffer, it is the user's responsibility to ensure the proper deletion or freeing of the buffer.

### Example Instantiation:
Default buffer **endianness** is `big-endian`. If you want to configure as `little-endian`, instantiate as `bytepack::binary_stream<std::endian::little> stream(..`
- Create a stream with a new internal buffer of the specified size in bytes:
  1. `bytepack::binary_stream stream(1024);`
- Create a stream using a user-supplied buffer (_read buffer_view class below_):
  1. `bytepack::buffer_view buffer(ptr, size);`
  2. `bytepack::binary_stream stream(buffer);`

### Methods:
- `data()`:
  - Returns a `buffer_view` representing the current state of the internal buffer.
  - Example: `auto data = stream.data();`
    - You can access underlying data pointer and serialized data size with `as<T>()` and `size()`. Read `bytepack::buffer_view` section below.

### Serialization Methods:
> _one method to serialize them all: `stream.write(var);`_
- basic types: `stream.write(value);`
- C-style arrays: `stream.write(arr);`
- _std::string_ and _std:string_view_:
  - Serialize string with size prefixed (Prepends the string length. Default type of prefix is _std::size_t_): 
    - `stream.write(str);`
  - Serialize string with size prefixed (Prepends the string length. Specify type of prefix): 
    - e.g. `stream.write<std::uint8_t>(str);`
  - Serialize string with null terminated (writes `'\0'` to buffer after the string):
    - `stream.write(str, bytepack::StringMode::NullTerminated);`
- _std::array_: `stream.write(arr);`
- _std::vector_:
  - Serialize vector with size prefixed (default type of prefix is _std::size_t_): 
    - `stream.write(vec);`
  - Serialize vector with size prefixed (specify type of prefix): 
    - e.g. `stream.write<std::uint16_t>(vec);`
  - Serialize vector with fixed size (no size prefix): 
    - e.g. `stream.write(vec, 2);`

### Deserialize Methods:
> _one method to deserialize them all: `stream.read(var);`_
- basic types: `stream.read(value);`
- C-style arrays: `stream.read(arr);`
- _std::string_ and _std:string_view_:
  - Deserialize size prefixed strings (default type of prefix is _std::size_t_): 
    - `stream.read(str);`
  - Deserialize size prefixed strings (specify type of prefix): 
    - e.g. `stream.read<std::uint8_t>(str);`
  - Deserialize null terminated strings (reads until `'\0'` is reached. _It won't exceed the buffer_):
    - `stream.read(str, bytepack::StringMode::NullTerminated);`
- _std::array_: `stream.read(arr);`
- _std::vector_:
  - Deserialize size prefixed vectors (default type of prefix is _std::size_t_): 
    - `stream.read(vec);`
  - Deserialize size prefixed vectors (specify type of prefix): 
    - e.g. `stream.read<std::uint16_t>(vec);`
  - Deserialize fixed size vectors (no size prefix): 
    - e.g. `stream.read(vec, 2);`

### Other Methods:
- `reset()`:
  - The `reset` method resets the internal indices used for serialization and deserialization within the `binary_stream`. This method offers significant advantages beyond just resetting indices, especially in the context of network or socket communication. When using a `buffer_view` to hold a non-owning mutable reference to a buffer resource (like a `char*` array) for socket/network communication, `reset` enables efficient and continuous data processing without the need to create new `binary_stream` instances. Each time new data arrives through the network, the `binary_stream` can be reset to start reading from the beginning of the same `buffer_view`. This approach is particularly useful for handling streaming data or processing multiple messages using the same buffer, thereby optimizing resource usage and simplifying buffer management in networked applications. For example, after processing a chunk of incoming data, calling `reset()` prepares the `binary_stream` to handle the next chunk using the same underlying buffer, ensuring seamless and efficient data handling in continuous communication scenarios.

## bytepack::**buffer_view**
### Example Instantiation:
- From C-style Array (_stack buffer_):
  1. `char data[1024]{};`
  2. `bytepack::buffer_view buffer(data);`
- From Pointer and Size:
  1. `char* data = new char[1024]{};`
  2. `bytepack::buffer_view buffer(data, 1024);`
- From _std::string_:
  1. `std::string str = "...`
  2. `bytepack::buffer_view buffer(str);`

### Methods:
- `as<T>()`:
  - Template method to get a pointer to the buffer's data cast to the specified type T.
  - Example: `char* ptr = buffer.as<char>();`
- `size()`:
  - Returns the size of the buffer in bytes.
  - Example: `std::size_t size = buffer.size();`
- `ssize()`:
  - Returns the signed size of the buffer.
  - Example: `std::ptrdiff_t ssize = buffer.ssize();`
- `is_empty()`:
  - Checks if the buffer is empty (size is 0).
  - Example: `bool empty = buffer.is_empty();`
- `operator bool()`:
  - Checks if the buffer is valid (non-null and size greater than 0).
  - Example: `if (buffer) { /* Buffer is valid */ }`

## Example Codes
### Simple Serialization - 1 (_default heap allocated buffer_)
  ```cpp
// Variables to serialize
int intArr[10]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
float num = 29845.221612f;

bytepack::binary_stream stream(1024);

stream.write(intArr);
stream.write(num);

bytepack::buffer_view buffer = stream.data();
char* dataPtr = buffer.as<char>();
std::size_t dataSize = buffer.size(); // total serialized data size (44 byte).
```

### Simple Deserialization - 1 (_buffer instantiated with external resource_)
  ```cpp
int intArr_[10]{};
float num_{};

// create `bytepack::buffer_view buffer(..` with a resource to deserialize from.
bytepack::binary_stream stream(buffer);

stream.read(intArr_);
stream.read(num_);
```

### Simple Serialization - 2 (_stack allocated buffer_)
  ```cpp
// Variables to serialize
int intArr[10]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
float num = 29845.221612f;

char data[64]{}; // stack buffer
bytepack::buffer_view buffer(data);
bytepack::binary_stream stream(buffer);

stream.write(intArr);
stream.write(num);

auto buffer = stream.data();
auto size = buffer.size(); // total serialized data size (44 byte).
```
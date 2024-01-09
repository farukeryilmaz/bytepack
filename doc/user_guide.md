# User Guide & API Reference
## 1. Introduction:
The `BytePack` guide covers its binary serialization capabilities, focusing on various data types, classes, and methods. It includes practical examples and addresses platform-specific concerns, offering solutions for consistent cross-platform serialization.
> For installation, see [Installation & Running Tests](installation_and_test.md)

## 2. Supported Data Types
- **Fundamental types** such as _`char`, `int`, `double`, etc._ ([more](https://en.cppreference.com/w/cpp/language/types))
- **Fixed width integer types** such as _`std::uint8_t`, `std::int64_t`, etc._ ([more](https://en.cppreference.com/w/cpp/types/integer))
- **Enumerated types:** _`enum`, `enum class`_
- **Standard Containers:** _`std::vector` and `std::array`_
- **Standard Strings:** _`std::string` and `std::string_view`_
- **C-style arrays** such as _`char[]`, `int[]`, `double[]`, etc._

## 3. Limitations
1. **Architecture-Dependent Type Sizes:** Types like `std::size_t` vary in size across different architectures. For instance, `std::size_t` is **8 bytes on 64-bit systems** but **4 bytes on 32-bit systems**.
2. **Platform-Specific Type Sizes:** Certain types, such as `long int` and `unsigned long int`, have different sizes on different platforms. They are **8 bytes on Windows** but only **4 bytes on GNU/Linux** systems, even when both platforms are 64-bit.

These variations can cause inconsistent behavior in serialize/deserialize processes across different platforms and architectures.
### 3.1 Recommendations
For consistent cross-platform compatibility, it's recommended to use `fixed-width integer types` from `<cstdint>` (e.g., `std::uint32_t`, `std::int64_t`) to ensure predictable data sizes during serialization and deserialization.

## 4. Classes
> **namespace**: Library specific implementations (class, concept, etc.) are under `bytepack` namespace.
- `binary_stream`: The main class for serializing and deserializing data.
- `buffer_view`: A non-owning mutable class that represents a buffer to provide an interface to access binary data without owning it. It encapsulates a pointer to data and its size. It does not manage the lifetime of the underlying data.

## 4.1 `bytepack::binary_stream`
The `binary_stream` class can be instantiated either with a non-owning buffer (_buffer_view_) or a specified size in bytes. When initialized with a size, the binary_stream autonomously allocates and manages the buffer's lifecycle. However, if it is constructed with a user-supplied buffer, it is the user's responsibility to ensure the proper deletion or freeing of the buffer.

### Example Instantiation:
Default buffer **endianness** is `big-endian`. If you want to configure as `little-endian`, instantiate as `bytepack::binary_stream<std::endian::little> stream(..`
- Create a stream with a new internal buffer of the specified size in bytes:
  1. `bytepack::binary_stream stream(1024);`
- Create a stream using a user-supplied buffer (_read buffer_view class section below_):
  1. `bytepack::buffer_view buffer(ptr, size);`
  2. `bytepack::binary_stream stream(buffer);`

### Serialization Methods:
> _One method to serialize them all: `stream.write(var);`_  
> You can serialize multiple variables in a single call: `stream.write(var1, var2, var3, ...);`
- basic types: `stream.write(value);`
- C-style arrays: `stream.write(arr);`
- _std::string_ and _std:string_view_:
  - Serialize string with size prefixed (Prepends the string length. Default type of prefix is _std::uint32_t_):
    - `stream.write(str);`
  - Serialize string with size prefixed. Specify type of prefix (Prepends the string length):
    - e.g. `stream.write<std::uint8_t>(str);`
  - Serialize string with null terminator (Appends `'\0'` to the end of the string):
    - `stream.write<bytepack::StringMode::NullTerm>(str);`
  - Serialize string with given size (no size prefix):
    - e.g. `stream.write<10>(str);`
    - Note-1: If the string size is greater than the given size, it will be truncated.
    - Note-2: If the string size is less than the given size, it will be padded with `'\0'`.
- _std::array_: `stream.write(arr);`
- _std::vector_:
  - Serialize vector with size prefixed (Prepends the vector size. Default type of prefix is _std::uint32_t_):
    - `stream.write(vec);`
  - Serialize vector with size prefixed. Specify type of prefix (Prepends the vector size):
    - e.g. `stream.write<std::uint16_t>(vec);`
  - Serialize vector with fixed size (no size prefix): 
    - e.g. `stream.write<5>(vec);`
    - Note-1: If the vector size is greater than the given size, it will be truncated.
    - Note-2: If the vector size is less than the given size, it won't be serialized and return false.

### Deserialize Methods:
> _One method to deserialize them all: `stream.read(var);`_  
> You can deserialize multiple variables in a single call: `stream.read(var1, var2, var3, ...);`
- basic types: `stream.read(value);`
- C-style arrays: `stream.read(arr);`
- _std::string_ and _std:string_view_:
  - Deserialize size prefixed strings (default type of prefix is _std::uint32_t_):
    - `stream.read(str);`
  - Deserialize size prefixed strings. Specify type of prefix:
    - e.g. `stream.read<std::uint8_t>(str);`
  - Deserialize null terminated strings (reads until `'\0'` is reached. _It won't exceed the buffer_):
    - `stream.read<bytepack::StringMode::NullTerm>(str_);`
  - Deserialize fixed size strings:
    - e.g. `stream.read<10>(str);`
    - Note-1: If the serialized string size is greater than the given read size, it will be truncated and later read calls will result in incorrect deserialization since the internal indices will be out of sync.
    - Note-2: If the serialized string is padded with `'\0'` characters, they will be removed from the end of the string. This does not affect the internal indices and serialization/deserialization process.
    - Note-3: These Behaviors are applied only to _std::string_ and _std::string_view_ types. C-style strings are not affected by these behaviors.
    - Note-4: You can also use `bytepack::StringMode::NullTerm` to read null terminated `C-style strings (char[])` as `std::string` type.
- _std::array_: `stream.read(arr);`
- _std::vector_:
  - Deserialize size prefixed vectors (default type of prefix is _std::uint32_t_):
    - `stream.read(vec);`
  - Deserialize size prefixed vectors. Specify type of prefix:
    - e.g. `stream.read<std::uint16_t>(vec);`
  - Deserialize fixed size vectors (no size prefix): 
    - e.g. `stream.read<5>(vec);`

### Other Methods:
- `data()`:
  - Returns a `bytepack::buffer_view` representing the current state of the internal buffer.
  - Example: `auto data = stream.data();`
    - You can access underlying data pointer and serialized data size with `as<T>()` and `size()`. Read `bytepack::buffer_view` section below.
- `reset()`:
  - The `reset` method in the `binary_stream` resets internal indices for serialization and deserialization. It's especially beneficial for network/socket communication. With reset, you can efficiently process both incoming and outgoing data without creating new `binary_stream` instances. This is useful for streaming data or handling multiple messages with the same buffer, optimizing resource usage and simplifying buffer management in networked applications.

## 4.2 `bytepack::buffer_view`
### Example Instantiation:
- From C-style Array (_stack buffer_):
  1. `char data[1024]{};`
  2. `bytepack::buffer_view buffer(data);`
- From pointer and size:
  1. `char* data = new char[1024]{};`
  2. `bytepack::buffer_view buffer(data, 1024);`
- From _std::string_:
  1. `std::string str = "...`
  2. `bytepack::buffer_view buffer(str);`
- From _std::array_:
  1. `std::array<char, 1024> arr{};`
  2. `bytepack::buffer_view buffer(arr);`
  3. or `bytepack::buffer_view buffer(arr.data(), size);`
- From void pointer and size:
  1. `void* data = ...`
  2. `bytepack::buffer_view buffer(data, size);`

### Methods:
- `as<T>()`:
  - Template method to get a pointer to the buffer's data cast to the specified type T.
  - Example: `char* ptr = buffer.as<char>();`
- `size()`:
  - Returns the unsigned size of the buffer in bytes.
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

## 5. Example Codes
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

char data[64]{}; // or std::array<char, 64> data{};
bytepack::buffer_view buffer(data);
bytepack::binary_stream stream(buffer);

stream.write(intArr);
stream.write(num);

auto buffer = stream.data();
auto size = buffer.size(); // total serialized data size (44 byte).
```

### Use Case - 1 (Network byte order - big-endian)
```cpp
struct SensorData {
  std::int64_t timestamp; // UNIX timestamp of measurement
  double value;           // Measured value
  char sensor_id[16];     // Identifier of the sensor

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(timestamp, value, sensor_id);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(timestamp, value, sensor_id);
  }
};

SensorData sensorData{ 1701037875, 23.6, "Sensor-001" };

bytepack::binary_stream serializationStream(64);
sensorData.serialize(serializationStream);

bytepack::buffer_view buffer = serializationStream.data();

bytepack::binary_stream deserializationStream(buffer);
SensorData sensorData_{};
sensorData_.deserialize(deserializationStream);
```

### Use Case - 2 (Network byte order - big-endian)
```cpp
```cpp
enum class Type { TYPE_1, TYPE_2, TYPE_3, TYPE_4, TYPE_5 };

struct GPSData {
    Type type;                    // Type of the data
    std::uint32_t timestamp;      // UNIX timestamp
    double latitude;              // Latitude in degrees
    double longitude;             // Longitude in degrees
    float altitude;               // Altitude in meters
    std::uint16_t numSatellites;  // Number of satellites in view
    char deviceID[16];            // Device ID

    void serialize(bytepack::binary_stream<>& stream) const {
        stream.write(type);
        stream.write(timestamp);
        stream.write(latitude);
        stream.write(longitude);
        stream.write(altitude);
        stream.write(numSatellites);
        stream.write(deviceID);
    }

    void deserialize(bytepack::binary_stream<>& stream) {
        stream.read(type);
        stream.read(timestamp);
        stream.read(latitude);
        stream.read(longitude);
        stream.read(altitude);
        stream.read(numSatellites);
        stream.read(deviceID);
    }
};

GPSData gpsData{ Type::TYPE_4, 1701037875, 36.8805426411, 30.6692287448, 123.456f, 12, "GPS-DEVICE-1" };

bytepack::binary_stream serializationStream(1024);
gpsData.serialize(serializationStream);

auto buffer = serializationStream.data();

bytepack::binary_stream deserializationStream(buffer);
GPSData gpsData_{};
gpsData_.deserialize(deserializationStream);
```

### Use Case - 3 (little-endian)
```cpp
struct SensorData {
  std::int64_t timestamp; // UNIX timestamp of measurement
  double value;           // Measured value
  char sensor_id[16];     // Identifier of the sensor

  void serialize(bytepack::binary_stream<std::endian::little>& stream) const {
    stream.write(timestamp, value, sensor_id);
  }

  void deserialize(bytepack::binary_stream<std::endian::little>& stream) {
    stream.read(timestamp, value, sensor_id);
  }
};

SensorData sensorData{ 1701037875, 23.6, "Sensor-001" };

bytepack::binary_stream<std::endian::little> serializationStream(64);
sensorData.serialize(serializationStream);

bytepack::buffer_view buffer = serializationStream.data();

bytepack::binary_stream<std::endian::little> deserializationStream(buffer);
SensorData sensorData_{};
sensorData_.deserialize(deserializationStream);
```
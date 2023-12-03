# BytePack: Simple C++ Binary Serialization Library
Header-only C++20 library designed for efficient and flexible binary serialization and deserialization, allowing users to specify the endianness. BytePack does not enforce specific standardization (IDL, metadata, etc.), making it highly adaptable for projects that rely on external interface, custom data formats, such as those found in Interface Control Documents (ICD), Interface Design Description (IDD), etc. BytePack seamlessly works with such specifications, making it straightforward to integrate into existing systems.

_While it offers several features and benefits, it's important to note that the project is still under development and may not fulfill all features or requirements you might expect from a mature library. The library is evolving, and your feedback and contributions are valuable in shaping its future._

## Features
- **Header-Only:** Easy to integrate, just include `bytepack.hpp` in your project.
- **No Library-Specific Encoding:** Avoids any custom encoding (IDL, metadata, etc.) facilitating interoperability and simplifying the troubleshooting of network packets.
- **Configurable Endianness Support:** Choose the desired endianness for data serialization and deserialization.
- **No Exceptions:** Ensures stable and predictable error handling.
- **Flexible Buffer Management:** Utilizes a non-owning mutable buffer concept for efficient memory management.
- **Easy to Debug and Customize:** Clear and concise codebase makes debugging easy. Adaptable for customization to address specific requirements.
- **Cross-platform compatible:** Tested on `GNU/Linux` and `Windows`.

## Usage Example
This is just one example among many possible scenarios demonstrating the diverse applications and versatility of `BytePack`. It provides a glimpse into the project's potential, encouraging users to explore a wide range of other use cases.
```cpp
// Sample struct for a conceptual usage scenario
struct GPSData {
	std::uint32_t timestamp;      // UNIX timestamp
	double latitude;              // Latitude in degrees
	double longitude;             // Longitude in degrees
	float altitude;               // Altitude in meters
	std::uint16_t numSatellites;  // Number of satellites in view
	char deviceID[16];            // Device ID

	void serialize(bytepack::binary_stream<>& stream) const {
		stream.write(timestamp);
		stream.write(latitude);
		stream.write(longitude);
		stream.write(altitude);
		stream.write(numSatellites);
		stream.write(deviceID);
	}

	void deserialize(bytepack::binary_stream<>& stream) {
		stream.read(timestamp);
		stream.read(latitude);
		stream.read(longitude);
		stream.read(altitude);
		stream.read(numSatellites);
		stream.read(deviceID);
	}
};

GPSData gpsData{ 1701037875, 36.8805426411, 30.6692287448, 123.456f, 12, "GPS-DEVICE-1" };

// default endian is big-endian (network byte order). you can change the endianness by passing
// the endian type as a template parameter, e.g. `bytepack::binary_stream<std::endian::little>`
bytepack::binary_stream serializationStream(1024); // 1024 is the buffer size in bytes

gpsData.serialize(serializationStream);

bytepack::buffer_view buffer = serializationStream.data();

// e.g. char* dataPtr = buffer.as<char>();
std::uint8_t* dataPtr = buffer.as<std::uint8_t>();
std::size_t dataSize = buffer.size(); // returns the serialized data size in bytes
// you can send the dataPtr to a socket, write it to a file, etc.

// If you want to deserialize the data coming from a socket etc.,
// you can initialize non-owning mutable buffer with the data pointer and size.
// e.g. `bytepack::buffer_view buffer(dataPtr, dataSize);`
bytepack::binary_stream deserializationStream(buffer);

GPSData gpsData_{};
gpsData_.deserialize(deserializationStream);
```

## Requirements
- Implemented in `C++20` (uses concepts)
- `CMake 3.12` or higher.
- **GNU/Linux:** `GCC 10.1` or higher, `Clang 11` or higher.
- **Windows:** `Visual Studio 2019 version 16.11.14` or higher.
- **macOS:** _Not officially tested. Feedback and contributions from macOS users are welcome._

## Installation
Simply clone the repository or download the `bytepack.hpp` file and include it in your project.
```bash
git clone https://github.com/farukeryilmaz/bytepack.git
```
If you want to clone the repository with submodules (Catch2 v2.x), use the following command:
```bash
git clone --recursive https://github.com/farukeryilmaz/bytepack.git
```
If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

Include the library in your C++ project:
```cpp
#include "bytepack/bytepack.hpp"
```

## Design Philosophy
BytePack doesn't enforce any standardization, versioning, or the use of Interface Description Language (IDL) in serialization, providing you with the freedom to define your data structures and protocols. This approach is ideal when interfacing with systems where data formats and protocols are defined externally, as is often the case in standards like IEEE 12207, Interface Control Documents (ICD), Interface Design Description (IDD), and other industry-specific specifications. It allows you to seamlessly integrate BytePack into diverse projects, accommodating a wide range of requirements and compliance standards.

## Contributions and Feedback
Contributions are welcome! If you encounter issues, have suggestions, or want to contribute to the development of the library, please feel free to open issues and submit pull requests.

For any questions, feedback, or inquiries, you can also reach out to me directly:
- **Email:** `faruk [at] farukeryilmaz [dot] com`
- **X (Twitter):** [@farukerylmz](https://twitter.com/farukerylmz)

## License
`BytePack` is licensed under the [MIT License](https://github.com/farukeryilmaz/bytepack/blob/main/LICENSE).

MIT License

Copyright (c) 2023 Faruk Eryilmaz and contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

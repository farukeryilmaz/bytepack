# BytePack: Simple C++ Binary Serialization Library
BytePack is a header-only C++20 library designed for efficient and flexible binary serialization and deserialization, allowing users to specify the endianness. BytePack does not enforce specific standardization, making it highly adaptable for projects that rely on custom data formats, such as those found in Interface Control Documents (ICD). BytePack seamlessly works with such specifications, making it straightforward to integrate into existing systems.

_While it offers several features and benefits, it's important to note that the project is still under development and may not fulfill all features or requirements you might expect from a mature library. The library is evolving, and your feedback and contributions are valuable in shaping its future._

## Features
- **Header-Only:** Easy to integrate, just include `bytepack.hpp` in your project.
- **Endian Support:** Choose the desired endianness for data serialization and deserialization.
- **No Exceptions:** Ensures stable and predictable error handling.
- **No Library-Specific Encoding:** Avoids any custom encoding, padding, or alignment, facilitating interoperability and simplifying the troubleshooting of network packets.
- **Easy to Debug and Customize:** Clear and concise codebase makes debugging easy and allows for easy modifications to meet specific needs.
- **Flexible Buffer Management:** Utilizes a non-owning mutable buffer concept for efficient memory management and greater control over buffer handling.
- **Platform Compatibility:** Tested on Windows and Linux.

## Requirements
- Implemented in `C++20` (uses concepts)
- `CMake 3.12` or higher.
- **GNU+Linux:** `GCC 10.1` or higher, `Clang 11` or higher.
- **Windows:** `Visual Studio 2019 version 16.11.14` or higher.
- **macOS:** _Not officially tested. Feedback and contributions from macOS users are welcome._

## Installation
Simply clone the repository or download the `bytepack.hpp` file and include it in your project.
```bash
git clone https://github.com/farukeryilmaz/bytepack.git
```
Include the library in your C++ project:
```cpp
#include "bytepack/bytepack.hpp"
```

## Usage Example
```cpp
struct GPSData {
	std::uint32_t timestamp;      // UNIX timestamp
	double latitude;              // Latitude in degrees
	double longitude;             // Longitude in degrees
	float altitude;               // Altitude in meters
	std::uint16_t numSatellites;  // Number of satellites in view
	char deviceID[16];            // Device ID

	void serialize(bytepack::binary_stream<>& stream) const
	{
		stream.write(timestamp);
		stream.write(latitude);
		stream.write(longitude);
		stream.write(altitude);
		stream.write(numSatellites);
		stream.write(deviceID);
	}

	void deserialize(bytepack::binary_stream<>& stream)
	{
		stream.read(timestamp);
		stream.read(latitude);
		stream.read(longitude);
		stream.read(altitude);
		stream.read(numSatellites);
		stream.read(deviceID);
	}
};

GPSData gpsData{ 1701037875, 36.8805426411, 30.6692287448, 123.456f, 12, "GPS-DEVICE-1" };

// default endian is big-endian (network byte order). you can change the endian by passing
// the endian type as a template parameter, e.g. bytepack::binary_stream<std::endian::little>
bytepack::binary_stream serializationStream(1024);
gpsData.serialize(serializationStream);

bytepack::buffer_view buffer = serializationStream.data();

// e.g. char* dataPtr = buffer.as<char>();
std::uint8_t* dataPtr = buffer.as<std::uint8_t>();
std::size_t dataSize = buffer.size();
// you can send the dataPtr to a socket, write it to a file, etc.

// You do not have to use another binary_stream (`deserializationStream`) to deserialize
// the data. You can deserialize the data directly from the stream (`serializationStream`)
// since you already serialized the data into it. However, if you want to deserialize
// the data coming from a socket etc., you can use another binary_stream to deserialize
// the data like below. e.g. `bytepack::buffer_view buffer(dataPtr, dataSize);`
bytepack::binary_stream deserializationStream(buffer);

GPSData gpsData_{};
gpsData_.deserialize(deserializationStream);
```

## Design Philosophy
BytePack doesn't enforce any standardization or versioning in serialization, giving you the freedom to define your data structures and protocols. This approach is ideal when interfacing with systems where data formats and protocols are defined externally, such as in Interface Control Documents (ICDs).

## Contributions and Feedback
Contributions are welcome! If you encounter issues, have suggestions, or want to contribute to the development of the library, please feel free to open issues and submit pull requests.

For any questions, feedback, or inquiries, you can also reach out to me directly:
- **Email:** `faruk [at] farukeryilmaz [dot] com`
- **X (Twitter):** [@farukerylmz](https://twitter.com/farukerylmz)

## License
This project is licensed under [MIT License](https://github.com/farukeryilmaz/bytepack/blob/main/LICENSE).
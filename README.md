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

## Design Philosophy
BytePack doesn't enforce any standardization or versioning in serialization, giving you the freedom to define your data structures and protocols. This approach is ideal when interfacing with systems where data formats and protocols are defined externally, such as in Interface Control Documents (ICDs).

## Contributions and Feedback
Contributions are welcome! If you encounter issues, have suggestions, or want to contribute to the development of the library, please feel free to open issues and submit pull requests.

For any questions, feedback, or inquiries, you can also reach out to me directly:
- **Email:** `faruk [at] farukeryilmaz [dot] com`
- **X (Twitter):** [@farukerylmz](https://twitter.com/farukerylmz)

## License
This project is licensed under [MIT License](https://github.com/farukeryilmaz/bytepack/blob/main/LICENSE).
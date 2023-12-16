# Installation & Running Tests
## 1. Requirements
- Implemented in `C++20` and only uses C++ Standard Library.
- `CMake 3.12` or higher.
- **GNU/Linux:** `GCC 10.1` or higher, `Clang 11` or higher.
- **Windows:** `Visual Studio 2019 version 16.11.14` or higher.
- **macOS:** `Xcode 14.3` or higher.

## 2. Installation
To use the library, simply clone the repository or [download](../include/bytepack/bytepack.hpp) the `bytepack.hpp` file and include it in your project.
```bash
git clone https://github.com/farukeryilmaz/bytepack.git
```
Include the library in your C++ project:
```cpp
#include "bytepack/bytepack.hpp"
```

## 3. Building and Testing
Clone the repository with submodule `Catch2 v2.x` to run tests:
```bash
git clone --recursive https://github.com/farukeryilmaz/bytepack.git
```
> If you cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

### 3.1 Visual Studio
(incomplete)
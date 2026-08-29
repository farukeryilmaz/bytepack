# Installation & Running Tests

## 1. Requirements

BytePack is a header-only C++20 library that uses only the C++ Standard Library. CMake is not required when you copy or include `bytepack.hpp` directly in another project.

To build this repository's tests, you need:

- CMake 3.12 or newer.
- The bundled Catch2 v2 Git submodule.
- A C++20-compatible compiler:
  - **GNU/Linux:** GCC 10.1 or newer, or Clang 11 or newer.
  - **Windows:** Visual Studio 2019 version 16.11.14 or newer.
  - **macOS:** Xcode 14.3 or newer, or the corresponding/current Apple Command Line Tools. Full Xcode is needed only when generating an Xcode project.

## 2. Installation

For library use only, clone the repository or [download `bytepack.hpp`](../include/bytepack/bytepack.hpp):

```bash
git clone https://github.com/farukeryilmaz/bytepack.git
```

Include the library in your C++ project:

```cpp
#include <bytepack/bytepack.hpp>
```

Clone recursively when you also want to build the tests:

```bash
git clone --recursive https://github.com/farukeryilmaz/bytepack.git
cd bytepack
```

If you already cloned without submodules, initialize Catch2 from the repository root:

```bash
git submodule update --init --recursive
```

## 3. Building and Testing

### 3.1 Visual Studio

You can generate Visual Studio Solution (.sln) with CMake and run the test on Visual Studio:

> Run cmake command under project folder. It generates **BytePack.sln** file under **build** folder.

- **Visual Studio 2026 version 18:** `cmake -S . -B build -G "Visual Studio 18 2026" -DBYTEPACK_BUILD_TESTS=ON`
- **Visual Studio 2022 version 17:** `cmake -S . -B build -G "Visual Studio 17 2022" -DBYTEPACK_BUILD_TESTS=ON`
- **Visual Studio 2019 version 16:** `cmake -S . -B build -G "Visual Studio 16 2019" -DBYTEPACK_BUILD_TESTS=ON`

> The Visual Studio 2026 generator was added in CMake 4.2, so that generator requires CMake 4.2 or newer.

Or you can directly run test using **CMake** with **MSVC**:

```console
cmake -S . -B build -DCMAKE_CXX_STANDARD=20 -DBYTEPACK_BUILD_TESTS=ON
cmake --build build --config Release
cd build
ctest -C Release
```

> To disable building tests, replace `-DBYTEPACK_BUILD_TESTS=ON` with `-DBYTEPACK_BUILD_TESTS=OFF`

### 3.2 GNU/Linux

```bash
cmake -S . -B build -DBYTEPACK_BUILD_TESTS=ON
cmake --build build
cd build
ctest
```

> Use `-DBYTEPACK_BUILD_TESTS=OFF` in the cmake command to skip building tests.

### 3.3 macOS command-line build

Install Apple's Command Line Tools if neither they nor full Xcode are installed:

```bash
xcode-select --install
```

Confirm the active compiler and CMake versions:

```bash
clang++ --version
cmake --version
```

Configure, compile, and run the tests from the repository root:

```bash
cmake -S . -B build \
  -DBYTEPACK_BUILD_TESTS=ON \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

These commands work on both Apple Silicon and Intel Macs. To explicitly produce a universal test binary containing both `arm64` and `x86_64` slices:

```bash
cmake -S . -B build-universal \
  -DBYTEPACK_BUILD_TESTS=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
cmake --build build-universal --parallel
ctest --test-dir build-universal --output-on-failure
```

CTest runs the slice native to the current Mac; the build verifies that both architecture slices compile and link.

### 3.4 macOS Xcode project

Full Xcode is required for the Xcode generator. Generate the project, build its Release configuration, and run the tests with:

```bash
cmake -S . -B build-xcode -G Xcode -DBYTEPACK_BUILD_TESTS=ON
cmake --build build-xcode --config Release
ctest --test-dir build-xcode -C Release --output-on-failure
```

The generated project is located at `build-xcode/BytePack.xcodeproj`.

## 4. macOS Troubleshooting

### Invalid active developer path

Install the Command Line Tools with `xcode-select --install`. If the tools are installed but the selected path is wrong, select either the full Xcode installation or the standalone tools:

```bash
sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer
# Or, for standalone Command Line Tools:
sudo xcode-select --switch /Library/Developer/CommandLineTools
```

### Catch2 source directory is missing

Initialize the test dependency from the repository root:

```bash
git submodule update --init --recursive
```

### Dependencies have the wrong architecture

Use dependencies built for the Mac's native architecture, or configure all dependencies and the project with the same `CMAKE_OSX_ARCHITECTURES` value. Check a binary with `file path/to/binary` and check the current Mac with `uname -m`.

### CMake reports a Catch2 deprecation warning

CMake 4.x can warn that the bundled Catch2 v2 project declares compatibility with an old CMake version. This warning originates in the test dependency and does not prevent BytePack from configuring, compiling, or running its tests.

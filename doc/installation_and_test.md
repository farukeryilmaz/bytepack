# Installation & Running Tests

## 1. Requirements

BytePack is a header-only C++20 library that uses only the C++ Standard Library. CMake is not required when you copy or include `bytepack.hpp` directly in another project.

To build this repository's tests, you need:

- CMake 3.21 or newer.
- The bundled Catch2 v2 Git submodule.
- A C++20-compatible compiler:
  - **GNU/Linux:** GCC 10.1 or newer, or Clang 11 or newer.
  - **Windows:** Visual Studio 2019 version 16.11.14 or newer.
  - **macOS:** Xcode 14.3 or newer, or the corresponding/current Apple Command Line Tools. Full Xcode is needed only when generating an Xcode project.

These are the minimum supported compiler versions. Continuous integration currently tests GCC 12 and Clang 18 on Ubuntu 24.04, Visual Studio 2026 on Windows Server 2025, and AppleClang on macOS 15 and 26.

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

- **Visual Studio 2026 version 18:** `cmake -S . -B build -G "Visual Studio 18 2026" -A x64 -DBYTEPACK_BUILD_TESTS=ON`
- **Visual Studio 2022 version 17:** `cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DBYTEPACK_BUILD_TESTS=ON`
- **Visual Studio 2019 version 16:** `cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DBYTEPACK_BUILD_TESTS=ON`

> The Visual Studio 2026 generator was added in CMake 4.2, so that generator requires CMake 4.2 or newer.

The full Visual Studio IDE is optional for command-line builds. Visual Studio Build Tools is sufficient when the **Desktop development with C++** workload is installed.

Or you can directly run test using **CMake** with **MSVC**:

```console
cmake -S . -B build -DBYTEPACK_BUILD_TESTS=ON
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

Visual Studio generators support multiple configurations in one build directory. The configuration passed to `cmake --build` and `ctest` must therefore match; the commands above use `Release` for both.

> To disable building tests, replace `-DBYTEPACK_BUILD_TESTS=ON` with `-DBYTEPACK_BUILD_TESTS=OFF`.

### 3.2 GNU/Linux

Configure, compile, and run the tests from the repository root:

```bash
cmake -S . -B build \
  -DBYTEPACK_BUILD_TESTS=ON \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

To select a specific compiler, set `CC` and `CXX` during the first configure. For example, these commands select the compiler versions used by continuous integration:

```bash
CC=gcc-12 CXX=g++-12 cmake -S . -B build-gcc \
  -DBYTEPACK_BUILD_TESTS=ON \
  -DCMAKE_BUILD_TYPE=Release

CC=clang-18 CXX=clang++-18 cmake -S . -B build-clang \
  -DBYTEPACK_BUILD_TESTS=ON \
  -DCMAKE_BUILD_TYPE=Release
```

Compiler executable names can vary by distribution. Use separate build directories when switching compilers because CMake caches the selected toolchain. Build and test either directory with:

```bash
cmake --build build-gcc --parallel
ctest --test-dir build-gcc --output-on-failure
```

Replace `build-gcc` with `build-clang` to build and test the Clang configuration.

> Use `-DBYTEPACK_BUILD_TESTS=OFF` in the CMake command to skip building tests.

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

## 4. Troubleshooting

### Catch2 source directory is missing

This test dependency is required on every platform. Initialize it from the repository root:

```bash
git submodule update --init --recursive
```

### Switching compilers or generators fails

CMake caches the compiler, generator, and target architecture in each build directory. Configure a new build directory when changing any of them, such as `build-gcc`, `build-clang`, or `build-vs2026`.

### Visual Studio generator is unavailable

Confirm that the requested Visual Studio or Visual Studio Build Tools version is installed with the **Desktop development with C++** workload. Also confirm that CMake supports the requested generator; in particular, `Visual Studio 18 2026` requires CMake 4.2 or newer.

### Visual Studio cannot find the tests

Build and test the same configuration. For example, pair `cmake --build build --config Release` with `ctest --test-dir build -C Release --output-on-failure`.

### GNU/Linux compiler selection fails

Set `CC` and `CXX` before the first configure and use matching C and C++ compiler versions from the same toolchain. Errors originating deep inside standard-library headers can indicate an incompatible compiler and standard-library combination; use a combination supported by your distribution and configure it in a new build directory.

### macOS invalid active developer path

Install the Command Line Tools with `xcode-select --install`. If the tools are installed but the selected path is wrong, select either the full Xcode installation or the standalone tools:

```bash
sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer
# Or, for standalone Command Line Tools:
sudo xcode-select --switch /Library/Developer/CommandLineTools
```

### macOS dependencies have the wrong architecture

Use dependencies built for the Mac's native architecture, or configure all dependencies and the project with the same `CMAKE_OSX_ARCHITECTURES` value. Check a binary with `file path/to/binary` and check the current Mac with `uname -m`.

# BytePack Examples: ASIO, Qt, and Raw Sockets

The example sources demonstrate integrations with external networking libraries. They are not built by BytePack's root CMake project and are not part of the core unit-test suite. The core BytePack header itself has no third-party runtime dependency.

## Content

- [Boost.Asio](boost_asio)
  - A UDP client sends multiple message types to a server on the same port. The server deserializes each message according to its type and prints it.
- [Qt Sockets](qt_socket)
  - The `UdpCommunicator` class receives UDP messages and sends messages to a specified host and port.
- Raw sockets _(coming soon)_

## macOS Prerequisites

Install the Apple Command Line Tools and the dependencies needed by the example you want to build:

```bash
xcode-select --install
brew install boost
# Additionally, for the Qt example:
brew install qtbase
```

The Homebrew packages are available for both Apple Silicon and Intel macOS. Build every dependency and the example for the same architecture.

## Boost.Asio UDP Example

The Boost example needs Boost.Asio, Boost.System, and the header-only Boost.CRC component. From the BytePack repository root on macOS:

```bash
mkdir -p build/examples
BOOST_PREFIX="$(brew --prefix boost)"

c++ -std=c++20 -pthread \
  -Iinclude -I"$BOOST_PREFIX/include" \
  examples/boost_asio/asio_udp_server.cpp \
  -L"$BOOST_PREFIX/lib" -lboost_system \
  -o build/examples/asio_udp_server

c++ -std=c++20 -pthread \
  -Iinclude -I"$BOOST_PREFIX/include" \
  examples/boost_asio/asio_udp_client.cpp \
  -L"$BOOST_PREFIX/lib" -lboost_system \
  -o build/examples/asio_udp_client
```

Start the server first, then run the client in another terminal:

```bash
./build/examples/asio_udp_server
./build/examples/asio_udp_client
```

Both programs use UDP port `1234` on localhost by default.

## Qt UDP Example

The Qt example needs Qt Core, Qt Network, Boost.CRC, and a Qt-aware build system because `UdpCommunicator` uses the `Q_OBJECT` macro. Enable CMake's automatic Meta-Object Compiler support and link the example to Qt Core and Network. A target integrated into a CMake project rooted at the BytePack repository can use:

```cmake
set(CMAKE_AUTOMOC ON)

find_package(Boost REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_executable(qt_udp
  examples/qt_socket/qt_udp_main.cpp
  examples/qt_socket/UdpCommunicator.h
  examples/qt_socket/messages.h
)

target_include_directories(qt_udp PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
)

target_link_libraries(qt_udp PRIVATE Qt6::Core Qt6::Network)
```

On macOS, pass the Homebrew Qt location if CMake cannot find Qt automatically:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qtbase)"
```

Before running the Qt example, change the destination address and ports in [`qt_udp_main.cpp`](qt_socket/qt_udp_main.cpp) for the local network being tested. The checked-in example sends to `10.0.0.2:55555` and listens on UDP port `1234`.

## Architecture Troubleshooting

An Apple Silicon/Intel mismatch usually appears as a linker error mentioning an incompatible architecture. Check the current host and dependency binaries with:

```bash
uname -m
file path/to/library-or-executable
```

Use native Homebrew packages, or build the application and every linked dependency with the same `CMAKE_OSX_ARCHITECTURES` setting.

## License

Example code in the `examples` folder is provided for demonstration purposes only and is not covered under any specific license terms. Users are free to use it as they wish.

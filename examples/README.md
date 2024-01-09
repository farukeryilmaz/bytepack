# BytePack Examples: ASIO, Qt, and Raw Sockets
## Content
- [Boost.Asio](boost_asio)
    - It currently has client and server examples for UDP protocol. Client sends different types of messages to server
      via same port. Server receives the messages, deserializes them based on their type and prints them to the console.
- [Qt Sockets](qt_socket)
    - It currently has Qt `UdpCommunicator` class that receives messages from a UDP socket and `UdpCommunicator::sendMessage(..` method that
      sends messages to given host and port via UDP socket.
- [Raw Sockets]() _(coming soon)_

## License
Example codes in the `examples` folder are provided for demonstration purposes only and is not covered under any specific license terms. Users are free to use it as they wish.
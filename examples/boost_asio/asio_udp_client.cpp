/*
 * BytePack Example Codes
 *
 * The example codes provided in this file are for demonstration purposes only.
 * These examples are not covered under the BytePack library's main license.
 * Users are free to use, modify, and distribute these example codes as they wish.
 */

#include <iostream>

#include <boost/asio.hpp>
#include <bytepack/bytepack.hpp>
#include "messages.h"

using boost::asio::ip::udp;

class UdpClient
{
public:
  UdpClient(boost::asio::io_context& ioContext, const std::string& serverIP, short serverPort)
    : m_socket(ioContext), m_serverEndpoint(boost::asio::ip::make_address(serverIP), serverPort), m_buffer{},
      m_binaryStream(bytepack::buffer_view(m_buffer))
  {
    m_socket.open(udp::v4());
  }

  void send(const TransformerData& transformerData)
  {
    m_binaryStream.reset();
    m_binaryStream.write(MessageType::TransformerData);
    transformerData.serialize(m_binaryStream);

    auto buffer = m_binaryStream.data();
    m_socket.send_to(boost::asio::buffer(buffer.as<void>(), buffer.size()), m_serverEndpoint);
  }

  void send(const CircuitBreakerStatus& circuitBreakerStatus)
  {
    m_binaryStream.reset();
    m_binaryStream.write(MessageType::CircuitBreakerStatus);
    circuitBreakerStatus.serialize(m_binaryStream);

    auto buffer = m_binaryStream.data();
    m_socket.send_to(boost::asio::buffer(buffer.as<void>(), buffer.size()), m_serverEndpoint);
  }

private:
  udp::socket m_socket;
  udp::endpoint m_serverEndpoint;

  std::array<std::uint8_t, 1024> m_buffer;
  bytepack::binary_stream<> m_binaryStream;
};

int
  main()
{
  std::cout << "Client started!" << std::endl;

  const std::string serverIP = "127.0.0.1";
  constexpr short serverPort = 1234;

  try {
    boost::asio::io_context ioContext;
    UdpClient client(ioContext, serverIP, serverPort);

    // Example of sending a TransformerData message
    TransformerData transformerData{ 123456789,
                                     123456789,
                                     "1234567890123456789",
                                     { 10.0f, 20.0f, 30.0f },
                                     { 1.0f, 2.0f, 3.0f },
                                     0.5f,
                                     25.0f,
                                     50,
                                     1000,
                                     2000,
                                     0x01,
                                     0x0001,
                                     { 0, 0, 0 } };
    client.send(transformerData);

    // Example of sending a CircuitBreakerStatus message
    CircuitBreakerStatus circuitBreakerStatus{ 123456789,  123, true, 2, 123456789, "Overcurrent detected on phase A",
                                               { 0, 0, 0 } };
    client.send(circuitBreakerStatus);

    std::cout << "Data sent!" << std::endl;

    ioContext.run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

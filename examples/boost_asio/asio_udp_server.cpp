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

class UdpServer
{
public:
  UdpServer(boost::asio::io_context& ioContext, short port)
    : m_socket(ioContext, udp::endpoint(udp::v4(), port)), m_recvBuffer{}
  {
    startReceive();
  }

private:
  void startReceive()
  {
    m_socket.async_receive_from(boost::asio::buffer(m_recvBuffer), m_remoteEndpoint,
                                [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                                  if (!ec && bytes_recvd > 0) {
                                    processMessages(m_recvBuffer, bytes_recvd);
                                  }
                                  startReceive();
                                });
  }

  void processMessages(std::array<char, 1024 * 10>& recvBuffer, std::size_t bytes_recvd)
  {
    bytepack::binary_stream stream(bytepack::buffer_view(recvBuffer.data(), bytes_recvd));

    MessageType messageType{};
    stream.read(messageType);

    switch (messageType) {
    case MessageType::TransformerData:
    {
      TransformerData transformerData{};
      const bool isValid = transformerData.deserialize(stream);
      if (isValid) {
        std::cout << "Transformer data:\n"
                  << "  Timestamp: " << transformerData.timestamp << "\n"
                  << "  Identifier: " << transformerData.identifier << "\n"
                  << "  Serial number: " << transformerData.serial_number << "\n"
                  << "  Voltage: (" << transformerData.voltage[0] << ", " << transformerData.voltage[1] << ", "
                  << transformerData.voltage[2] << ")\n"
                  << "  Current: (" << transformerData.current[0] << ", " << transformerData.current[1] << ", "
                  << transformerData.current[2] << ")\n"
                  << "  Power factor: " << transformerData.power_factor << "\n"
                  << "  Temperature: " << transformerData.temperature << "\n"
                  << "  Humidity: " << static_cast<int>(transformerData.humidity) << "\n"
                  << "  Energy consumed: " << transformerData.energyConsumed << "\n"
                  << "  Peak load: " << transformerData.peakLoad << "\n"
                  << "  Status flags: " << static_cast<int>(transformerData.status_flags) << "\n"
                  << "  Alarm codes: " << transformerData.alarm_codes << "\n"
                  << "  Reserved: (" << transformerData.reserved[0] << ", " << transformerData.reserved[1] << ", "
                  << transformerData.reserved[2] << ")\n";

      } else {
        std::cout << "Corrupted transformer data message!\n";
      }
      break;
    }
    case MessageType::CircuitBreakerStatus:
    {
      CircuitBreakerStatus circuitBreakerStatus{};
      const bool isValid = circuitBreakerStatus.deserialize(stream);
      if (isValid) {
        std::cout << "Circuit breaker status:\n"
                  << "  Timestamp: " << circuitBreakerStatus.timestamp << "\n"
                  << "  Circuit ID: " << circuitBreakerStatus.circuit_id << "\n"
                  << "  Is open: " << std::boolalpha << circuitBreakerStatus.is_open << "\n"
                  << "  Trip count: " << static_cast<int>(circuitBreakerStatus.trip_count) << "\n"
                  << "  Last trip time: " << circuitBreakerStatus.last_trip_time << "\n"
                  << "  Fault description: " << circuitBreakerStatus.fault_description << "\n"
                  << "  Reserved: (" << circuitBreakerStatus.reserved[0] << ", " << circuitBreakerStatus.reserved[1]
                  << ", " << circuitBreakerStatus.reserved[2] << ")\n";

      } else {
        std::cout << "Corrupted circuit breaker status message!\n";
      }
      break;
    }
    default:
      std::cout << "Invalid or corrupted message!\n";
    }
  }

private:
  udp::socket m_socket;
  udp::endpoint m_remoteEndpoint;
  std::array<char, 1024 * 10> m_recvBuffer;
};

int
  main()
{
  std::cout << "Server started!" << std::endl;

  constexpr short port = 1234;
  try {
    boost::asio::io_context ioContext;
    UdpServer server(ioContext, port);
    ioContext.run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

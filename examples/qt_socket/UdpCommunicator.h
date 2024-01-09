/*
 * BytePack Example Codes
 *
 * The example codes provided in this file are for demonstration purposes only.
 * These examples are not covered under the BytePack library's main license.
 * Users are free to use, modify, and distribute these example codes as they wish.
 */

#ifndef QT_UDP_UDPCOMMUNICATOR_H
#define QT_UDP_UDPCOMMUNICATOR_H

#include <iostream>
#include <array>

#include <QUdpSocket>
#include <QByteArray>

#include <bytepack/bytepack.hpp>
#include "messages.h"

class UdpCommunicator : public QObject
{
  Q_OBJECT

public:
  explicit UdpCommunicator(quint16 port, QObject* parent = nullptr)
    : QObject(parent), m_socket(new QUdpSocket(this)), m_readBuffer{}, m_readStream(bytepack::buffer_view(m_readBuffer))
  {
    // Bind the socket to listen for incoming messages
    m_socket->bind(QHostAddress::Any, port);
    connect(m_socket, &QUdpSocket::readyRead, this, &UdpCommunicator::onReadyRead);
  }

  void sendMessage(const QHostAddress& address, quint16 port, const CircuitBreakerControl& circuitBreakerControl)
  {
    m_writeStream.reset();
    circuitBreakerControl.serialize(m_writeStream);

    auto serializedData = m_writeStream.data();
    m_socket->writeDatagram(serializedData.as<const char>(), serializedData.ssize(), address, port);
  }

private slots:
  void onReadyRead()
  {
    while (m_socket->hasPendingDatagrams()) {
      // m_socket->pendingDatagramSize();
      QHostAddress sender;
      quint16 senderPort;

      m_socket->readDatagram(m_readBuffer.data(), std::ssize(m_readBuffer), &sender, &senderPort);

      processMessages(sender, senderPort);
    }
  }

private:
  void processMessages(const QHostAddress& sender, quint16 senderPort)
  {
    m_readStream.reset();

    MessageType messageType{};
    m_readStream.read(messageType);

    switch (messageType) {
    case MessageType::TransformerData:
    {
      TransformerData transformerData{};
      const bool isValid = transformerData.deserialize(m_readStream);
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
      const bool isValid = circuitBreakerStatus.deserialize(m_readStream);
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
  QUdpSocket* m_socket;
  bytepack::binary_stream<> m_writeStream{ 1024 * 10 }; // Adjust buffer size as needed

  std::array<char, 1024 * 10> m_readBuffer;
  bytepack::binary_stream<> m_readStream;
};

#endif // QT_UDP_UDPCOMMUNICATOR_H

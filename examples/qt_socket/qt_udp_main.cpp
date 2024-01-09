/*
 * BytePack Example Codes
 *
 * The example codes provided in this file are for demonstration purposes only.
 * These examples are not covered under the BytePack library's main license.
 * Users are free to use, modify, and distribute these example codes as they wish.
 */

#include <iostream>

#include <QCoreApplication>

#include "UdpCommunicator.h"

int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);

  std::cout << "Server started!" << std::endl;

  quint16 localPort = 1234;
  UdpCommunicator communicator(localPort);

  QHostAddress destAddress("10.0.0.2");
  quint16 destPort = 55555;

  CircuitBreakerControl circuitBreakerControl{ 123456789, 12345, true };
  communicator.sendMessage(destAddress, destPort, circuitBreakerControl);

  std::cout << "Data sent!" << std::endl;

  return app.exec();
}

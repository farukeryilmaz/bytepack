/*
 * BytePack Example Codes
 *
 * The example codes provided in this file are for demonstration purposes only.
 * These examples are not covered under the BytePack library's main license.
 * Users are free to use, modify, and distribute these example codes as they wish.
 */

#ifndef ASIO_UDP_MESSAGES_H
#define ASIO_UDP_MESSAGES_H

#include <cstdint>
#include <array>
#include <string>

#include <boost/crc.hpp>

#include <bytepack/bytepack.hpp>

enum class MessageType : uint8_t { TransformerData = 10, CircuitBreakerStatus = 11 };

struct TransformerData
{
  int64_t timestamp;
  uint32_t identifier;
  char serial_number[20];
  float voltage[3]; // Voltage levels for three phases
  float current[3]; // Current levels for three phases
  float power_factor;
  float temperature;
  uint8_t humidity;
  uint32_t energyConsumed; // Total energy consumed
  uint32_t peakLoad;       // Peak load recorded
  uint8_t status_flags;
  uint16_t alarm_codes;
  uint32_t reserved[3]; // Reserved for future use
  uint32_t crc32;

  void serialize(bytepack::binary_stream<>& stream) const
  {
    stream.write(timestamp, identifier, serial_number, voltage, current, power_factor, temperature, humidity,
                 energyConsumed, peakLoad, status_flags, alarm_codes, reserved);
    stream.write(computeCRC32());
  }

  bool deserialize(bytepack::binary_stream<>& stream)
  {
    stream.read(timestamp, identifier, serial_number, voltage, current, power_factor, temperature, humidity,
                energyConsumed, peakLoad, status_flags, alarm_codes, reserved, crc32);
    return verifyCRC32();
  }

private:
  [[nodiscard]] uint32_t computeCRC32() const
  {
    boost::crc_32_type crcComputer;
    crcComputer.process_bytes(&timestamp, sizeof(timestamp));
    crcComputer.process_bytes(&identifier, sizeof(identifier));
    crcComputer.process_bytes(serial_number, sizeof(serial_number));
    crcComputer.process_bytes(voltage, sizeof(voltage));
    crcComputer.process_bytes(current, sizeof(current));
    crcComputer.process_bytes(&power_factor, sizeof(power_factor));
    crcComputer.process_bytes(&temperature, sizeof(temperature));
    crcComputer.process_bytes(&humidity, sizeof(humidity));
    crcComputer.process_bytes(&energyConsumed, sizeof(energyConsumed));
    crcComputer.process_bytes(&peakLoad, sizeof(peakLoad));
    crcComputer.process_bytes(&status_flags, sizeof(status_flags));
    crcComputer.process_bytes(&alarm_codes, sizeof(alarm_codes));
    crcComputer.process_bytes(reserved, sizeof(reserved));
    return crcComputer.checksum();
  }

  [[nodiscard]] bool verifyCRC32() const { return crc32 == computeCRC32(); }
};

struct CircuitBreakerStatus
{
  int64_t timestamp;
  uint32_t circuit_id;
  bool is_open;
  uint8_t trip_count;
  uint32_t last_trip_time;
  char fault_description[80];
  int32_t reserved[3]; // Reserved for future use
  uint32_t crc32;

  void serialize(bytepack::binary_stream<>& stream) const
  {
    stream.write(timestamp, circuit_id, is_open, trip_count, last_trip_time, fault_description, reserved);
    stream.write(computeCRC32());
  }

  [[nodiscard]] bool deserialize(bytepack::binary_stream<>& stream)
  {
    stream.read(timestamp, circuit_id, is_open, trip_count, last_trip_time, fault_description, reserved, crc32);
    return verifyCRC32();
  }

private:
  [[nodiscard]] uint32_t computeCRC32() const
  {
    boost::crc_32_type crcComputer;
    crcComputer.process_bytes(&timestamp, sizeof(timestamp));
    crcComputer.process_bytes(&circuit_id, sizeof(circuit_id));
    crcComputer.process_bytes(&is_open, sizeof(is_open));
    crcComputer.process_bytes(&trip_count, sizeof(trip_count));
    crcComputer.process_bytes(&last_trip_time, sizeof(last_trip_time));
    crcComputer.process_bytes(fault_description, sizeof(fault_description));
    crcComputer.process_bytes(reserved, sizeof(reserved));
    return crcComputer.checksum();
  }

  [[nodiscard]] bool verifyCRC32() const { return crc32 == computeCRC32(); }
};

#endif // ASIO_UDP_MESSAGES_H
#include <catch2/catch.hpp>

#include <bytepack/bytepack.hpp>


TEST_CASE("Struct mixed data serialization and deserialization (big-endian)")
{
	// Example enum and struct

	enum class Type { TYPE_1, TYPE_2, TYPE_3, TYPE_4, TYPE_5 };

	struct GPSData {
		Type type;                    // Type of the data
		std::uint32_t timestamp;      // UNIX timestamp
		double latitude;              // Latitude in degrees
		double longitude;             // Longitude in degrees
		float altitude;               // Altitude in meters
		std::uint16_t numSatellites;  // Number of satellites in view
		char deviceID[16];            // Device ID

		void serialize(bytepack::binary_stream<>& stream) const {
			stream.write(type);
			stream.write(timestamp);
			stream.write(latitude);
			stream.write(longitude);
			stream.write(altitude);
			stream.write(numSatellites);
			stream.write(deviceID);
		}

		void deserialize(bytepack::binary_stream<>& stream) {
			stream.read(type);
			stream.read(timestamp);
			stream.read(latitude);
			stream.read(longitude);
			stream.read(altitude);
			stream.read(numSatellites);
			stream.read(deviceID);
		}
	};

	GPSData gpsData{ Type::TYPE_4, 1701037875, 36.8805426411, 30.6692287448, 123.456f, 12, "GPS-DEVICE-1" };

	bytepack::binary_stream serializationStream(1024);
	gpsData.serialize(serializationStream);
	bytepack::buffer_view buffer = serializationStream.data();

	bytepack::binary_stream deserializationStream(buffer);
	GPSData gpsData_{};
	gpsData_.deserialize(deserializationStream);

	REQUIRE(gpsData.type == gpsData_.type);
	REQUIRE(gpsData.timestamp == gpsData_.timestamp);
	REQUIRE(gpsData.latitude == Approx(gpsData_.latitude).epsilon(1e-9));
	REQUIRE(gpsData.longitude == Approx(gpsData_.longitude).epsilon(1e-9));
	REQUIRE(gpsData.altitude == Approx(gpsData_.altitude).epsilon(1e-2));
	REQUIRE(gpsData.numSatellites == gpsData_.numSatellites);
	REQUIRE_THAT(gpsData.deviceID, Catch::Matchers::Equals(gpsData_.deviceID));
}
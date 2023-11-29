#include <catch2/catch.hpp>

#include <bytepack/bytepack.hpp>


TEST_CASE("Fundamental types (big-endian)")
{
	// Fundamental types to serialize
	std::int8_t num1 = -10;
	std::int16_t num2 = 21;
	std::int32_t num3 = -33;
	std::int64_t num4 = 4345;
	std::uint8_t num5 = 54;
	std::uint16_t num6 = 68;
	std::uint32_t num7 = 70;
	std::uint64_t num8 = 84;
	float num9 = 9.456f;
	double num10 = 10.34526243;
	bool isBool = true;
	signed char char1 = 'f';
	unsigned char char2 = 'E';
	short int num11 = 11;
	unsigned short int num12 = 12;
	int num13 = -123043;
	unsigned int num14 = 14;
	long int num15 = 15;
	unsigned long int num16 = 16;
	long long int num17 = 17;
	unsigned long long int num18 = 18;

	bytepack::binary_stream bstream(1024);

	bstream.write(num1);
	bstream.write(num2);
	bstream.write(num3);
	bstream.write(num4);
	bstream.write(num5);
	bstream.write(num6);
	bstream.write(num7);
	bstream.write(num8);
	bstream.write(num9);
	bstream.write(num10);
	bstream.write(isBool);
	bstream.write(char1);
	bstream.write(char2);
	bstream.write(num11);
	bstream.write(num12);
	bstream.write(num13);
	bstream.write(num14);
	bstream.write(num15);
	bstream.write(num16);
	bstream.write(num17);
	bstream.write(num18);

	// Fundamental types to deserialize
	std::int8_t num1_{};
	std::int16_t num2_{};
	std::int32_t num3_{};
	std::int64_t num4_{};
	std::uint8_t num5_{};
	std::uint16_t num6_{};
	std::uint32_t num7_{};
	std::uint64_t num8_{};
	float num9_{};
	double num10_{};
	bool isBool_{};
	signed char char1_{};
	unsigned char char2_{};
	short int num11_{};
	unsigned short int num12_{};
	int num13_{};
	unsigned int num14_{};
	long int num15_{};
	unsigned long int num16_{};
	long long int num17_{};
	unsigned long long int num18_{};

	bytepack::binary_stream bstream_(bstream.data());

	bstream_.read(num1_);
	bstream_.read(num2_);
	bstream_.read(num3_);
	bstream_.read(num4_);
	bstream_.read(num5_);
	bstream_.read(num6_);
	bstream_.read(num7_);
	bstream_.read(num8_);
	bstream_.read(num9_);
	bstream_.read(num10_);
	bstream_.read(isBool_);
	bstream_.read(char1_);
	bstream_.read(char2_);
	bstream_.read(num11_);
	bstream_.read(num12_);
	bstream_.read(num13_);
	bstream_.read(num14_);
	bstream_.read(num15_);
	bstream_.read(num16_);
	bstream_.read(num17_);
	bstream_.read(num18_);

	REQUIRE(num1 == num1_);
	REQUIRE(num2 == num2_);
	REQUIRE(num3 == num3_);
	REQUIRE(num4 == num4_);
	REQUIRE(num5 == num5_);
	REQUIRE(num6 == num6_);
	REQUIRE(num7 == num7_);
	REQUIRE(num8 == num8_);
	REQUIRE(num9 == Approx(num9_).epsilon(1e-2));
	REQUIRE(num10 == Approx(num10_).epsilon(1e-7));
	REQUIRE(isBool == isBool_);
	REQUIRE(char1 == char1_);
	REQUIRE(char2 == char2_);
	REQUIRE(num11 == num11_);
	REQUIRE(num12 == num12_);
	REQUIRE(num13 == num13_);
	REQUIRE(num14 == num14_);
	REQUIRE(num15 == num15_);
	REQUIRE(num16 == num16_);
	REQUIRE(num17 == num17_);
	REQUIRE(num18 == num18_);
}

TEST_CASE("Fundamental types (little-endian)")
{
	// Fundamental types to serialize
	std::int8_t num1 = -66;
	std::int16_t num2 = 96;
	std::int32_t num3 = -55;
	std::int64_t num4 = -785;
	std::uint8_t num5 = 63;
	std::uint16_t num6 = 112;
	std::uint32_t num7 = 42;
	std::uint64_t num8 = 44;
	float num9 = 758.939f;
	double num10 = 346342.0029315337;
	bool isBool = true;
	signed char char1 = 'D';
	unsigned char char2 = 'o';
	short int num11 = 24;
	unsigned short int num12 = 32;
	int num13 = 863043;
	unsigned int num14 = 64;
	long int num15 = 2154;
	unsigned long int num16 = 434;
	long long int num17 = 61247;
	unsigned long long int num18 = 1348;

	bytepack::binary_stream<std::endian::little> bstream(1024);

	bstream.write(num1);
	bstream.write(num2);
	bstream.write(num3);
	bstream.write(num4);
	bstream.write(num5);
	bstream.write(num6);
	bstream.write(num7);
	bstream.write(num8);
	bstream.write(num9);
	bstream.write(num10);
	bstream.write(isBool);
	bstream.write(char1);
	bstream.write(char2);
	bstream.write(num11);
	bstream.write(num12);
	bstream.write(num13);
	bstream.write(num14);
	bstream.write(num15);
	bstream.write(num16);
	bstream.write(num17);
	bstream.write(num18);

	// Fundamental types to deserialize into
	std::int8_t num1_{};
	std::int16_t num2_{};
	std::int32_t num3_{};
	std::int64_t num4_{};
	std::uint8_t num5_{};
	std::uint16_t num6_{};
	std::uint32_t num7_{};
	std::uint64_t num8_{};
	float num9_{};
	double num10_{};
	bool isBool_{};
	signed char char1_{};
	unsigned char char2_{};
	short int num11_{};
	unsigned short int num12_{};
	int num13_{};
	unsigned int num14_{};
	long int num15_{};
	unsigned long int num16_{};
	long long int num17_{};
	unsigned long long int num18_{};

	auto buffer = bstream.data();
	bytepack::binary_stream<std::endian::little> bstream_(buffer);

	bstream_.read(num1_);
	bstream_.read(num2_);
	bstream_.read(num3_);
	bstream_.read(num4_);
	bstream_.read(num5_);
	bstream_.read(num6_);
	bstream_.read(num7_);
	bstream_.read(num8_);
	bstream_.read(num9_);
	bstream_.read(num10_);
	bstream_.read(isBool_);
	bstream_.read(char1_);
	bstream_.read(char2_);
	bstream_.read(num11_);
	bstream_.read(num12_);
	bstream_.read(num13_);
	bstream_.read(num14_);
	bstream_.read(num15_);
	bstream_.read(num16_);
	bstream_.read(num17_);
	bstream_.read(num18_);

	REQUIRE(num1 == num1_);
	REQUIRE(num2 == num2_);
	REQUIRE(num3 == num3_);
	REQUIRE(num4 == num4_);
	REQUIRE(num5 == num5_);
	REQUIRE(num6 == num6_);
	REQUIRE(num7 == num7_);
	REQUIRE(num8 == num8_);
	REQUIRE(num9 == Approx(num9_).epsilon(1e-2));
	REQUIRE(num10 == Approx(num10_).epsilon(1e-9));
	REQUIRE(isBool == isBool_);
	REQUIRE(char1 == char1_);
	REQUIRE(char2 == char2_);
	REQUIRE(num11 == num11_);
	REQUIRE(num12 == num12_);
	REQUIRE(num13 == num13_);
	REQUIRE(num14 == num14_);
	REQUIRE(num15 == num15_);
	REQUIRE(num16 == num16_);
	REQUIRE(num17 == num17_);
	REQUIRE(num18 == num18_);
}
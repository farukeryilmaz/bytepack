#include <catch2/catch.hpp>

#include <bytepack/bytepack.hpp>


TEST_CASE("Fundamental types (big-endian)")
{
	// Fundamental types to serialize
	float num1 = -3.14159f;
	double num2 = 2.718281828459045;
	bool isBool = false;
	signed char char1 = 'A';
	unsigned char char2 = 'z';
	short int num3 = -567;
	unsigned short int num4 = 789;
	int num5 = 123456;
	unsigned int num6 = 7890;
	long int num7 = -1234567;
	unsigned long int num8 = 9876543;
	long long int num9 = -123456789;
	unsigned long long int num10 = 987654321;

	bytepack::binary_stream bstream(1024);

	bstream.write(num1);
	bstream.write(num2);
	bstream.write(isBool);
	bstream.write(char1);
	bstream.write(char2);
	bstream.write(num3);
	bstream.write(num4);
	bstream.write(num5);
	bstream.write(num6);
	bstream.write(num7);
	bstream.write(num8);
	bstream.write(num9);
	bstream.write(num10);

	// Fundamental types to deserialize
	float num1_{};
	double num2_{};
	bool isBool_{};
	signed char char1_{};
	unsigned char char2_{};
	short int num3_{};
	unsigned short int num4_{};
	int num5_{};
	unsigned int num6_{};
	long int num7_{};
	unsigned long int num8_{};
	long long int num9_{};
	unsigned long long int num10_{};

	auto buffer = bstream.data();
	bytepack::binary_stream bstream_(buffer);

	bstream_.read(num1_);
	bstream_.read(num2_);
	bstream_.read(isBool_);
	bstream_.read(char1_);
	bstream_.read(char2_);
	bstream_.read(num3_);
	bstream_.read(num4_);
	bstream_.read(num5_);
	bstream_.read(num6_);
	bstream_.read(num7_);
	bstream_.read(num8_);
	bstream_.read(num9_);
	bstream_.read(num10_);

	REQUIRE(num1 == Approx(num1_).epsilon(1e-4));
	REQUIRE(num2 == Approx(num2_).epsilon(1e-11));
	REQUIRE(isBool == isBool_);
	REQUIRE(char1 == char1_);
	REQUIRE(char2 == char2_);
	REQUIRE(num3 == num3_);
	REQUIRE(num4 == num4_);
	REQUIRE(num5 == num5_);
	REQUIRE(num6 == num6_);
	REQUIRE(num7 == num7_);
	REQUIRE(num8 == num8_);
	REQUIRE(num9 == num9_);
	REQUIRE(num10 == num10_);
}

TEST_CASE("Fundamental types (little-endian)")
{
	// Fundamental types to serialize
	float num1 = 7.123f;
	double num2 = 8.23456789;
	bool isBool = true;
	signed char char1 = 'x';
	unsigned char char2 = 'Y';
	short int num3 = -9876;
	unsigned short int num4 = 5432;
	int num5 = -12345;
	unsigned int num6 = 67890;
	long int num7 = -234567;
	unsigned long int num8 = 987654;
	long long int num9 = -3456789;
	unsigned long long int num10 = 87654321;

	bytepack::binary_stream<std::endian::little> bstream(1024);

	bstream.write(num1);
	bstream.write(num2);
	bstream.write(isBool);
	bstream.write(char1);
	bstream.write(char2);
	bstream.write(num3);
	bstream.write(num4);
	bstream.write(num5);
	bstream.write(num6);
	bstream.write(num7);
	bstream.write(num8);
	bstream.write(num9);
	bstream.write(num10);

	// Fundamental types to deserialize
	float num1_{};
	double num2_{};
	bool isBool_{};
	signed char char1_{};
	unsigned char char2_{};
	short int num3_{};
	unsigned short int num4_{};
	int num5_{};
	unsigned int num6_{};
	long int num7_{};
	unsigned long int num8_{};
	long long int num9_{};
	unsigned long long int num10_{};

	auto buffer = bstream.data();
	bytepack::binary_stream<std::endian::little> bstream_(buffer);

	bstream_.read(num1_);
	bstream_.read(num2_);
	bstream_.read(isBool_);
	bstream_.read(char1_);
	bstream_.read(char2_);
	bstream_.read(num3_);
	bstream_.read(num4_);
	bstream_.read(num5_);
	bstream_.read(num6_);
	bstream_.read(num7_);
	bstream_.read(num8_);
	bstream_.read(num9_);
	bstream_.read(num10_);

	REQUIRE(num1 == Approx(num1_).epsilon(1e-2));
	REQUIRE(num2 == Approx(num2_).epsilon(1e-7));
	REQUIRE(isBool == isBool_);
	REQUIRE(char1 == char1_);
	REQUIRE(char2 == char2_);
	REQUIRE(num3 == num3_);
	REQUIRE(num4 == num4_);
	REQUIRE(num5 == num5_);
	REQUIRE(num6 == num6_);
	REQUIRE(num7 == num7_);
	REQUIRE(num8 == num8_);
	REQUIRE(num9 == num9_);
	REQUIRE(num10 == num10_);
}
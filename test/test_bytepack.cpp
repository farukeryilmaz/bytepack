#define CATCH_CONFIG_MAIN
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

TEST_CASE("String and sting view (big-endian)")
{
	std::string str = "Hello World!";
	std::string_view str2 = "bytepack library";

	bytepack::binary_stream bstream(1024);

	bstream.write(str);
	bstream.write<short int>(str2);

	std::string str_{};
	std::string str2_{}; // string_view cannot be read into directly (it's a reference)

	auto buffer = bstream.data();
	bytepack::binary_stream bstream_(buffer);

	bstream_.read(str_);
	bstream_.read<short int>(str2_);

	// 8 bytes for the size of the string, 12 bytes for the string itself
	// 2 bytes for the custom size of the string_view, 16 bytes for the string_view itself
	// 38 bytes in total
	REQUIRE(38 == buffer.size());

	REQUIRE(str == str_);
	REQUIRE(str2 == str2_);
}

TEST_CASE("String and sting view (little-endian)")
{
	std::string str = "Hello Bytepack!";
	std::string_view str2 = "bytepack library C++20";

	bytepack::binary_stream<std::endian::little> bstream(64);

	bstream.write(str);
	bstream.write<unsigned long int>(str2);

	std::string str_{};
	std::string str2_{}; // string_view cannot be read into directly (it's a reference)

	auto buffer = bstream.data();
	bytepack::binary_stream<std::endian::little> bstream_(buffer);

	bstream_.read(str_);
	bstream_.read<unsigned long int>(str2_);

	// 8 bytes for the size of the string, 15 bytes for the string itself
	// 4 bytes for the custom size of the string_view, 22 bytes for the string_view itself
	// 49 bytes in total
	REQUIRE(49 == buffer.size());

	REQUIRE(str == str_);
	REQUIRE(str2 == str2_);
}

TEST_CASE("String, sting view and fundamental types mixed (big-endian)")
{
	double num1 = 3754.34526243;
	std::string str = "Hello World!";
	std::uint32_t num2 = 9034;
	std::string_view str2 = "bytepack library";
	float num3 = 193.92834f;

	bytepack::binary_stream bstream(1024);

	bstream.write(num1);
	bstream.write(str);
	bstream.write(num2);
	bstream.write<short int>(str2);
	bstream.write(num3);

	double num1_{};
	std::string str_{};
	std::uint32_t num2_{};
	std::string str2_{}; // string_view cannot be read into directly (it's a reference)
	float num3_{};

	auto buffer = bstream.data();
	bytepack::binary_stream bstream_(buffer);

	bstream_.read(num1_);
	bstream_.read(str_);
	bstream_.read(num2_);
	bstream_.read<short int>(str2_);
	bstream_.read(num3_);

	// 8 bytes for double,
	// 8 bytes for the size of the string, 12 bytes for the string itself,
	// 4 bytes for the uint32_t,
	// 2 bytes for the custom size of the string_view, 16 bytes for the string_view itself,
	// 4 bytes for the float
	// 54 bytes in total
	REQUIRE(54 == buffer.size());

	REQUIRE(num1 == Approx(num1_).epsilon(1e-7));
	REQUIRE(str == str_);
	REQUIRE(num2 == num2_);
	REQUIRE(str2 == str2_);
	REQUIRE(num3 == Approx(num3_).epsilon(1e-4));
}

TEST_CASE("Basic array types (big-endian)")
{
	// basic array types to serialize
	int intArr[50]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
					11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
					21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
					31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
					41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };

	float num = 29845.221612f;

	char charArr[28]{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
					  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
					  'u', 'v', 'w', 'x', 'y', 'z', ' ', '!' };

	bytepack::binary_stream stream(232);

	stream.write(intArr);
	stream.write(num);
	stream.write(charArr);

	// basic array types to deserialize
	int intArr_[50]{};
	float num_{};
	char charArr_[28]{};

	bytepack::binary_stream stream_(stream.data());

	stream_.read(intArr_);
	stream_.read(num_);
	stream_.read(charArr_);

	// 200 bytes for the int array, 4 bytes for the float, 28 bytes for the char array
	// 232 bytes in total
	REQUIRE(232 == stream.data().size());

	REQUIRE(std::equal(std::begin(intArr), std::end(intArr), std::begin(intArr_)));
	REQUIRE(num == Approx(num_).epsilon(1e-5));
	REQUIRE_THAT(charArr, Catch::Matchers::Equals(charArr_));
}

TEST_CASE("Basic array types (little-endian)")
{
	// basic array types to serialize
	int intArr[50]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
					11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
					21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
					31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
					41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };

	char charArr[28]{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
					  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
					  'u', 'v', 'w', 'x', 'y', 'z', ' ', '!' };

	bytepack::binary_stream<std::endian::little> stream(228);

	stream.write(intArr);
	stream.write(charArr);

	// basic array types to deserialize
	int intArr_[50]{};
	char charArr_[28]{};

	bytepack::binary_stream<std::endian::little> stream_(stream.data());

	stream_.read(intArr_);
	stream_.read(charArr_);

	// 200 bytes for the int array, 28 bytes for the char array
	// 228 bytes in total
	REQUIRE(228 == stream.data().size());

	REQUIRE(std::equal(std::begin(intArr), std::end(intArr), std::begin(intArr_)));
	REQUIRE_THAT(charArr, Catch::Matchers::Equals(charArr_));
}
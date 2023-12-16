#include <catch2/catch.hpp>

#include <bytepack/bytepack.hpp>


TEST_CASE("String and string view (big-endian)")
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

TEST_CASE("String and string view (little-endian)")
{
	std::string str = "Hello Bytepack!";
	char str2[] = "bytepack library";
	std::string_view str3 = "bytepack library C++20";

	bytepack::binary_stream<std::endian::little> bstream(74);

	bstream.write(str);
	bstream.write(str2);
	bstream.write<std::uint32_t>(str3);

	std::string str_{};
	std::string str2_{};
	std::string str3_{}; // string_view cannot be read into directly (it's a reference)

	auto buffer = bstream.data();
	bytepack::binary_stream<std::endian::little> bstream_(buffer);

	bstream_.read(str_);
	bstream_.read(str2_, bytepack::StringMode::NullTerminated);
	bstream_.read<std::uint32_t>(str3_);

	// 8 bytes for the size of the string, 15 bytes for the string itself
	// 17 bytes for the string (char[]), (17th byte for the null terminator)
	// 4 bytes for the custom size of the string_view, 22 bytes for the string_view itself
	// 66 bytes in total
	REQUIRE(66 == buffer.size());

	REQUIRE(str == str_);
	REQUIRE(str2 == str2_);
	REQUIRE(str3 == str3_);
}

TEST_CASE("String, string view and fundamental types mixed (big-endian)")
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

TEST_CASE("String and string view null terminated (little-endian)")
{
	std::string str = "Hello BytePack!";
	double num1 = 3754.34526243;

	bytepack::binary_stream<std::endian::little> bstream(32);

	bstream.write(str, bytepack::StringMode::NullTerminated);
	bstream.write(num1);

	std::string str_{};
	double num1_{};

	auto buffer = bstream.data();
	bytepack::binary_stream<std::endian::little> bstream_(buffer);

	bstream_.read(str_, bytepack::StringMode::NullTerminated);
	bstream_.read(num1_);

	// 15 bytes for the string, 1 byte for the null terminator '\0'
	// 8 bytes for double
	// 24 bytes in total
	REQUIRE(24 == buffer.size());
	REQUIRE(str == str_);
	REQUIRE(num1 == Approx(num1_).epsilon(1e-7));
}
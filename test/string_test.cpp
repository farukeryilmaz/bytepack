#include <catch2/catch.hpp>

#include <bytepack/bytepack.hpp>

TEST_CASE("String and string view (big-endian)")
{
  std::string str = "Hello World!";
  std::string_view str2 = "bytepack library";
  std::string str3 = "bytepack";

  bytepack::binary_stream bstream(43);

  bstream.write(str);
  bstream.write<short int>(str2);
  bstream.write<bytepack::StringMode::NullTerm>(str3);

  std::string str_{};
  std::string str2_{}; // string_view cannot be read into directly (it's a reference)
  std::string str3_{};

  auto buffer = bstream.data();
  bytepack::binary_stream bstream_(buffer);

  bstream_.read(str_);
  bstream_.read<short int>(str2_);
  bstream_.read<bytepack::StringMode::NullTerm>(str3_);

  // 4 bytes for the size of the str, 12 bytes for the string itself
  // 2 bytes for the custom size of the str2 string_view, 16 bytes for the string_view itself
  // 8 bytes for the size of the str3, 1 byte for the null terminator '\0'
  // 43 bytes in total
  REQUIRE(43 == buffer.size());

  REQUIRE(str == str_);
  REQUIRE(str2 == str2_);
  REQUIRE(str3 == str3_);
}

TEST_CASE("String and string view (little-endian)")
{
  std::string str = "Hello Bytepack!";
  char str2[] = "bytepack library";
  std::string_view str3 = "bytepack library C++20";

  bytepack::binary_stream<std::endian::little> bstream(74);

  bstream.write(str);
  bstream.write(str2);
  bstream.write<std::uint8_t>(str3);

  std::string str_{};
  std::string str2_{};
  std::string str3_{}; // string_view cannot be read into directly (it's a reference)

  auto buffer = bstream.data();
  bytepack::binary_stream<std::endian::little> bstream_(buffer);

  bstream_.read(str_);
  bstream_.read<bytepack::StringMode::NullTerm>(str2_);
  bstream_.read<std::uint8_t>(str3_);

  // 4 bytes for the size of the string, 15 bytes for the string itself
  // 17 bytes for the string (char[]), (17th byte for the null terminator)
  // 1 bytes for the custom size of the string_view, 22 bytes for the string_view itself
  // 59 bytes in total
  REQUIRE(59 == buffer.size());

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
  // 4 bytes for the size of the string, 12 bytes for the string itself,
  // 4 bytes for the uint32_t,
  // 2 bytes for the custom size of the string_view, 16 bytes for the string_view itself,
  // 4 bytes for the float
  // 50 bytes in total
  REQUIRE(50 == buffer.size());

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

  bstream.write<bytepack::StringMode::NullTerm>(str);
  bstream.write(num1);

  std::string str_{};
  double num1_{};

  auto buffer = bstream.data();
  auto buff = bytepack::buffer_view(buffer.as<void>(), buffer.size());
  bytepack::binary_stream<std::endian::little> bstream_(buff);

  bstream_.read<bytepack::StringMode::NullTerm>(str_);
  bstream_.read(num1_);

  // 15 bytes for the string, 1 byte for the null terminator '\0'
  // 8 bytes for double
  // 24 bytes in total
  REQUIRE(24 == buffer.size());
  REQUIRE(str == str_);
  REQUIRE(num1 == Approx(num1_).epsilon(1e-7));
}

TEST_CASE("String and string view with given size (big-endian)")
{
  // Data to serialize
  std::string str = "Hello BytePack!abcdefgh";
  std::string str2 = "Test string with given size";
  std::string_view str3 = "bytepack library C++20 testabcdefgh";
  std::string_view str4 = "abcdefghijklmnopqrstuvwxyz123456789101112131415161718192021222324252627282930313233343536373"
                          "8394041424344454647484950515253545556575859606162636465666768697071727374757677787980818283";

  // Data to test against
  std::string str_test = "Hello BytePack!";
  std::string str3_test = "bytepack library C++20 test";

  bytepack::binary_stream bstream(100);

  bstream.write<15>(str);
  bstream.write<50>(str2);
  bstream.write<27>(str3);
  bstream.write(str4);

  std::string str_{};
  std::string str2_{};
  std::string str3_{}; // string_view cannot be read into directly (it's a reference)
  std::string str4_{};

  bytepack::binary_stream bstream_(bstream.data());

  bstream_.read<15>(str_);
  bstream_.read<50>(str2_);
  bstream_.read<27>(str3_);
  bstream_.read(str4_);

  // 15 bytes for the string, 50 bytes for the string, 27 bytes for the string_view, 183 bytes for the string.
  // Total is 275 but the buffer size is 100 bytes, so the str4 is not serialized and deserialized.
  // Written 92 bytes in total.
  REQUIRE(92 == bstream.data().size());

  REQUIRE(str_test == str_);
  REQUIRE(str2 == str2_);
  REQUIRE(str3_test == str3_);
  REQUIRE(str4_.empty());
}
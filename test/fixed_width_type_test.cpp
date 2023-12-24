#include <catch2/catch.hpp>

#include <bytepack/bytepack.hpp>

TEST_CASE("Fixed width integer types (big-endian)")
{
  // Fixed width integer types to serialize
  std::int8_t num1 = -7;
  std::int16_t num2 = 152;
  std::int32_t num3 = -2398;
  std::int64_t num4 = 66874;
  std::int_fast8_t num5 = -17;
  std::int_fast16_t num6 = 345;
  std::int_fast32_t num7 = -4512;
  std::int_fast64_t num8 = 987654;
  std::int_least8_t num9 = -31;
  std::int_least16_t num10 = 4231;
  std::int_least32_t num11 = -82456;
  std::int_least64_t num12 = 12345678;
  std::intmax_t num13 = -999;
  std::intptr_t num14 = 5678;
  std::uint8_t num15 = 45;
  std::uint16_t num16 = 6543;
  std::uint32_t num17 = 123456;
  std::uint64_t num18 = 9876543;
  std::uint_fast8_t num19 = 89;
  std::uint_fast16_t num20 = 12345;
  std::uint_fast32_t num21 = 987654;
  std::uint_fast64_t num22 = 12345678;
  std::uint_least8_t num23 = 12;
  std::uint_least16_t num24 = 5432;
  std::uint_least32_t num25 = 8765432;
  std::uint_least64_t num26 = 87654321;
  std::uintmax_t num27 = 1234567890;
  std::uintptr_t num28 = 87654321;

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
  bstream.write(num11);
  bstream.write(num12);
  bstream.write(num13);
  bstream.write(num14);
  bstream.write(num15);
  bstream.write(num16);
  bstream.write(num17);
  bstream.write(num18);
  bstream.write(num19);
  bstream.write(num20);
  bstream.write(num21);
  bstream.write(num22);
  bstream.write(num23);
  bstream.write(num24);
  bstream.write(num25);
  bstream.write(num26);
  bstream.write(num27);
  bstream.write(num28);

  // Fixed width integer types to deserialize
  std::int8_t num1_{};
  std::int16_t num2_{};
  std::int32_t num3_{};
  std::int64_t num4_{};
  std::int_fast8_t num5_{};
  std::int_fast16_t num6_{};
  std::int_fast32_t num7_{};
  std::int_fast64_t num8_{};
  std::int_least8_t num9_{};
  std::int_least16_t num10_{};
  std::int_least32_t num11_{};
  std::int_least64_t num12_{};
  std::intmax_t num13_{};
  std::intptr_t num14_{};
  std::uint8_t num15_{};
  std::uint16_t num16_{};
  std::uint32_t num17_{};
  std::uint64_t num18_{};
  std::uint_fast8_t num19_{};
  std::uint_fast16_t num20_{};
  std::uint_fast32_t num21_{};
  std::uint_fast64_t num22_{};
  std::uint_least8_t num23_{};
  std::uint_least16_t num24_{};
  std::uint_least32_t num25_{};
  std::uint_least64_t num26_{};
  std::uintmax_t num27_{};
  std::uintptr_t num28_{};

  // reads from the same stream which has the buffer of the written data
  bstream.read(num1_);
  bstream.read(num2_);
  bstream.read(num3_);
  bstream.read(num4_);
  bstream.read(num5_);
  bstream.read(num6_);
  bstream.read(num7_);
  bstream.read(num8_);
  bstream.read(num9_);
  bstream.read(num10_);
  bstream.read(num11_);
  bstream.read(num12_);
  bstream.read(num13_);
  bstream.read(num14_);
  bstream.read(num15_);
  bstream.read(num16_);
  bstream.read(num17_);
  bstream.read(num18_);
  bstream.read(num19_);
  bstream.read(num20_);
  bstream.read(num21_);
  bstream.read(num22_);
  bstream.read(num23_);
  bstream.read(num24_);
  bstream.read(num25_);
  bstream.read(num26_);
  bstream.read(num27_);
  bstream.read(num28_);

  REQUIRE(num1 == num1_);
  REQUIRE(num2 == num2_);
  REQUIRE(num3 == num3_);
  REQUIRE(num4 == num4_);
  REQUIRE(num5 == num5_);
  REQUIRE(num6 == num6_);
  REQUIRE(num7 == num7_);
  REQUIRE(num8 == num8_);
  REQUIRE(num9 == num9_);
  REQUIRE(num10 == num10_);
  REQUIRE(num11 == num11_);
  REQUIRE(num12 == num12_);
  REQUIRE(num13 == num13_);
  REQUIRE(num14 == num14_);
  REQUIRE(num15 == num15_);
  REQUIRE(num16 == num16_);
  REQUIRE(num17 == num17_);
  REQUIRE(num18 == num18_);
  REQUIRE(num19 == num19_);
  REQUIRE(num20 == num20_);
  REQUIRE(num21 == num21_);
  REQUIRE(num22 == num22_);
  REQUIRE(num23 == num23_);
  REQUIRE(num24 == num24_);
  REQUIRE(num25 == num25_);
  REQUIRE(num26 == num26_);
  REQUIRE(num27 == num27_);
  REQUIRE(num28 == num28_);
}
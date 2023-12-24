#include <catch2/catch.hpp>

#include <bytepack/bytepack.hpp>

TEST_CASE("Basic array types (big-endian)")
{
  // basic array types to serialize
  int intArr[50]{ 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };

  float num = 29845.221612f;

  char charArr[28]{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '!' };

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

  for (size_t i = 0; i < 28; i++) {
    REQUIRE(charArr[i] == charArr_[i]);
  }
}

TEST_CASE("Basic array types (little-endian)")
{
  // basic array types to serialize
  int intArr[50]{ 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };

  char charArr[28]{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '!' };

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

  for (size_t i = 0; i < 28; i++) {
    REQUIRE(charArr[i] == charArr_[i]);
  }
}

TEST_CASE("Basic array types - stack buffer (big-endian)")
{
  // basic array types to serialize
  int intArr[15]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

  char charArr[9]{ 'z', 'y', 'x', 'w', 'v', 'u', 't', 's', 'r' };

  double num = 19251293.1923421;

  std::array<char, 232> data{};
  bytepack::binary_stream stream(bytepack::buffer_view{ data });

  stream.write(intArr);
  stream.write(charArr);
  stream.write(num);

  // basic array types to deserialize
  int intArr_[15]{};
  char charArr_[9]{};
  double num_{};

  bytepack::binary_stream stream_(bytepack::buffer_view{ data });

  stream_.read(intArr_);
  stream_.read(charArr_);
  stream_.read(num_);

  // 60 bytes for the int array, 9 bytes for the char array, 8 bytes for the double
  // 77 bytes in total
  REQUIRE(77 == stream.data().size());

  REQUIRE(std::equal(std::begin(intArr), std::end(intArr), std::begin(intArr_)));
  for (size_t i = 0; i < 9; i++) {
    REQUIRE(charArr[i] == charArr_[i]);
  }
  REQUIRE(num == Approx(num_).epsilon(1e-5));
}
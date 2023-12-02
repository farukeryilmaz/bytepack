#include <catch2/catch.hpp>

#include <bytepack/bytepack.hpp>


TEST_CASE("Std containers - vector test (big-endian)")
{
	// std::vector to serialize
	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	std::vector<std::uint16_t> vec2 = { 11, 22, 33, 44, 55 };
	std::vector<std::int64_t> vec3 = { -111, 222, -333, 444, -555 };
	std::vector<std::uint32_t> vec4 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
										11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
										21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
										31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
										41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
										51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
										61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
										71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
										81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
										91, 92, 93, 94, 95, 96, 97, 98, 99, 100 };
	std::vector<std::uint32_t> vec4_test = { vec4.begin(), vec4.begin() + 36 };

	bytepack::binary_stream bstream(1024);

	bstream.write(vec);
	bstream.write<std::uint8_t>(vec2);
	bstream.write(vec3, 5);
	bstream.write(vec4, 36);

	// std::vector to deserialize
	std::vector<int> vec_{};
	std::vector<std::uint16_t> vec2_{};
	std::vector<std::int64_t> vec3_{};
	std::vector<std::uint32_t> vec4_{};

	auto buffer = bstream.data();
	bytepack::binary_stream bstream_(buffer);

	bstream_.read(vec_);
	bstream_.read<std::uint8_t>(vec2_);
	bstream_.read(vec3_, 5);
	bstream_.read(vec4_, 36);

	REQUIRE(vec == vec_);
	REQUIRE(vec2 == vec2_);
	REQUIRE(vec3 == vec3_);
	REQUIRE(vec4_test == vec4_);
}
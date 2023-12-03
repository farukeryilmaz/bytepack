/**
MIT License

Copyright (c) 2023 Faruk Eryilmaz and contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

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

TEST_CASE("Std containers - array test (big-endian)")
{
	// std::array to serialize
	std::array<int, 5> arr = { 1, 2, 3, 4, 5 };
	std::array<std::uint16_t, 5> arr2 = { 11, 22, 33, 44, 55 };
	std::array<std::int64_t, 5> arr3 = { -111, 222, -333, 444, -555 };
	std::array<double, 5> arr4 = { 1.1, 2.2, 3.3, 4.4, 5.5 };

	bytepack::binary_stream bstream(1024);

	bstream.write(arr);
	bstream.write(arr2);
	bstream.write(arr3);
	bstream.write(arr4);

	// std::array to deserialize
	std::array<int, 5> arr_{};
	std::array<std::uint16_t, 5> arr2_{};
	std::array<std::int64_t, 5> arr3_{};
	std::array<double, 5> arr4_{};

	auto buffer = bstream.data();
	bytepack::binary_stream bstream_(buffer);

	bstream_.read(arr_);
	bstream_.read(arr2_);
	bstream_.read(arr3_);
	bstream_.read(arr4_);

	REQUIRE(arr == arr_);
	REQUIRE(arr2 == arr2_);
	REQUIRE(arr3 == arr3_);

	for (size_t i = 0; i < arr4.size(); ++i) {
		REQUIRE(arr4[i] == Approx(arr4_[i]).epsilon(1e-2));
	}
}

TEST_CASE("Std containers - vector/array mixed test (big-endian)")
{
	// std::array and std::vector to serialize
	std::vector<int> vec = { -23, -54, 7772, -2345, 10023 };
	std::vector<std::uint16_t> vec2 = { 11, 22, 33, 44, 55 };
	std::array<float, 6> arr = { 120.20245f, -23.234f, 0.0001f, 1.234f, 100.0f, -100.0f };
	std::array<std::uint8_t, 3> arr2 = { 155, 200, 255 };
	std::vector<std::int64_t> vec3 = { -1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
										11, 12, 13, 14, 15, 16, 17, 18, 19, -20,
										21, -22, 23, 24, -25, 26, 27, 28, 29, 30,
										31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
										41, 42, 43, -44, 45, 46, 47, 48, 49, 50,
										51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
										61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
										71, 72, 73, 74, 75, -76, 77, 78, 79, 80,
										81, -82, 83, 84, 85, 86, 87, 88, -89, 90,
										91, 92, -93, 94, 95, 96, 97, -98, 99, 100 };

	bytepack::binary_stream bstream(1024);

	// write vector with given size eliminating the need to write the size metadata
	bstream.write(vec, 5);

	bstream.write(vec2);
	bstream.write(arr);
	bstream.write(arr2);
	bstream.write<std::uint8_t>(vec3);

	// std::array and std::vector to deserialize
	std::array<int, 5> arr_vec_{};
	std::vector<std::uint16_t> vec2_{};
	std::array<float, 6> arr_{};
	std::array<std::uint8_t, 3> arr2_{};
	std::vector<std::int64_t> vec3_{};

	auto buffer = bstream.data();
	bytepack::binary_stream bstream_(buffer);

	// read vector as array with exact size
	bstream_.read(arr_vec_);

	bstream_.read(vec2_);
	bstream_.read(arr_);
	bstream_.read(arr2_);
	bstream_.read<std::uint8_t>(vec3_);

	for (size_t i = 0; i < vec.size(); ++i) {
		REQUIRE(vec[i] == arr_vec_[i]);
	}

	REQUIRE(vec2 == vec2_);

	for (size_t i = 0; i < arr.size(); ++i) {
		REQUIRE(arr[i] == Approx(arr_[i]).epsilon(1e-3));
	}
	for (size_t i = 0; i < arr2.size(); ++i) {
		REQUIRE(arr2[i] == arr2_[i]);
	}

	REQUIRE(vec3 == vec3_);
}


TEST_CASE("Std containers - vector/array mixed test (little-endian)")
{
	// std::array and std::vector to serialize
	std::vector<int> vec = { 963, -334, -72, -965675, 234 };
	std::vector<std::uint16_t> vec2 = { 87, 13, 0, 65535, 65534 };
	std::array<float, 6> arr = { 97.5645f, -23.234f, 0.0001f, -1.234f, 1000.0f, -100.0f };
	std::array<std::uint8_t, 3> arr2 = { 155, 200, 255 };
	std::vector<std::int64_t> vec3 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, -10,
										-11, 12, 13, 14, 15, 16, 17, 18, 19, -20,
										21, -22, 23, 24, 25, 26, 27, 28, -29, 30,
										31, 32, -33, 34, 35, 36, 37, -38, 39, 40,
										41, 42, 43, -44, 45, 46, -47, 48, 49, 50,
										-51, 52, 53, 54, -55, -56, 57, 58, 59, -60,
										61, 62, 63, 64, -65, -66, 67, 68, 69, 70,
										71, 72, 73, -74, 75, 76, -77, 78, 79, 80,
										81, 82, -83, 84, 85, 86, 87, -88, 89, 90,
										91, -92, 93, 94, 95, 96, 97, 98, -99, 100 };

	bytepack::binary_stream<std::endian::little> bstream(1024);

	// write vector with given size eliminating the need to write the size metadata
	bstream.write(vec, 5);

	bstream.write(vec2);
	bstream.write(arr);
	bstream.write(arr2);
	bstream.write<std::uint8_t>(vec3);

	// std::array and std::vector to deserialize
	std::array<int, 5> arr_vec_{};
	std::vector<std::uint16_t> vec2_{};
	std::array<float, 6> arr_{};
	std::array<std::uint8_t, 3> arr2_{};
	std::vector<std::int64_t> vec3_{};

	auto buffer = bstream.data();
	bytepack::binary_stream<std::endian::little> bstream_(buffer);

	// read vector as array with exact size
	bstream_.read(arr_vec_);

	bstream_.read(vec2_);
	bstream_.read(arr_);
	bstream_.read(arr2_);
	bstream_.read<std::uint8_t>(vec3_);

	for (size_t i = 0; i < vec.size(); ++i) {
		REQUIRE(vec[i] == arr_vec_[i]);
	}

	REQUIRE(vec2 == vec2_);

	for (size_t i = 0; i < arr.size(); ++i) {
		REQUIRE(arr[i] == Approx(arr_[i]).epsilon(1e-3));
	}
	for (size_t i = 0; i < arr2.size(); ++i) {
		REQUIRE(arr2[i] == arr2_[i]);
	}

	REQUIRE(vec3 == vec3_);
}

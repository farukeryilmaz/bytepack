/**
 * @file bytepack.hpp
 * @brief Provides functionality for binary serialization and deserialization.
 */

#ifndef BYTEPACK_HPP
#define BYTEPACK_HPP

#include <bit>
#include <string>
#include <cstring> 
#include <cstddef>
#include <cstdint>
#include <algorithm>

namespace bytepack {

	/**
	 * @class buffer
	 * @brief A mutable class that represents a buffer for holding binary data.
	 *
	 * The buffer class encapsulates a pointer to data and its size. It is designed to
	 * provide an interface to access binary data without owning it. This class does not
	 * manage the lifetime of the underlying data.
	 */
	class buffer {
	public:
		template<typename T, std::size_t N>
		constexpr buffer(T(&array)[N]) noexcept
			: data_{ array }, size_{ N * sizeof(T) },
			ssize_{ to_ssize(N * sizeof(T)) } {}

		template<typename T>
		buffer(T* ptr, std::size_t size) noexcept
			: data_{ static_cast<void*>(ptr) }, size_{ size * sizeof(T) },
			ssize_{ to_ssize(size * sizeof(T)) } {}

		buffer(std::string& str) noexcept
			: data_{ str.data() }, size_{ str.size() }, ssize_{ to_ssize(str.size()) } {}

		/**
		 * Templated method to get data as the specified type.
		 *
		 * Warning: This method does not provide type safety and assumes the user knows the correct type of the data.
		 * Users must ensure that the type T they cast to matches the actual type of data in the buffer.
		 *
		 * Note on Alignment:
		 * The method may cause undefined behavior if the data is not correctly aligned for the requested type.
		 * For instance, accessing a buffer containing char data as an int* or double* might cause issues.
		 *
		 * Note on Error Checking:
		 * This method does not perform size checks. Users should ensure that the buffer is large enough
		 * to contain the data of type T. Accessing beyond the bounds of the buffer can lead to undefined behavior.
		 *
		 * @tparam T The type to which the buffer's data will be cast.
		 * @return Pointer to the buffer's data cast to the specified type.
		 */
		template<typename T>
		[[nodiscard]] constexpr T* as() const noexcept {
			// if (size_ < sizeof(T)) {
			// 	throw std::runtime_error("Buffer size is too small for this type");
			// }
			return static_cast<T*>(data_);
		}

		[[nodiscard]] constexpr std::size_t size() const noexcept { return size_; }

		[[nodiscard]] constexpr std::ptrdiff_t ssize() const noexcept { return ssize_; }

		[[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }

		[[nodiscard]] constexpr operator bool() const noexcept { return data_ && size_ > 0; }

	private:
		constexpr std::ptrdiff_t to_ssize(const std::size_t size) const noexcept {
			using R = std::common_type_t<std::ptrdiff_t, std::make_signed_t<decltype(size)>>;
			return static_cast<R>(size);
		}

	private:
		void* data_;
		std::size_t size_;
		std::ptrdiff_t ssize_; // signed
	};

	template<typename T>
	concept NetworkSerializableBasic = std::is_trivially_copyable_v<T>
		&& std::is_standard_layout_v<T>
		&& std::is_class_v<T> == false;

	enum class endian {
		little = std::endian::little,
		big = std::endian::big,
		native = std::endian::native,
		network = std::endian::big
	};

	/**
	 * @class binary_stream
	 * @brief A class for serializing and deserializing binary data with support for different endianness.
	 * It supports handling both internally allocated buffers and user-supplied buffers.
	 *
	 * @tparam BufferEndian The endianness to use for serialization and deserialization.
	 *                      Defaults to network byte order (big-endian).
	 */
	template<bytepack::endian BufferEndian = bytepack::endian::network>
	class binary_stream {

	public:
		explicit binary_stream(const std::size_t buffer_size) noexcept
			: buffer_{ new std::uint8_t[buffer_size]{}, buffer_size }, owns_buffer_{ true },
			current_serialize_index_{ 0 }, current_deserialize_index_{ 0 }
		{
			// TODO: consider lazy initialization (lazy-load) for `buffer_`. It might come in useful in some cases.
		}

		explicit binary_stream(const bytepack::buffer& buffer) noexcept
			: buffer_{ buffer }, owns_buffer_{ false }, current_serialize_index_{ 0 },
			current_deserialize_index_{ 0 }
		{}

		~binary_stream() noexcept {
			if (owns_buffer_) {
				delete[] buffer_.as<std::uint8_t>();
			}
		}

		binary_stream(const binary_stream&) = delete;
		binary_stream& operator=(const binary_stream&) = delete;
		binary_stream(binary_stream&&) = delete;
		binary_stream& operator=(binary_stream&&) = delete;

		constexpr void reset() noexcept
		{
			current_serialize_index_ = 0;
			current_deserialize_index_ = 0;
		}

		[[nodiscard]] bytepack::buffer data() const noexcept {
			return { buffer_.as<std::uint8_t>(), current_serialize_index_ };
		}

		template<NetworkSerializableBasic T>
		bool write(const T& value) noexcept {
			if (buffer_.size() < (current_serialize_index_ + sizeof(T))) {
				return false;
			}

			std::memcpy(buffer_.as<std::uint8_t>() + current_serialize_index_, &value, sizeof(T));

			if constexpr (BufferEndian != bytepack::endian::native && sizeof(T) > 1) {
				std::ranges::reverse(buffer_.as<std::uint8_t>() + current_serialize_index_,
					buffer_.as<std::uint8_t>() + current_serialize_index_ + sizeof(T));
			}

			current_serialize_index_ += sizeof(T);

			return true;
		}

		template<typename SizeType = std::size_t>
		bool write(const std::string& value) noexcept {
			static_assert(std::is_integral_v<SizeType>, "SizeType must be an integral type!");

			const SizeType str_length = static_cast<SizeType>(value.length());
			if ((std::is_signed_v<SizeType> && str_length < 0)
				|| static_cast<std::size_t>(str_length) != value.length()) {
				// Overflow or incorrect size type
				return false;
			}

			if (buffer_.size() < (current_serialize_index_ + sizeof(SizeType) + str_length)) {
				// String data and its length field cannot fit in the remaining buffer space
				return false;
			}

			// Write string length field first (before the string data)
			if (write(str_length) == false) {
				return false;
			}

			std::memcpy(buffer_.as<std::uint8_t>() + current_serialize_index_, value.data(), value.length());
			current_serialize_index_ += value.length();

			return true;
		}

	private:
		bytepack::buffer buffer_;

		// Flag to indicate buffer ownership
		// TODO: Consider alternative ownership models and design to handle external buffers
		bool owns_buffer_;

		std::size_t current_serialize_index_;
		std::size_t current_deserialize_index_;
	};

} // namespace bytepack

#endif // BYTEPACK_HPP
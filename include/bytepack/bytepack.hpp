#ifndef BYTEPACK_HPP
#define BYTEPACK_HPP

#include <bit>
#include <string>
#include <cstring> 
#include <cstddef>
#include <cstdint>

namespace bytepack {

	class buffer {
	public:
		template<typename T, std::size_t N>
		buffer(T(&array)[N])
			: data_{ array }, size_{ N * sizeof(T) },
			ssize_{ get_ssize(N * sizeof(T)) } {}

		template<typename T>
		buffer(T* ptr, std::size_t size)
			: data_{ static_cast<void*>(ptr) }, size_{ size * sizeof(T) },
			ssize_{ get_ssize(size * sizeof(T)) } {}

		buffer(std::string& str)
			: data_{ str.data() }, size_{ str.size() }, ssize_{ get_ssize(str.size()) } {}

		void* data() const { return data_; }

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
		T* as() const {
			// if (size_ < sizeof(T)) {
			// 	throw std::runtime_error("Buffer size is too small for this type");
			// }
			return static_cast<T*>(data_);
		}

		std::size_t size() const { return size_; }

		std::ptrdiff_t ssize() const { return size_; }

		operator bool() const {
			return data_ && size_ > 0;
		}

	private:
		std::ptrdiff_t get_ssize(const std::size_t size)
		{
			if (size <= static_cast<std::size_t>(std::numeric_limits<std::ptrdiff_t>::max())) {
				return static_cast<ptrdiff_t>(size_);
			}
			return std::numeric_limits<std::ptrdiff_t>::max();
		}

	private:
		void* data_;
		std::size_t size_;		// unsigned
		std::ptrdiff_t ssize_;	// signed
	};

	template<std::endian TargetEndian = std::endian::big>
	class binary_stream {

	public:
		explicit binary_stream(const std::size_t buffer_size) noexcept
			: system_endianness_{ std::endian::native }, target_endianness_{ TargetEndian },
			buffer_{ new std::uint8_t[buffer_size]{}, buffer_size }, owns_buffer_{ true }
		{
			// TODO: consider lazy initialization (lazy-load) for `buffer_`. It might come in useful in some cases.
		}

		explicit binary_stream(const bytepack::buffer& buffer) noexcept
			: system_endianness_{ std::endian::native }, target_endianness_{ TargetEndian },
			buffer_{ buffer }, owns_buffer_{ false }
		{}

		~binary_stream() noexcept {
			if (owns_buffer_) {
				delete[] buffer_.as<std::uint8_t>();
			}
		}

		bytepack::buffer data() const {
			return buffer_;
		}

		binary_stream(const binary_stream&) = delete;
		binary_stream& operator=(const binary_stream&) = delete;
		binary_stream(binary_stream&&) = delete;
		binary_stream& operator=(binary_stream&&) = delete;

	private:
		std::endian system_endianness_;
		std::endian target_endianness_;

		bytepack::buffer buffer_;

		// Flag to indicate buffer ownership
		// TODO: Consider alternative ownership models and design to handle external buffers
		bool owns_buffer_;
	};

}

#endif // BYTEPACK_HPP
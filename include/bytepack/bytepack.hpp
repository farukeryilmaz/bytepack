#ifndef BYTEPACK_HPP
#define BYTEPACK_HPP

#include <bit>

namespace bytepack {

	class buffer final
	{
	public:
		explicit buffer() noexcept
			: data_{ nullptr }, size_{ 0 }
		{};

		explicit buffer(const int size) {
			size_ = size;
			data_ = new char[static_cast<size_t>(size_)] {};
		}

		~buffer() noexcept {
			delete[] static_cast<char*>(data_);
		}

		buffer(const buffer& other)
			: size_{ other.size_ }, data_{ new char[other.size_] } {
			std::memcpy(data_, other.data_, size_);
		}

		buffer(buffer&& other) noexcept
			: data_{ other.data_ }, size_{ other.size_ } {
			other.data_ = nullptr;
			other.size_ = 0;
		}

		buffer& operator=(const buffer& other) {
			if (this != &other) {
				delete[] static_cast<char*>(data_);
				size_ = other.size_;
				data_ = new char[size_];
				std::memcpy(data_, other.data_, size_);
			}
			return *this;
		}

		buffer& operator=(buffer&& other) noexcept {
			if (this != &other) {
				delete[] data_;
				data_ = other.data_;
				size_ = other.size_;
				other.data_ = nullptr;
				other.size_ = 0;
			}
			return *this;
		}

		inline void* data() const { return data_; }

		inline int size() const { return size_; }

	private:
		void* data_;
		int size_;
	};

	template<int BufferSize, std::endian OutputEndian = std::endian::native>
	class binary_stream final
	{
		static_assert(("Buffer size must be greater than 0 (zero)", BufferSize > 0));

	public:
		constexpr explicit binary_stream() noexcept
			: system_endianness_{ std::endian::native }, target_endianness_{ OutputEndian }
		{
			buffer_ = buffer(BufferSize);
		}

		const bytepack::buffer& data() const { return buffer_; }

	private:
		std::endian system_endianness_;
		std::endian target_endianness_;

		// TODO: consider lazy initialization (lazy-load). It might come in useful in some cases.
		bytepack::buffer buffer_;
	};

}

#endif // BYTEPACK_HPP
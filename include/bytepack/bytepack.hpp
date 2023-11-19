#ifndef BYTEPACK_HPP
#define BYTEPACK_HPP

#include <bit>
#include <string>
#include <cstring> 
#include <cstddef>

namespace bytepack {

	class buffer final
	{
	public:
		explicit buffer() noexcept
			: data_{ nullptr }, size_{ 0 }, ssize_{ 0 }
		{};

		explicit buffer(const std::size_t size) noexcept
			: data_{ new char[size] {} }, size_{ size }
		{
			if (size <= static_cast<std::size_t>(std::numeric_limits<std::ptrdiff_t>::max())) {
				ssize_ = static_cast<ptrdiff_t>(size);
			}
			else {
				ssize_ = std::numeric_limits<std::ptrdiff_t>::max();
			}
		}

		~buffer() noexcept {
			delete[] static_cast<char*>(data_);
		}

		buffer(const buffer& other)
			: data_{ new char[other.size_] }, size_{ other.size_ }, ssize_{ other.ssize_ } {
			std::memcpy(data_, other.data_, size_);
		}

		buffer(buffer&& other) noexcept
			: data_{ other.data_ }, size_{ other.size_ }, ssize_{ other.ssize_ } {
			other.data_ = nullptr;
			other.size_ = 0;
			other.ssize_ = 0;
		}

		buffer& operator=(const buffer& other) {
			if (this != &other) {
				delete[] static_cast<char*>(data_);
				size_ = other.size_;
				ssize_ = other.ssize_;
				data_ = new char[size_];
				std::memcpy(data_, other.data_, size_);
			}
			return *this;
		}

		buffer& operator=(buffer&& other) noexcept {
			if (this != &other) {
				delete[] static_cast<char*>(data_);
				data_ = other.data_;
				size_ = other.size_;
				ssize_ = other.ssize_;
				other.data_ = nullptr;
				other.size_ = 0;
				other.ssize_ = 0;
			}
			return *this;
		}

		void* data() const { return data_; }

		std::size_t size() const { return size_; }

		std::ptrdiff_t ssize() const { return ssize_; }

		operator bool() const {
			return data_ && size_ > 0;
		}

	private:
		void* data_;
		std::size_t size_;		// unsigned
		std::ptrdiff_t ssize_;	// signed
	};

	class buffer_view final {
	public:
		buffer_view(const buffer& buf)
			: data_{ buf.data() }, size_{ buf.size() }, ssize_{ buf.ssize() }
		{}

		template<typename T, size_t N>
		buffer_view(T(&array)[N])
			: data_{ array }, size_{ N * sizeof(T) },
			ssize_{ get_ssize(N * sizeof(T)) } {}

		template<typename T>
		buffer_view(T* ptr, size_t size)
			: data_{ static_cast<void*>(ptr) }, size_{ size * sizeof(T) },
			ssize_{ get_ssize(size * sizeof(T)) } {}

		buffer_view(std::string& str)
			: data_{ str.data() }, size_{ str.size() }, ssize_{ get_ssize(str.size()) } {}

		void* data() const { return data_; }

		template<typename T>
		T as() const {
			return static_cast<T>(data_);
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

	template<int BufferSize, std::endian OutputEndian = std::endian::native>
	class binary_stream final {
		static_assert(("Buffer size must be greater than 0 (zero)", BufferSize > 0));

	public:
		constexpr explicit binary_stream() noexcept
			: system_endianness_{ std::endian::native }, target_endianness_{ OutputEndian }
		{
			buffer_ = bytepack::buffer(static_cast<std::size_t>(BufferSize));
		}

		bytepack::buffer_view data() const {
			return buffer_;
		}

	private:
		std::endian system_endianness_;
		std::endian target_endianness_;

		// TODO: consider lazy initialization (lazy-load). It might come in useful in some cases.
		bytepack::buffer buffer_;
	};

}

#endif // BYTEPACK_HPP
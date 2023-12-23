/*
 * MIT License
 *
 * Copyright (c) 2023 Faruk Eryilmaz
 *
 * Contact: faruk [at] farukeryilmaz [dot] com
 * GitHub: https://github.com/farukeryilmaz/bytepack
 *
 * Licensed under the MIT License. See accompanying LICENSE file
 * or copy at https://opensource.org/licenses/MIT
 */

/**
 * @file bytepack.hpp
 * @brief Simple C++ binary serialization library with endianness control, avoiding custom encoding.
 */

#ifndef BYTEPACK_HPP
#define BYTEPACK_HPP

#include <bit>
#include <string>
#include <string_view>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <concepts>
#include <optional>
#include <vector>
#include <array>

namespace bytepack {

template<typename T>
concept SerializableBuffer =
  std::is_fundamental_v<T> && std::is_pointer_v<T> == false && std::is_reference_v<T> == false;

/**
 * @class buffer_view
 * @brief A mutable class that represents a buffer for holding binary data.
 *
 * The buffer_view class encapsulates a pointer to data and its size. It is designed to
 * provide an interface to access binary data without owning it. This class does not
 * manage the lifetime of the underlying data.
 */
class buffer_view
{
public:
  template<typename T, std::size_t N>
  requires SerializableBuffer<T>
  explicit constexpr buffer_view(T (&array)[N]) noexcept
    : data_{ array }, size_{ N * sizeof(T) }, ssize_{ to_ssize(N * sizeof(T)) }
  {}

  template<typename T>
  requires SerializableBuffer<T>
  explicit constexpr buffer_view(T* ptr, std::size_t size) noexcept
    : data_{ static_cast<void*>(ptr) }, size_{ size * sizeof(T) }, ssize_{ to_ssize(size * sizeof(T)) }
  {}

  explicit buffer_view(std::string& str) noexcept
    : data_{ str.data() }, size_{ str.size() }, ssize_{ to_ssize(str.size()) }
  {}

  template<typename T, std::size_t N>
  requires SerializableBuffer<T>
  explicit constexpr buffer_view(std::array<T, N>& array) noexcept
    : data_{ array.data() }, size_{ N * sizeof(T) }, ssize_{ to_ssize(N * sizeof(T)) }
  {}

  /**
   * Templated method to get data as the specified type.
   *
   * Warning: This method does not provide type safety and assumes the user knows the correct type of the data.
   * It also does not perform size checks, so users must ensure that the buffer is large enough to contain
   * the data of type T. Accessing beyond the bounds of the buffer can lead to undefined behavior.
   *
   * @tparam T The type to which the buffer's data will be cast.
   * @return Pointer to the buffer's data cast to the specified type.
   */
  template<typename T>
  requires SerializableBuffer<T>
  [[nodiscard]] constexpr T* as() const noexcept
  {
    return static_cast<T*>(data_);
  }

  [[nodiscard]] constexpr std::size_t size() const noexcept { return size_; }

  [[nodiscard]] constexpr std::ptrdiff_t ssize() const noexcept { return ssize_; }

  [[nodiscard]] constexpr bool is_empty() const noexcept { return size_ == 0; }

  [[nodiscard]] constexpr operator bool() const noexcept { return data_ && size_ > 0; }

private:
  static constexpr std::ptrdiff_t to_ssize(const std::size_t size) noexcept
  {
    using R = std::common_type_t<std::ptrdiff_t, std::make_signed_t<decltype(size)>>;
    return static_cast<R>(size);
  }

private:
  void* data_;
  std::size_t size_;
  std::ptrdiff_t ssize_; // signed
};

template<typename T>
concept NetworkSerializableBasic =
  (std::is_fundamental_v<T> || std::is_enum_v<T>)&&std::is_pointer_v<T> == false && std::is_reference_v<T> == false;

template<typename T>
concept NetworkSerializableBasicArray = std::is_array_v<T> && NetworkSerializableBasic<std::remove_extent_t<T>>;

template<typename T>
concept SerializableString = std::same_as<T, std::string> || std::same_as<T, std::string_view>;

template<typename T>
concept IntegralType = std::is_integral_v<T>;

enum class StringMode {
  SizePrefixed,  // String length is serialized as metadata before the string data (default)
  NullTerminated // Null terminator is appended to the string data instead of prepending string length metadata
};

/**
 * @class binary_stream
 * @brief A class for serializing and deserializing binary data with support for different endianness.
 * It supports handling both internally allocated buffers and user-supplied buffers.
 *
 * @tparam BufferEndian The endianness to use for serialization and deserialization.
 *                      Defaults to big-endian (network byte order).
 */
template<std::endian BufferEndian = std::endian::big>
class binary_stream final
{
public:
  explicit binary_stream(const std::size_t buffer_size) noexcept
    : buffer_{ new std::uint8_t[buffer_size]{}, buffer_size }, owns_buffer_{ true }, write_index_{ 0 }, read_index_{ 0 }
  {}

  explicit constexpr binary_stream(const bytepack::buffer_view& buffer) noexcept
    : buffer_{ buffer }, owns_buffer_{ false }, write_index_{ 0 }, read_index_{ 0 }
  {}

  ~binary_stream() noexcept
  {
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
    write_index_ = 0;
    read_index_ = 0;
  }

  [[nodiscard]] bytepack::buffer_view data() const noexcept
  {
    return bytepack::buffer_view(buffer_.as<std::uint8_t>(), write_index_);
  }

  template<NetworkSerializableBasic T>
  bool write(const T& value) noexcept
  {
    if (buffer_.size() < (write_index_ + sizeof(T))) {
      return false;
    }

    std::memcpy(buffer_.as<std::uint8_t>() + write_index_, &value, sizeof(T));

    if constexpr (BufferEndian != std::endian::native && sizeof(T) > 1) {
      // TODO: htonl/htons/ntohl/ntohs performs better for endianness conversion. However, it
      // requires platform-specific headers. It is possible to implement hton/ntoh like functions
      // in the library in a platform-independent way using bit shifts.
      // Benchmark link: https://quick-bench.com/q/va-kzUk1J1BfvSgR05Z1YPnrJhg
      std::ranges::reverse(buffer_.as<std::uint8_t>() + write_index_,
                           buffer_.as<std::uint8_t>() + write_index_ + sizeof(T));
    }

    write_index_ += sizeof(T);

    return true;
  }

  template<NetworkSerializableBasicArray T>
  bool write(const T& value) noexcept
  {
    using ElementType = std::remove_extent_t<T>;
    constexpr std::size_t elementSize = sizeof(ElementType);
    constexpr std::size_t numElements = sizeof(T) / elementSize;

    // Check if there is enough space in the buffer for the entire array
    if (buffer_.size() < (write_index_ + sizeof(T))) {
      return false;
    }

    if constexpr (BufferEndian == std::endian::native || elementSize == 1) {
      // If the buffer and system endianness match, or each element is one byte,
      // endianness is irrelevant, so memcpy the entire array at once
      std::memcpy(buffer_.as<std::uint8_t>() + write_index_, value, sizeof(T));
      write_index_ += sizeof(T);
    } else {
      // For multi-byte types with differing endianness, handle each element individually
      for (size_t i = 0; i < numElements; ++i) {
        std::memcpy(buffer_.as<std::uint8_t>() + write_index_, &value[i], elementSize);
        std::ranges::reverse(buffer_.as<std::uint8_t>() + write_index_,
                             buffer_.as<std::uint8_t>() + write_index_ + elementSize);
        write_index_ += elementSize;
      }
    }

    return true;
  }

  template<typename T, std::size_t N>
  requires NetworkSerializableBasic<T>
  bool write(const std::array<T, N>& array) noexcept
  {
    if (buffer_.size() < (write_index_ + N * sizeof(T))) {
      // Array elements cannot fit in the remaining buffer space
      return false;
    }

    if constexpr (BufferEndian == std::endian::native || sizeof(T) == 1) {
      // If the buffer and system endianness match, or each element is one byte,
      // endianness is irrelevant, so memcpy the entire array at once
      std::memcpy(buffer_.as<std::uint8_t>() + write_index_, array.data(), N * sizeof(T));
      write_index_ += N * sizeof(T);
    } else { // For multi-byte types with differing endianness, handle each element individually
      for (std::size_t i = 0; i < N; ++i) {
        std::memcpy(buffer_.as<std::uint8_t>() + write_index_, &array[i], sizeof(T));
        std::ranges::reverse(buffer_.as<std::uint8_t>() + write_index_,
                             buffer_.as<std::uint8_t>() + write_index_ + sizeof(T));
        write_index_ += sizeof(T);
      }
    }
    return true;
  }

  template<IntegralType SizeType = std::size_t, typename T>
  requires NetworkSerializableBasic<T>
  bool write(const std::vector<T>& vector, const std::optional<std::size_t> num_elements = std::nullopt) noexcept
  {
    const std::size_t size = num_elements.value_or(vector.size());
    // if `num_elements` is not specified, the vector size is serialized as metadata before the vector data
    if (num_elements == std::nullopt) {
      if (buffer_.size() < (write_index_ + sizeof(SizeType) + size * sizeof(T))) {
        // Vector size field and its elements cannot fit in the remaining buffer space
        return false;
      }

      const SizeType size_custom = static_cast<SizeType>(size);
      if ((std::is_signed_v<SizeType> && size_custom < 0) || static_cast<std::size_t>(size_custom) != size) {
        // Overflow or incorrect size type
        return false;
      }

      // Write vector size field first (before the vector data)
      if (write(size_custom) == false) {
        return false;
      }
    } else {
      if (vector.size() < size || buffer_.size() < (write_index_ + size * sizeof(T))) {
        return false;
      }
    }

    if constexpr (BufferEndian == std::endian::native || sizeof(T) == 1) {
      // If the buffer and system endianness match, or each element is one byte,
      // endianness is irrelevant, so memcpy the entire array at once
      std::memcpy(buffer_.as<std::uint8_t>() + write_index_, vector.data(), size * sizeof(T));
      write_index_ += size * sizeof(T);
    } else { // For multi-byte types with differing endianness, handle each element individually
      for (std::size_t i = 0; i < size; ++i) {
        std::memcpy(buffer_.as<std::uint8_t>() + write_index_, &vector[i], sizeof(T));
        std::ranges::reverse(buffer_.as<std::uint8_t>() + write_index_,
                             buffer_.as<std::uint8_t>() + write_index_ + sizeof(T));
        write_index_ += sizeof(T);
      }
    }
    return true;
  }

  template<IntegralType SizeType = std::size_t, SerializableString StringType>
  bool write(const StringType& value, const bytepack::StringMode mode = bytepack::StringMode::SizePrefixed) noexcept
  {
    if (mode == StringMode::NullTerminated) {
      return write_string_null_terminated(value);
    }
    return write_string_size_prefixed<SizeType>(value);
  }

  // TODO: Currently, the serialization of std::wstring and std::wstring_view is not supported
  // due to the variation in wchar_t size across platforms (e.g., 2 bytes on Windows, 4 bytes on Unix/Linux).
  // A future enhancement could involve standardizing on a character encoding like UTF-8 for network
  // transmission and handling the conversion from/to std::wstring while considering platform differences.
  // This would ensure consistent behavior across different systems and avoid issues with wchar_t size discrepancies.
  // For now, users can use std::string and std::string_view for string serialization.

  template<NetworkSerializableBasic T>
  bool read(T& value) noexcept
  {
    if (buffer_.size() < (read_index_ + sizeof(T))) {
      return false;
    }

    std::memcpy(&value, buffer_.as<std::uint8_t>() + read_index_, sizeof(T));

    if constexpr (BufferEndian != std::endian::native && sizeof(T) > 1) {
      // Using reinterpret_cast to treat 'value' as an array of bytes is safe here because:
      // The `NetworkSerializableBasic` concept ensures 'T' is a non-class, fundamental type, making
      // it trivially copyable and ensuring a consistent, predictable memory layout across systems.
      std::ranges::reverse(reinterpret_cast<std::uint8_t*>(&value),
                           reinterpret_cast<std::uint8_t*>(&value) + sizeof(T));
    }

    read_index_ += sizeof(T);

    return true;
  }

  template<NetworkSerializableBasicArray T>
  bool read(T& value) noexcept
  {
    using ElementType = std::remove_extent_t<T>;
    constexpr std::size_t elementSize = sizeof(ElementType);
    constexpr std::size_t numElements = sizeof(T) / elementSize;

    // Check if there is enough data in the buffer to read the entire array
    if (buffer_.size() < (read_index_ + sizeof(T))) {
      return false;
    }

    if constexpr (BufferEndian == std::endian::native || elementSize == 1) {
      // If the buffer and system endianness match, or each element is one byte,
      // endianness is irrelevant, so memcpy the entire array at once
      std::memcpy(value, buffer_.as<std::uint8_t>() + read_index_, sizeof(T));
      read_index_ += sizeof(T);
    } else {
      // For multi-byte types with differing endianness, handle each element individually
      for (size_t i = 0; i < numElements; ++i) {
        std::memcpy(&value[i], buffer_.as<std::uint8_t>() + read_index_, elementSize);
        std::ranges::reverse(reinterpret_cast<std::uint8_t*>(&value[i]),
                             reinterpret_cast<std::uint8_t*>(&value[i]) + elementSize);
        read_index_ += elementSize;
      }
    }

    return true;
  }

  template<typename T, std::size_t N>
  requires NetworkSerializableBasic<T>
  bool read(std::array<T, N>& array) noexcept
  {
    // Check if there is enough data in the buffer to read the entire array
    if (buffer_.size() < (read_index_ + N * sizeof(T))) {
      return false;
    }

    if constexpr (BufferEndian == std::endian::native || sizeof(T) == 1) {
      // If the buffer and system endianness match, or each element is one byte,
      // endianness is irrelevant, so memcpy the entire array at once
      std::memcpy(array.data(), buffer_.as<std::uint8_t>() + read_index_, N * sizeof(T));
      read_index_ += N * sizeof(T);
    } else {
      // For multi-byte types with differing endianness, handle each element individually
      for (size_t i = 0; i < N; ++i) {
        std::memcpy(&array[i], buffer_.as<std::uint8_t>() + read_index_, sizeof(T));
        std::ranges::reverse(reinterpret_cast<std::uint8_t*>(&array[i]),
                             reinterpret_cast<std::uint8_t*>(&array[i]) + sizeof(T));
        read_index_ += sizeof(T);
      }
    }

    return true;
  }

  template<IntegralType SizeType = std::size_t, typename T>
  requires NetworkSerializableBasic<T>
  bool read(std::vector<T>& vector, const std::optional<std::size_t> num_elements = std::nullopt) noexcept
  {
    std::size_t size{};
    // If `num_elements` is not specified, the std::vector size is deserialized from the metadata before the vector data
    if (num_elements == std::nullopt) {
      SizeType size_custom{};
      // vector size cannot be negative, and zero-size is also unusual so it's treated as an error.
      if (read(size_custom) == false || size_custom < 1) {
        return false;
      }
      size = static_cast<std::size_t>(size_custom);
    } else {
      size = num_elements.value();
    }

    if (buffer_.size() < (read_index_ + size * sizeof(T))) {
      return false;
    }

    vector.resize(size);

    if constexpr (BufferEndian == std::endian::native || sizeof(T) == 1) {
      // If the buffer and system endianness match, or each element is one byte,
      // endianness is irrelevant, so memcpy the entire array at once
      std::memcpy(vector.data(), buffer_.as<std::uint8_t>() + read_index_, size * sizeof(T));
      read_index_ += size * sizeof(T);
    } else { // For multi-byte types with differing endianness, handle each element individually
      for (std::size_t i = 0; i < size; ++i) {
        std::memcpy(&vector[i], buffer_.as<std::uint8_t>() + read_index_, sizeof(T));
        std::ranges::reverse(reinterpret_cast<std::uint8_t*>(&vector[i]),
                             reinterpret_cast<std::uint8_t*>(&vector[i]) + sizeof(T));
        read_index_ += sizeof(T);
      }
    }
    return true;
  }

  template<IntegralType SizeType = std::size_t>
  bool read(std::string& value, const bytepack::StringMode mode = bytepack::StringMode::SizePrefixed) noexcept
  {
    if (mode == StringMode::NullTerminated) {
      return read_string_null_terminated(value);
    }
    return read_string_size_prefixed<SizeType>(value);
  }

private:
  template<IntegralType SizeType = std::size_t, SerializableString StringType>
  bool write_string_size_prefixed(const StringType& value) noexcept
  {
    const SizeType str_length = static_cast<SizeType>(value.length());
    if ((std::is_signed_v<SizeType> && str_length < 0) || static_cast<std::size_t>(str_length) != value.length()) {
      // Overflow or incorrect size type
      return false;
    }

    if (buffer_.size() < (write_index_ + sizeof(SizeType) + static_cast<std::size_t>(str_length))) {
      // String data and its length field cannot fit in the remaining buffer space
      return false;
    }

    // Write string length field first (before the string data)
    if (write(str_length) == false) {
      return false;
    }

    std::memcpy(buffer_.as<std::uint8_t>() + write_index_, value.data(), value.length());
    write_index_ += value.length();

    return true;
  }

  template<SerializableString StringType>
  bool write_string_null_terminated(const StringType& value) noexcept
  {
    const std::size_t str_length = value.length() + 1; // +1 for null terminator

    if (buffer_.size() < (write_index_ + str_length)) {
      return false;
    }

    std::memcpy(buffer_.as<std::uint8_t>() + write_index_, value.data(), str_length - 1);
    buffer_.as<std::uint8_t>()[write_index_ + str_length - 1] = '\0'; // null terminator
    write_index_ += str_length;

    return true;
  }

  template<IntegralType SizeType = std::size_t, SerializableString StringType>
  bool read_string_size_prefixed(StringType& value) noexcept
  {
    if (buffer_.size() < (read_index_ + sizeof(SizeType))) {
      return false;
    }

    // Temporarily read string length without incrementing deserialize index
    SizeType str_length{};
    std::memcpy(&str_length, buffer_.as<std::uint8_t>() + read_index_, sizeof(SizeType));

    // Handle endianness for the string length
    if constexpr (BufferEndian != std::endian::native && sizeof(SizeType) > 1) {
      std::ranges::reverse(reinterpret_cast<std::uint8_t*>(&str_length),
                           reinterpret_cast<std::uint8_t*>(&str_length) + sizeof(SizeType));
    }

    // String length cannot be negative, and zero-length is also unusual so it's treated as an error.
    if (str_length < 1) {
      return false;
    }

    if (buffer_.size() < (read_index_ + sizeof(SizeType) + static_cast<std::size_t>(str_length))) {
      return false;
    }

    // Alternative approach in case of performance issues: first resize the string to the required size
    // using `value.resize(str_length)` and then copy the string data using `std::memcpy(value.data(), ...)`
    value.assign(buffer_.as<char>() + sizeof(SizeType) + read_index_, static_cast<std::size_t>(str_length));

    read_index_ += sizeof(SizeType) + static_cast<std::size_t>(str_length);

    return true;
  }

  template<SerializableString StringType>
  bool read_string_null_terminated(StringType& value) noexcept
  {
    std::size_t end_index = read_index_;
    // Find null terminator in the buffer starting from the current deserialize index
    while (end_index < buffer_.size() && buffer_.as<char>()[end_index] != '\0') {
      ++end_index; // Move to next byte
    }

    if (end_index >= buffer_.size()) {
      return false; // Null terminator not found
    }

    std::size_t str_length = end_index - read_index_;
    value.assign(buffer_.as<char>() + read_index_, str_length);

    read_index_ += str_length + 1; // +1 for null terminator

    return true;
  }

private:
  bytepack::buffer_view buffer_;

  // Flag to indicate buffer ownership
  // TODO: Consider alternative ownership models, and design to handle external buffers
  bool owns_buffer_;

  std::size_t write_index_;
  std::size_t read_index_;
};

} // namespace bytepack

#endif // BYTEPACK_HPP
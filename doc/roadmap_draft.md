# BytePack Roadmap Draft: Collaborative Shaping

This document is a living collection of various ideas at different stages
of development. It includes strong concepts closely aligned with our project's direction, alongside some initial, less
refined thoughts needing more feedback and proof-of-concept work. Remember, a great idea fitting well with BytePack
doesn't guarantee implementation, just as a less likely idea isn't automatically excluded. Inside, you'll find ideas,
detailed feedback, code snippets, potential solutions, and community input. Feedback and suggestions are always welcome
through **GitHub issues** or **e-mail**, and are also gathered from discussions on forums and social media.

> Please also read [Motivation & Design Philosophy of BytePack](design_philosophy.md)
> and [Contributing to BytePack](../CONTRIBUTING.md) documents.

## 1. Partial Serialization & Deserialization

**Overview:** Enabling deserialization of one or more fields from buffer without deserializing the entire
buffer. Also, enabling serialization of one or more fields to correct byte offsets in a buffer without serializing the
entire object.

**Status:** _(PoC implementation is ongoing)_ Exploring API choices, technical approaches and efficiency gains. Seeking
community feedback.

### API Ideas (Without Reflection)

One or more of the following API choices can be implemented. The choice of API will depend on the use case and the
user's preference.

1. Read/Write a single field by **byte offset**:
    - **Cons:** requires users to know the byte offset of the data member.
        - To know the byte offset of a data member in the buffer, users need to know the size of all data members, their
          order in the class, and then calculate the byte offset of the data member by adding the sizes of the previous
          data members. For instance, byte offset of the 3rd data member (c) in the struct is 12:  
          `struct MyStruct { int a; double b; float c; };`
        - If there is an ICD/IDD (Interface Control Document/Interface Design Document) or similar document for the data
          structure, byte offsets for each data member can be found in the document
    - `binary_stream::read_field<ByteOffset>(value)` or `::read_field(value, byte_offset)`
    - `binary_stream::write_field<ByteOffset>(value)` or `::write_field(value, byte_offset)`
2. Read/Write a single field by giving ignored fields:
    - **Cons:** if number of data members is large, it may be difficult to specify all ignored fields.
    - `binary_stream::read_field<IgnoredFields...>(value)` or `::read_field(ignored_fields..., value)`
        - `binary_stream::read_field({ignored_field...}, value)` or `::read_field({ignored_field...}, value)`
    - `binary_stream::write_field<IgnoredFields...>(value)` or `::write_field(ignored_fields..., value)`
3. Read/Write a single field by **index**:
    - Requires registering the data members in the order they appear in the class.
        - `binary_stream::register(data_member1, data_member2, ...)` or `::register<DataMembers...>()`
        - It is possible to calculate byte offset of a data member by its size. One solution is to save the byte offset
          of each data member in a `std::vector<std::size_t>` and use it to get the byte offset of a data member by its
          index.
        - Possible implementation: `byte_offsets.emplace_back(byte_offsets.back() + sizeof(data_memberN));`
    - `binary_stream::read_field<Index>(value)` or `::read_field(value, index)`
    - `binary_stream::write_field<Index>(value)` or `::write_field(value, index)`

### API Ideas (With Reflection)

Using `magic_get`, `boost::pfr` or custom implementation for reflection provides more flexibility and ease of use.
External libraries create a dependency which is not desirable.

## 2. Advanced Bit Serialization

**Overview:** It is not possible to access members of a bitfield directly via pointer or reference, so it is not
possible to serialize each member of a bitfield individually with current binary_stream API. It is not also possible to
serialize a class/struct that contains a bitfield directly because this is not cross-platform compatible due to padding
and alignment differences on different compilers, architectures, and platforms. For now, users can serialize bitfields
by using `std::bitset` or custom bit manipulation. Then current binary_stream API can be used to serialize.

**Status:** Considering different approaches and solutions like having a separate API for bit serialization. For
instance, `::write_bit<BitSize>(value)` or `::write_bit(value, bit_size)`. After bit serialization is done, it must be
aligned to the next byte boundary. Seeking community feedback if there is a need for this feature since there are
already solutions like `std::bitset` and custom bit manipulation.

## 3. Wide String Compatibility

**Overview:** Currently, the serialization of `std::wstring` and `std::wstring_view` is not supported due to the
variation in
`wchar_t` size across platforms (e.g., 2 bytes on Windows, 4 bytes on Unix/Linux). A future enhancement could involve
giving freedom to users to choose all possible implementations of `std::wstring` and `std::wstring_view` serialization
while considering platform differences. Standardization of `wchar_t` size across platforms is also a possible solution
but BytePack does not aim to enforce a specific standard. For now, users can use `std::string` and `std::string_view`
for string serialization.

**Status:** Under consideration. Seeking community feedback.

## 4. `union` and `std::variant` Serialization

**Overview:** Supporting `union` and `std::variant` serialization.

**Status:** Determining implementation challenges and solutions.

## 5. Buffer Ownership Transfer

**Overview:** Enabling buffer owner transfer with `std::unique_ptr` and `std::shared_ptr`. This might be aligned with
TODO comment on _'bool owns_buffer;'_ in _'binary_stream'_ class.

**Status:** Investigating memory management, buffer management, ownership transfer, and safety implications.

## 6. Custom Padding & Alignment

**Overview:** Providing custom padding and alignment options. For instance, padding given uint16_t (2 bytes) to 4 bytes
with 2 bytes of padding (0xFF or 0x00, etc.).  
**Status:** Under consideration. Seeking community feedback on use cases, and if there is a need for this feature.

## 7. Descriptive Error Handling

**Overview:** Implementing more descriptive error codes instead of returning `bool` (true/false) for success/failure.

```cpp
enum class error_code
{
  success = 0,
  buffer_overflow,
  buffer_allocation_failed,
  ...
};
```

It is also possible to implement an error_code struct encapsulating error code, error message, and enabling user to
check with `if (error_code) { ... }` or `if (!error_code) { ... }`:

**Status:** Collecting common error scenarios for comprehensive coverage. Seeking community feedback.

## 8. Dynamic Buffer Management

**Overview:** Enabling dynamic buffer management. For instance, if the buffer is full, automatically increasing the size
of the buffer.

**Status:** Under consideration. Seeking community feedback if there is a need for this feature.

## 9. Aggregate Type Support (Reflection)

**Overview:** Supporting reflection and aggregate type serialization (`magic_get`, `boost::pfr`, custom). This enables
users
to serialize/deserialize aggregate types without explicitly giving the data members. For instance, the following code
would be valid:

```cpp
struct my_struct
{
  int a;
  float b;
  std::string c;
};

my_struct s1{1, 2.0f, "abcdefg"};

bytepack::binary_stream stream(1024);
stream.write(s1); // or stream << s1;
```

**Status:** Comparing different libraries and custom solutions. External libraries create a dependency. Custom solutions
require more work and maintenance (there are simple example implementations of _magic_get_. Can be used as a starting
point). Seeking community feedback.

## 10. Serialization to File Streams

**Overview:** Direct serialization to file streams and deserialization from file streams.

**Status:** Under consideration. Examining file handling and I/O efficiency. Less likely to be implemented. Seeking
community feedback.

## 11. Lazy Buffer Initialization

**Overview:** Delaying buffer initialization until the first serialization/deserialization operation.

**Status:** Researching potential impact on performance and memory usage. Seeking community feedback.

## 12. `std::optional` Serialization

**Overview:** Adding support for `std::optional` serialization/deserialization.

**Status:** Assessing implementation approach and compatibility. Seeking community feedback.

## 13. Global Buffer Utilization

**Overview:** Utilizing a global buffer for serialization/deserialization.

**Status:** Seeking community feedback if there is a need for this feature. Evaluating pros and cons in terms of
performance and usability.

## 14. Extended STL Container Support

**Overview:** Broadening support for other std containers (`std::list`, `std::deque`, `std::map`, etc.).

**Status:** Requires community feedback if there is a need for these containers.

## Community and Communication

Direct communication through GitHub issues and pull requests is the primary way to discuss the project. If you have any
questions or want to discuss a feature idea, feel free to open an issue. You can also reach out to me directly via
e-mail: `faruk [at] farukeryilmaz [dot] com`
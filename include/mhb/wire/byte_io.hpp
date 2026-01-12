#ifndef MPI_HARES_BEARS_BYTE_WRITER_HPP
#define MPI_HARES_BEARS_BYTE_WRITER_HPP

#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>
#include <vector>

namespace mhb::wire {

// przepisuje kontenery i inne dane na bajty do wysłania i odebvran
class ByteWriter {
 public:
  void reserve(std::size_t n) { out_.reserve(n); }

  std::span<const std::uint8_t> bytes() const { return out_; }
  const std::vector<std::uint8_t>& vec() const { return out_; }
  std::vector<std::uint8_t>&& take() { return std::move(out_); }

  template <class T>
  void u(T value) {
    static_assert(std::is_integral_v<T>, "ByteWriter::u<T> wymagany typ całkowity");
    static_assert(std::is_unsigned_v<T>, "ByteWriter::u<T> wymagany typ bez znaku");

    for (std::size_t i = 0; i < sizeof(T); ++i) {
      out_.push_back(static_cast<std::uint8_t>((value >> (i * 8)) & 0xFFu));
    }
  }

  void byte(std::uint8_t b) { out_.push_back(b); }

  template <class T>
  void array_u(std::span<const T> values) {
    static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>);
    for (T v : values) u<T>(v);
  }

 private:
  std::vector<std::uint8_t> out_;
};

// Reader: czyta liczby w little-endian z bufora bajtów. Zwraca false przy błędzie.
class ByteReader {
 public:
  explicit ByteReader(std::span<const std::uint8_t> data) : data_(data) {}

  std::size_t remaining() const { return data_.size() - off_; }
  std::size_t offset() const { return off_; }

  bool byte(std::uint8_t& out) {
    if (remaining() < 1) return false;
    out = data_[off_++];
    return true;
  }

  template <class T>
  bool u(T& out) {
    static_assert(std::is_integral_v<T>, "ByteWriter::u<T> wymagany typ całkowity");
    static_assert(std::is_unsigned_v<T>, "ByteWriter::u<T> wymagany typ bez znaku");

    if (remaining() < sizeof(T)) return false;
    T value = 0;
    for (std::size_t i = 0; i < sizeof(T); ++i) {
      value |= (static_cast<T>(data_[off_++]) << (i * 8));
    }
    out = value;
    return true;
  }

  template <class T>
  bool array_u(std::span<T> out_values) {
    static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>);
    for (std::size_t i = 0; i < out_values.size(); ++i) {
      if (!u<T>(out_values[i])) return false;
    }
    return true;
  }

 private:
  std::span<const std::uint8_t> data_;
  std::size_t off_ = 0;
};

}  // namespace mhb::wire

#endif  // MPI_HARES_BEARS_BYTE_WRITER_HPP

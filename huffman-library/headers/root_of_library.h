#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <istream>
#include <limits>
#include <set>
#include <vector>

namespace huffman_ns {
using byte_t = unsigned char;
using code_t = std::pair<uint64_t, uint32_t>; // value, len

static constexpr size_t BITS_IN_BYTE = std::numeric_limits<byte_t>::digits;
static constexpr size_t SIZE_ALPHABET = std::numeric_limits<byte_t>::max() + 1;
static constexpr size_t BUFF_SIZE = 1 << 14;
static constexpr uint32_t MAX_LEN_OF_CODE = std::numeric_limits<size_t>::digits;

inline byte_t get_byte_from_char(char c) {
  return static_cast<byte_t>(c);
}

inline char get_char_from_byte(byte_t b) {
  return static_cast<char>(b);
}
} // namespace huffman_ns
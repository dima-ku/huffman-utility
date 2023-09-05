#pragma once

#include "root_of_library.h"

namespace huffman_ns {

class binary_reader {
public:
  binary_reader(const binary_reader& other) = delete;
  binary_reader& operator=(const binary_reader& other) = delete;

  explicit binary_reader(std::istream& stream = std::cin);
  ~binary_reader();

  bool get(byte_t& b);

  bool has_next() const;

  void clear();

  void read_buffer();

private:
  std::istream& stream;
  char buffer[BUFF_SIZE];
  size_t pos_in_buffer;
  size_t current_buffer_len;
};
} // namespace huffman_ns

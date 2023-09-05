#pragma once

#include "root_of_library.h"

namespace huffman_ns {

class binary_writer {
public:
  explicit binary_writer(std::ostream& stream = std::cout);

  ~binary_writer();

  void print(byte_t b);

  void flush();

  void print_bit(bool bit);

private:
  void print_buffer();

  std::ostream& stream;
  byte_t buffer[BUFF_SIZE];
  size_t pos_in_buffer;
  uint8_t pos_in_byte;
  byte_t current; // для вывода по битику
};
} // namespace huffman_ns

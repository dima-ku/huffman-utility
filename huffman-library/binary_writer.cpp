#include "headers/binary_writer.h"

#include <cstring>

huffman_ns::binary_writer::binary_writer(std::ostream& stream)
    : stream(stream),
      pos_in_buffer(0),
      pos_in_byte(0),
      current(0) {}

huffman_ns::binary_writer::~binary_writer() {
  print_buffer();
}

void huffman_ns::binary_writer::print(byte_t b) {
  assert(pos_in_byte == 0);
  if (pos_in_buffer < BUFF_SIZE) {
    buffer[pos_in_buffer] = b;
    pos_in_buffer++;
    if (pos_in_buffer == BUFF_SIZE) {
      print_buffer();
    }
  } else {
    print_buffer();
    print(b);
  }
}

void huffman_ns::binary_writer::flush() {
  print_buffer();
}

inline void huffman_ns::binary_writer::print_buffer() {
  if (pos_in_byte != 0) {
    assert(pos_in_buffer < BUFF_SIZE);
    current <<= (BITS_IN_BYTE - pos_in_byte);
    buffer[pos_in_buffer] = current;
    pos_in_buffer++;
  }

  stream.write(reinterpret_cast<char*>(buffer), pos_in_buffer);
  if (errno != 0) {
    throw std::runtime_error("Input is broken: " + std::string(strerror(errno)));
  }
  pos_in_buffer = 0;
  pos_in_byte = 0;
}

void huffman_ns::binary_writer::print_bit(bool bit) {
  if (pos_in_buffer == BUFF_SIZE) {
    print_buffer();
  }
  current <<= 1;
  if (bit) {
    current++;
  }
  pos_in_byte++;
  if (pos_in_byte == BITS_IN_BYTE) {
    buffer[pos_in_buffer] = current;
    pos_in_byte = 0;
    current = 0;
    pos_in_buffer++;
  }
}

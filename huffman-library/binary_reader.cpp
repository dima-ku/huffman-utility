#include "headers/binary_reader.h"

#include "headers/root_of_library.h"

#include <cstring>

huffman_ns::binary_reader::binary_reader(std::istream& stream)
    : stream(stream),
      pos_in_buffer(0),
      current_buffer_len(0) {}

huffman_ns::binary_reader::~binary_reader() {}

bool huffman_ns::binary_reader::get(byte_t& b) {
  if (pos_in_buffer < current_buffer_len) {
    b = get_byte_from_char(buffer[pos_in_buffer]);
    pos_in_buffer++;
    if (pos_in_buffer == current_buffer_len) {
      read_buffer();
    }
    return true;
  } else {
    read_buffer();
    if (pos_in_buffer < current_buffer_len) {
      b = get_byte_from_char(buffer[pos_in_buffer]);
      pos_in_buffer++;
      return true;
    } else {
      return false;
    }
  }
}

void huffman_ns::binary_reader::clear() {
  stream.clear();
  stream.seekg(0);
}

bool huffman_ns::binary_reader::has_next() const {
  return pos_in_buffer < current_buffer_len;
}

void huffman_ns::binary_reader::read_buffer() {
  stream.read(buffer, BUFF_SIZE);
  if (errno != 0) {
    throw std::runtime_error("Input is broken: " + std::string(strerror(errno)));
  }
  pos_in_buffer = 0;
  current_buffer_len = stream.gcount();
}

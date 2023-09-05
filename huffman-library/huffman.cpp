#include "headers/huffman.h"

#include "headers/binary_reader.h"
#include "headers/binary_writer.h"
#include "headers/decoder_huffman_tree.h"
#include "headers/encoder_huffman_tree.h"
#include "headers/root_of_library.h"

#include <sstream>

namespace huffman_ns {
void encode(std::istream& in, std::ostream& out) {
  if (in.rdbuf() == out.rdbuf()) {
    throw std::runtime_error("Please, don't use if in == out\n");
  }

  huffman_ns::binary_reader reader(in);
  huffman_ns::binary_writer writer(out);

  std::vector<size_t> count(SIZE_ALPHABET, 0);

  for (size_t i = 0; i < SIZE_ALPHABET; i++) {
    count[i] = 0;
  }

  byte_t tmp;
  while (reader.get(tmp)) {
    count[tmp]++;
  }
  reader.clear();

  size_t all_count = 0;
  size_t cnt_unique = 0;
  for (size_t i = 0; i < SIZE_ALPHABET; i++) {
    all_count += count[i];
    cnt_unique += count[i] == 0 ? 0 : 1;
  }

  byte_t meta = 0;         // meta = 0 не кодируем
  size_t SMALL_SIZE = 256; // меньше 256 не кодируем
  if (all_count <= SMALL_SIZE) {
    writer.print(meta);
    byte_t tmp;
    while (reader.get(tmp)) {
      writer.print(tmp);
    }
    writer.flush();
    return;
  }

  huffman_ns::encoder_huffman_tree encoded(count, cnt_unique);
  std::vector<code_t> codes = encoded.get_codes();

  size_t cnt_bits = 0;
  for (size_t i = 0; i < SIZE_ALPHABET; i++) {
    cnt_bits += count[i] * codes[i].second;
  }
  uint8_t rem = (BITS_IN_BYTE - cnt_bits % BITS_IN_BYTE) % BITS_IN_BYTE;
  meta = 1 + rem;
  writer.print(meta);

  for (int i = 0; i < SIZE_ALPHABET; i++) {
    writer.print(codes[i].second);
  }

  byte_t readed_byte = 0;
  while (reader.get(readed_byte)) {
    assert(codes[readed_byte].second != 0);
    for (int j = 0; j < codes[readed_byte].second; j++) {
      writer.print_bit(codes[readed_byte].first & (1u << (codes[readed_byte].second - 1 - j)));
    }
  }
  writer.flush();
}

void decode(std::istream& in, std::ostream& out) {
  if (in.rdbuf() == out.rdbuf()) {
    throw std::runtime_error("Please, don't use if in == out\n");
  }

  huffman_ns::binary_reader reader(in);
  huffman_ns::binary_writer writer(out);

  byte_t meta;
  if (!reader.get(meta)) {
    throw std::runtime_error("Decoded file can't be empty!");
  }

  if (meta == 0) {
    // не кодировали
    byte_t tmp;
    while (reader.get(tmp)) {
      writer.print(tmp);
    }
    return;
  }
  if (meta > BITS_IN_BYTE + 1) {
    throw std::runtime_error("File to decode is broken :(");
  }
  int rem = meta - 1;

  huffman_ns::decoder_huffman_tree decode(reader);

  byte_t cur;
  std::vector<huffman_ns::node> all_nodes = decode.get_all_nodes();
  size_t cur_ind_node = 0;
  while (reader.get(cur)) {
    int iter = BITS_IN_BYTE;
    if (!reader.has_next()) {
      iter -= static_cast<int>(rem);
    }
    for (int i = 0; i < iter; i++) {
      bool goRight = cur & (1u << (BITS_IN_BYTE - 1 - i));
      cur_ind_node = goRight ? all_nodes[cur_ind_node].right : all_nodes[cur_ind_node].left;
      if (all_nodes[cur_ind_node].is_leaf()) {
        writer.print(all_nodes[cur_ind_node].value);
        cur_ind_node = 0;
      }
    }
  }
  writer.flush();
}
} // namespace huffman_ns

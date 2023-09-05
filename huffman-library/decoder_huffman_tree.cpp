#include "headers/decoder_huffman_tree.h"

#include "headers/encoder_huffman_tree.h"

void broken_file(
    const std::string& msg = "File to decode is broken, a u sure that's this file encoded by huffman-tool?") {
  throw std::runtime_error(msg);
}

huffman_ns::decoder_huffman_tree::decoder_huffman_tree(binary_reader& reader) {
  std::vector<uint32_t> lens(SIZE_ALPHABET);
  byte_t tmp;
  for (size_t i = 0; i < SIZE_ALPHABET; i++) {
    if (reader.get(tmp)) {
      lens[i] = static_cast<int>(tmp);
    } else {
      broken_file();
    }
    if (lens[i] > MAX_LEN_OF_CODE) {
      throw std::runtime_error("File to decode is too big, we can't decode it!");
    }
  }

  auto codes = encoder_huffman_tree::get_codes_from_lens(lens);

  node rootNode(-1, -1);
  all_nodes.push_back(rootNode); //
  root = &rootNode;

  for (size_t i = 0; i < SIZE_ALPHABET; i++) {
    int cur = 0;
    for (int j = 0; j < codes[i].second; j++) {
      bool bit = codes[i].first & (1 << (codes[i].second - 1 - j));
      if (!bit) {
        if (all_nodes[cur].left != -1) {
          cur = all_nodes[cur].left;
          if (is_terminal[cur]) {
            broken_file();
          }
        } else {
          node new_node(-1, -1);
          if (j + 1 == codes[i].second) {
            new_node.value = i;
            is_terminal[all_nodes.size()] = true;
          }
          all_nodes[cur].left = all_nodes.size();
          cur = all_nodes.size();
          all_nodes.push_back(new_node);
        }
      } else {
        if (all_nodes[cur].right != -1) {
          cur = all_nodes[cur].right;
          if (is_terminal[cur]) {
            broken_file();
          }
        } else {
          node new_node(-1, -1);
          if (j + 1 == codes[i].second) {
            new_node.value = i;
            is_terminal[all_nodes.size()] = true;
          }
          all_nodes[cur].right = all_nodes.size();
          cur = all_nodes.size();
          all_nodes.push_back(new_node);
        }
      }
    }
  }
}

const std::vector<huffman_ns::node>& huffman_ns::decoder_huffman_tree::get_all_nodes() const {
  return all_nodes;
}

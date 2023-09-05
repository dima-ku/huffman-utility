#pragma once

#include "binary_reader.h"
#include "node.h"
#include "root_of_library.h"

namespace huffman_ns {
struct decoder_huffman_tree {
  explicit decoder_huffman_tree(binary_reader& reader);

  const std::vector<node>& get_all_nodes() const;

private:
  node* root;
  std::vector<node> all_nodes;
  bool is_terminal[2 * SIZE_ALPHABET]{};
};
} // namespace huffman_ns

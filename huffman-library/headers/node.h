#pragma once

#include "root_of_library.h"

namespace huffman_ns {
struct node {
  int32_t left;
  int32_t right;
  byte_t value;

  ~node() = default;

  node(int32_t l, int32_t r) : left(l), right(r) {}

  node(int32_t l, int32_t r, byte_t v) : left(l), right(r), value(v) {}

  inline bool is_leaf() const {
    return left == -1 && right == -1;
  }
};
} // namespace huffman_ns

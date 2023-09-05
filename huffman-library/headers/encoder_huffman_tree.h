#pragma once

#include "node.h"
#include "root_of_library.h"

namespace huffman_ns {
struct encoder_huffman_tree {
  explicit encoder_huffman_tree(const std::vector<size_t>& count, size_t cnt_unique);

  static std::vector<uint32_t> get_lens_of_codes_via_count(const std::vector<size_t>& count, size_t cnt_unique);

  static std::vector<code_t> get_codes_from_lens(const std::vector<uint32_t>& lens);

  const std::vector<code_t>& get_codes() const;

private:
  std::vector<code_t> codes = std::vector<code_t>(SIZE_ALPHABET);
};
} // namespace huffman_ns

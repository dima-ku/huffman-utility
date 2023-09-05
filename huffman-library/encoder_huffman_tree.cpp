#include "headers/encoder_huffman_tree.h"

#include "headers/node.h"

huffman_ns::encoder_huffman_tree::encoder_huffman_tree(const std::vector<size_t>& count, size_t cnt_unique) {
  std::vector<uint32_t> lens = get_lens_of_codes_via_count(count, cnt_unique);
  codes.reserve(SIZE_ALPHABET);
  codes = get_codes_from_lens(lens);
}

void dfs(size_t curV, int deep, const std::vector<huffman_ns::node>& all_nodes, std::vector<uint32_t>& lens);

std::vector<uint32_t> huffman_ns::encoder_huffman_tree::get_lens_of_codes_via_count(const std::vector<size_t>& count,
                                                                                    size_t cnt_unique) {
  std::vector<uint32_t> lens(SIZE_ALPHABET, 0);
  if (cnt_unique <= 2) {
    for (size_t i = 0; i < count.size(); i++) {
      if (count[i] != 0) {
        lens[i] = 1;
      }
    }
  } else {
    std::vector<node> all_nodes;
    std::set<std::pair<size_t, size_t>> s; // пара - {вес, индекс ноды в массиве allNodes}

    for (size_t i = 0; i < SIZE_ALPHABET; i++) {
      if (count[i] != 0) {
        node new_node(-1, -1, i);
        s.insert({count[i], all_nodes.size()});
        all_nodes.push_back(new_node);
      }
    }

    while (s.size() > 1) {
      auto u = *(s.begin());
      s.erase(s.begin());
      auto v = *(s.begin());
      s.erase(s.begin());

      node new_node(u.second, v.second);
      s.insert({u.first + v.first, all_nodes.size()});
      all_nodes.push_back(new_node);
    }

    dfs(all_nodes.size() - 1, 0, all_nodes, lens);
  }
  return lens;
}

std::vector<huffman_ns::code_t> huffman_ns::encoder_huffman_tree::get_codes_from_lens(
    const std::vector<uint32_t>& lens) {
  // algorithm: http://www.compression.ru/download/articles/huff/simakov_2002_huffcode.html#Tur97

  int mxL = 0;
  std::vector<uint8_t> cntWithL(SIZE_ALPHABET, 0);
  for (int len : lens) {
    mxL = std::max(mxL, static_cast<int>(len));
    cntWithL[len]++;
  }

  std::vector<code_t> codes(SIZE_ALPHABET, {0, 0});
  if (mxL <= 1) {
    uint32_t cur = 0;
    for (size_t i = 0; i < SIZE_ALPHABET; i++) {
      if (lens[i] != 0) {
        codes[i] = {cur++, 1};
      }
    }
  } else {
    std::vector<uint64_t> s(mxL + 1);
    s[mxL] = 0;
    for (int i = mxL - 1; i >= 2; i--) {
      s[i] = (s[i + 1] + cntWithL[i + 1]) >> 1;
    }
    s[1] = 1;
    std::vector<size_t> ind_with_this_len(mxL + 1, 0);
    for (size_t i = 0; i < SIZE_ALPHABET; i++) {
      codes[i].second = lens[i];
      codes[i].first = s[lens[i]] + ind_with_this_len[lens[i]];
      ind_with_this_len[lens[i]]++;
    }
  }
  return codes;
}

const std::vector<huffman_ns::code_t>& huffman_ns::encoder_huffman_tree::get_codes() const {
  return codes;
}

void dfs(size_t curV, int deep, const std::vector<huffman_ns::node>& all_nodes, std::vector<uint32_t>& lens) {
  if (all_nodes[curV].is_leaf()) {
    lens[all_nodes[curV].value] = deep;
    return;
  }
  if (all_nodes[curV].left != -1) {
    dfs(all_nodes[curV].left, deep + 1, all_nodes, lens);
  }
  if (all_nodes[curV].right != -1) {
    dfs(all_nodes[curV].right, deep + 1, all_nodes, lens);
  }
}

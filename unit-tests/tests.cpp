#include "huffman.h"

#include <gtest/gtest.h>

#include <fstream>

std::stringstream get_random_string_stream(size_t len) {
  std::stringstream result;
  for (size_t i = 0; i < len; i++) {
    result << huffman_ns::get_char_from_byte(rand() % huffman_ns::SIZE_ALPHABET);
  }
  return result;
}

void correctness_via_string_stream(std::stringstream in) {
  std::stringstream storage("");
  std::stringstream out("");
  huffman_ns::encode(in, storage);
  huffman_ns::decode(storage, out);
  EXPECT_EQ(in.str(), out.str());
}

void correctness_via_string_stream(const std::string& s) {
  correctness_via_string_stream(std::stringstream(s));
}

TEST(correctnes, testingStringStream) {
  correctness_via_string_stream("");
  correctness_via_string_stream("kek");
  correctness_via_string_stream("lol");
  correctness_via_string_stream("rofl");
  correctness_via_string_stream("for(int i = 0; i < n; i++)");
  correctness_via_string_stream("huffman");
  correctness_via_string_stream(get_random_string_stream(159));
  correctness_via_string_stream(get_random_string_stream(228));
  correctness_via_string_stream(get_random_string_stream(282));
  correctness_via_string_stream(get_random_string_stream(337));
  correctness_via_string_stream(get_random_string_stream(1337));
  correctness_via_string_stream(get_random_string_stream(10000));
  std::string allA, allB;
  for (int i = 0; i < 228; i++) {
    allA += 'a';
    allB += 'b';
  }
  correctness_via_string_stream(allA);
  correctness_via_string_stream(allB);
  correctness_via_string_stream(allA + allB);
  correctness_via_string_stream(allB + allA);
}

TEST(expect_throw, throw1) {
  std::string broken_meta = "a";
  std::stringstream in(broken_meta);
  std::stringstream out;
  EXPECT_ANY_THROW(huffman_ns::decode(in, out));
}

TEST(expect_throw, throw2) {
  std::string correct_meta_but_very_long_lens;
  correct_meta_but_very_long_lens += huffman_ns::get_char_from_byte(6);
  for (size_t i = 0; i < huffman_ns::SIZE_ALPHABET; i++) {
    correct_meta_but_very_long_lens += huffman_ns::get_char_from_byte(i);
  }
  std::stringstream in(correct_meta_but_very_long_lens);
  std::stringstream out;
  EXPECT_ANY_THROW(huffman_ns::decode(in, out));
}

TEST(expect_throw, throw3) {
  std::stringstream stream("Tyler");
  EXPECT_ANY_THROW(huffman_ns::encode(stream, stream));
}

TEST(expect_throw, throw4) {
  std::stringstream stream("Derden");
  EXPECT_ANY_THROW(huffman_ns::decode(stream, stream));
}

TEST(expect_throw, throw5) {
  std::stringstream in;
  huffman_ns::byte_t meta = 5;
  in << huffman_ns::get_char_from_byte(meta);
  for (size_t i = 0; i < huffman_ns::SIZE_ALPHABET; i++) {
    in << huffman_ns::get_char_from_byte(rand() % 7);
  }
  in << get_random_string_stream(100).str();
  EXPECT_ANY_THROW(huffman_ns::decode(in, std::cout));
}

TEST(expect_throw, throw6) {
  // decode from empty file
  std::stringstream in;
  EXPECT_ANY_THROW(huffman_ns::decode(in, std::cout));
}

cmake_minimum_required(VERSION 3.21)
project(huffman-library)

add_library(huffman-library STATIC
        binary_reader.cpp
        binary_writer.cpp
        encoder_huffman_tree.cpp
        decoder_huffman_tree.cpp
        huffman.cpp
        )


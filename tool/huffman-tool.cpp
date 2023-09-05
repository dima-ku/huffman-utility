#include "huffman.h"
#include "root_of_library.h"

#include <cstring>
#include <fstream>
#include <iostream>

void get_help() {
  std::cout << "Usage:\n";
  std::cout << "--help - like manual\n";
  std::cout << "--input %inputFileName% - input file to decode/encode\n";
  std::cout << "--output %outputFileName% - output file to decode/encode\n";
  std::cout << "--compress / --decompress - u must choose one of them, to decode/encode your file\n";
}

void print_error(const std::string& msg) {
  std::cerr << msg;
  get_help();
}

int main(int argc, char** argv) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++) {
    args.emplace_back(argv[i]);
  }

  std::string input;
  std::string output;
  bool visited_compress = false;
  bool visited_decompress = false;

  for (int i = 0; i < args.size(); i++) {
    if (args[i].size() >= 2) {
      if (args[i][0] != '-' && args[i][1] != '-') {
        print_error("Incorrect usage of program\n");
        return EXIT_FAILURE;
      }
      std::string parsed;
      for (size_t j = 2; j < args[i].size(); j++) {
        parsed += args[i][j];
      }
      if (parsed == "help") {
        get_help();
        return EXIT_SUCCESS;
      } else if (parsed == "input") {
        if (i + 1 == args.size()) {
          print_error("Incorrect usage of program\n");
          return EXIT_FAILURE;
        }
        i++;
        input = args[i];
      } else if (parsed == "output") {
        if (i + 1 == args.size()) {
          print_error("Incorrect usage of program\n");
          return EXIT_FAILURE;
        }
        i++;
        output = args[i];
      } else if (parsed == "compress") {
        visited_compress = true;
      } else if (parsed == "decompress") {
        visited_decompress = true;
      }
    } else {
      print_error("Incorrect usage of program\n");
      return EXIT_FAILURE;
    }
  }
  if (input.empty()) {
    print_error("input file not found in arguments!\n");
    return EXIT_FAILURE;
  } else if (output.empty()) {
    print_error("output file not found in arguments!\n");
    return EXIT_FAILURE;
  } else if (visited_compress == visited_decompress) {
    print_error("Program can only compress XOR decompress\n");
    return EXIT_FAILURE;
  }

  std::fstream in_stream(input, std::fstream::in);
  std::fstream out_stream(output, std::fstream::out);

  if (in_stream.fail()) {
    std::cerr << "Input file is broken! Error: " << strerror(errno) << "\n";
    return EXIT_FAILURE;
  } else if (out_stream.fail()) {
    std::cerr << "Output file is broken! Error: " << strerror(errno) << "\n";
    return EXIT_FAILURE;
  }

  if (visited_compress) {
    huffman_ns::encode(in_stream, out_stream);
  } else {
    huffman_ns::decode(in_stream, out_stream);
  }

  return EXIT_SUCCESS;
}

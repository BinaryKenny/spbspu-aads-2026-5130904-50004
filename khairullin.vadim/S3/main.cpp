#include <iostream>
#include "GraphSystem.h"
#include "fstream"

int main(int argc, char ** argv)
{
  if (argc != 1) {
    std::cerr << "No file or too many arguments\n";
    return 1;
  }
  const char * filename = argv[1];
  std::ifstream input(filename);
  khairullin::Vector< std::string > lines;
  while (!input.eof()) {
    std::string line = "";
    std::getline(input, line);
    if (line != "") {
      try {
        lines.pushBack(line);
      } catch (const std::bad_alloc & error) {
        std::cerr << error.what() << "\n";
        return 1;
      }
    }
  }
  khairullin::GraphSystem graphs;
  for (size_t i = 0; i < lines.getSize(); i++) {
    try {
      graphs.func(lines[i]);
    } catch (std::logic_error & e) {
      std::cout << e.what() << "\n";
    } catch (const std::bad_alloc & error) {
      std::cerr << error.what() << "\n";
      return 1;
    }
  }
}

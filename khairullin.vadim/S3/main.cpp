#include <iostream>
#include "GraphSystem.h"
#include "fstream"

int main(int argc, char ** argv)
{
  if (argc != 2) {
    std::cerr << "No file or too many arguments\n";
    return 1;
  }
  const char * filename = argv[1];
  std::ifstream input(filename);
  khairullin::Vector< std::string > lines;
  std::string line = "";
  while (std::getline(input, line, '\n')) {
    if (!line.empty()) {
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

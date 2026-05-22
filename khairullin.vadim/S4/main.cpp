#include "Datasets.h"
#include "Datasets.cpp"
#include <iostream>
#include <fstream>
int main(int argc, char ** argv)
{
  if (argc == 1) {
    std::cerr << "No such file\n";
    return 2;
  }
  const char * filename = argv[1];
  std::ifstream input(filename);
  khairullin::Datasets datasets;
  while (input) {
    try {
      datasets.addDataset(input);
    }
    catch (std::exception & e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }
  input.close();
  std::string line = "";
  while (std::getline(std::cin, line)) {
    try {
      datasets.function(line);
    }
    catch (std::logic_error & e) {
      std::cout << e.what() << "\n";
    }
    catch (...) {
      std::cerr << "Bad allocation error\n";
      return 1;
    }
  }
}

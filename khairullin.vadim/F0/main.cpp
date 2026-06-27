#include "Social.h"
#include <fstream>
int main(int argc, char * argv[])
{
  khairullin::Social Space;
  std::ifstream input(argv[1]);
  while (input) {
    try {
      Space.processor(input);
    }
    catch (...) {
    }
  }
  while (std::cin) {
    try {
      Space.processor(std::cin);
    }
    catch (const std::logic_error & e) {
      std::cout << e.what() << "\n";
    }
    catch (...) {
      std::cerr << "Unknown exception\n";
    }
  }
}

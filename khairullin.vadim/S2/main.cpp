#include <iostream>
#include <fstream>
#include "../common/Stack.hpp"
#include "../common/Queue.hpp"
#include "functions.hpp"
#include "Data.hpp"

using int_ll = long long int;

int main(int argc, char ** argv)
{
  std::string line = "";
  std::ifstream file;
  std::istream * in = &std::cin;
  if (argc > 1)
  {
    file.open(argv[1]);
    in = &file;
  }

  khairullin::Stack<int_ll> output_stack;
  while (std::getline(*in, line)) {
    int_ll res = khairullin::result(line);
    output_stack.push(res);
  }
  if (!output_stack.empty())
  {
    std::cout << output_stack.drop();
  }
  while (!output_stack.empty())
  {
    std::cout << " " << output_stack.drop();
  }
  std::cout << "\n";
}

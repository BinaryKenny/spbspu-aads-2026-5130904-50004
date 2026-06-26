#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "../common/Queue.hpp"
#include "Data.hpp"
#include <string>

using int_ll = long long int;
namespace khairullin
{
  bool operandIsChar(const char c);
  Queue<Data> input(std::string line);
  size_t getPriority(const char op);
  bool cmpPriority(const char op1, const char op2);
  void postfix(Queue< Data > & q, Queue< Data > & q1);
  int_ll pow(int_ll n1, int_ll n2);
  int_ll reverse(int_ll number);
  int_ll result(std::string line);
}
#endif

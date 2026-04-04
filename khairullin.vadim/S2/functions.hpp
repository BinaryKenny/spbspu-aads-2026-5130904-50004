#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "Queue.hpp"
#include "Stack.hpp"
#include "Data.hpp"
#include <string>
#include <cstddef>
namespace khairullin
{
  bool is_char(const char c);
  Queue<Data> input(std::string line);
  size_t get_priority(const char & op);
  bool priority(const char & op1, const char & op2);
  void postfix(Queue<Data> & q, Queue<Data> & q1);
  int degree(int n1, int n2);
  int reverse(int number);
}
#endif

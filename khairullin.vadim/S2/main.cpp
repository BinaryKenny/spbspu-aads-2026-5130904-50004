#include <iostream>
#include <limits>
#include <fstream>
#include <cstddef>
#include "List.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "functions.hpp"
#include "Data.hpp"

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

  khairullin::Stack<int> output_stack;

  constexpr int MAX = std::numeric_limits<int>::max();
  constexpr int MIN = std::numeric_limits<int>::min();
  while (std::getline(*in, line))
  {
    khairullin::Queue<khairullin::Data> input_queue;
    try
    {
      input_queue = khairullin::input(line);
    }
    catch (const std::exception & err)
    {
      std::cerr << err.what() << "\n";
      return 1;
    }
    if (!input_queue.not_empty())
    {
      continue;
    }
    khairullin::Queue<khairullin::Data> postfix_queue;
    try
    {
      khairullin::postfix(input_queue, postfix_queue);
    }
    catch (const std::bad_alloc & err)
    {
      std::cerr << err.what() << "\n";
      return 2;
    }
    khairullin::Stack<khairullin::Data> res_stack;
    try
    {
      while (postfix_queue.not_empty())
      {
        while(postfix_queue.value().is_int())
        {
          res_stack.push(postfix_queue.drop());
        }
        if (postfix_queue.not_empty())
        {
          char oper = postfix_queue.drop().char_value();
          if (oper == '#')
          {
            int number = res_stack.drop().value();
            res_stack.push(khairullin::Data(khairullin::reverse(number)));
          }
          else if (oper == '+')
          {
            int val_1 = res_stack.drop().value();
            int val_2 = res_stack.drop().value();
            int result = 0;
            if (MAX - val_1 > val_2)
            {
              result = val_1 + val_2;
              res_stack.push(khairullin::Data(result));
            }
            else
            {
              std::cerr << "Overflow\n";
              return 2;
            }
          }
          else if (oper == '-')
          {
            int val_1 = res_stack.drop().value();
            int val_2 = res_stack.drop().value();
            int result = 0;
            if (val_1 > 0)
            {
              if (MIN + val_1 =< val_2)
              {
                result = val_2 - val_1;
                res_stack.push(khairullin::Data(result));
              }
              else
              {
                std::cerr << "Underflow\n";
                return 2;
              }
            }
            else
            {
              if (MAX + val_1 => val_2 )
              {
                result = val_2 - val_1;
                res_stack.push(khairullin::Data(result));
              }
              else
              {
                std::cerr << "Overflow\n";
                return 2;
              }
            }
          }
          else if (oper == '*')
          {
            int val_1 = res_stack.drop().value();
            int val_2 = res_stack.drop().value();
            int result = 0;
            if (MAX / val_1 >= val_2)
            {
              result = val_1 * val_2;
              res_stack.push(khairullin::Data(result));
            }
            else
            {
              std::cerr << "Overflow\n";
              return 2;
            }
          }
          else if (oper == '/')
          {
            int val_1 = res_stack.drop().value();
            int val_2 = res_stack.drop().value();
            if (val_1 != 0)
            {
              int result = val_2 / val_1;
              res_stack.push(khairullin::Data(result));
            }
            else
            {
              std::cerr << "Divide 0\n";
              return 1;
            }
          }
          else if (oper == '%')
          {
            int val_1 = res_stack.drop().value();
            int val_2 = res_stack.drop().value();
            if (val_1 != 0)
            {
              int result = val_2 % val_1;
              if (result < 0)
              {
                result += val_1;
              }
              res_stack.push(khairullin::Data(result));
            }
            else
            {
              std::cerr << "Divide 0\n";
              return 1;
            }
          }
          else
          {
            std::cerr << "Fail input\n";
            return 1;
          }
        }
        output_stack.push(res_stack.drop().value());
        if (res_stack.not_empty())
        {
          std::cerr << "Wrong\n";
          return 3;
        }
      }
    }
    catch (...)
    {
      std::cerr << "Exception\n";
      return 2;
    }
    if (output_stack.not_empty())
    {
      std::cout << output_stack.drop();
    }
    while (output_stack.not_empty())
    {
      std::cout << " " << output_stack.drop();
    }
    std::cout << "\n";
  }
}

#include "functions.hpp"
#include <limits>
#include <iostream>

using int_ll = long long int;

bool khairullin::operandIsChar(const char c)
{
  switch (c)
  {
    case '(':
      return true;
    case ')':
      return true;
     case '+':
      return true;
    case '-':
      return true;
    case '*':
      return true;
    case '/':
      return true;
    case '%':
      return true;
    case '#':
      return true;
    default:
      return false;
  }
}

khairullin::Queue< khairullin::Data > khairullin::input(std::string line)
{
  Queue< Data > q;
  size_t i = 0;
  while(i < line.length())
  {
    char temp = line[i];
    if (operandIsChar(temp))
    {
      q.push(Data(temp));
      i++;
    }
    else
    {
      int_ll num = 0;
      std::string number = "";
      while(line[i] != ' ' && i < line.length())
      {
        number += line[i];
        i++;
      }
      try
      {
        num = std::stoll(number);
      }
      catch(...)
      {
        throw std::out_of_range("Some problems with the number");
      }
      q.push(Data(num));
    }
    if (i < line.length() && line[i] == ' ')
    {
      i++;
    }
  }
  return q;
}

size_t khairullin::getPriority(const char op)
{
  if (op == '+' || op == '-')
  {
    return 1;
  }
  if (op == '*' || op == '/' || op == '%')
  {
    return 2;
  }
  if (op == '#')
  {
    return 3;
  }
  return 0;
}

bool khairullin::cmpPriority(const char op1, const char op2)
{
  return getPriority(op1) >= getPriority(op2);
}

void khairullin::postfix(Queue< Data > & q, Queue< Data > & q1)
{
  Stack< Data > s2;
  while (!q.empty())
  {
    Data val = q.drop();
    if (val.is_int())
    {
      q1.push(val);
    }
    else if (val.char_value() == '(')
    {
      s2.push(val);
    }
    else if (val.char_value() == ')')
    {
      while (!s2.empty() && s2.top().char_value() != '(')
      {
        q1.push(s2.drop());
      }
      if (!s2.empty() && s2.top().char_value() == '(')
      {
        s2.drop();
      }
    }
    else
    {
      while (!s2.empty() && s2.top().char_value() != '('
        && cmpPriority(s2.top().char_value(), val.char_value()))
      {
        q1.push(s2.drop());
      }
      s2.push(val);
    }
  }
  while (!s2.empty())
  {
    q1.push(s2.drop());
  }
}

int_ll khairullin::pow(int_ll n1, int_ll n2)
{
  int_ll result = 1;
  while (n2 != 0)
  {
    result *= n1;
    n2--;
  }
  return result;
}
int_ll khairullin::reverse(int_ll number)
{
  int_ll MAX_NUM = std::numeric_limits<int_ll>::max();
  int_ll result = 0;
  int_ll sign = number >= 0 ? 1 : -1;
  number = number * sign;
  size_t counter = 0;
  const size_t MAX_SIZE = 19;
  int_ll temp[MAX_SIZE] = {0};
  while (number)
  {
    temp[counter] = number % 10;
    counter++;
    number = number / 10;
  }
  size_t id = 0;
  int_ll grade = static_cast<int_ll>(counter - 1);
  while(id < counter)
  {
    if (temp[id] > MAX_NUM / pow(10, grade)) {
      throw std::out_of_range("Some problems with the number");
    }
    if (result > MAX_NUM - temp[id] * pow(10, grade)) {
      throw std::out_of_range("Some problems with the number");
    }
    result += temp[id] * pow(10, grade);
    id++;
    grade--;
  }
  return sign * result;
}

int_ll khairullin::result(std::string line)
{
  constexpr int_ll MAX = std::numeric_limits<int_ll>::max();
  constexpr int_ll MIN = std::numeric_limits<int_ll>::min();
  Queue< Data > input_queue;
  try
  {
    input_queue = input(line);
  }
  catch (const std::exception & err) {
    std::cerr << err.what() << "\n";
    return 1;
  }
  Queue< Data > postfix_queue;
  try
  {
    postfix(input_queue, postfix_queue);
  }
  catch (const std::bad_alloc & err)
  {
    std::cerr << err.what() << "\n";
    return 2;
  }
  Stack< Data > res_stack;
  try
  {
    while (!postfix_queue.empty()) {
      while(!postfix_queue.empty() && postfix_queue.front().is_int())
      {
        res_stack.push(postfix_queue.drop());
      }
      if (postfix_queue.empty())
      {
        break;
      }
      char oper = postfix_queue.drop().char_value();
      if (oper == '#')
      {
        int_ll number = res_stack.drop().value();
        res_stack.push(Data(reverse(number)));
      }
      else if (oper == '+')
      {
        int_ll val_1 = res_stack.drop().value();
        int_ll val_2 = res_stack.drop().value();
        int_ll result = 0;
        if (MAX - val_1 >= val_2)
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
        int_ll val_1 = res_stack.drop().value();
        int_ll val_2 = res_stack.drop().value();
        int_ll result = 0;
        if (val_1 > 0)
        {
          if (MIN + val_1 <= val_2)
          {
            result = val_2 - val_1;
            res_stack.push(Data(result));
          }
          else
          {
            std::cerr << "Underflow\n";
            return 2;
          }
        }
        else
        {
          if (MAX + val_1 >= val_2 )
          {
            result = val_2 - val_1;
            res_stack.push(Data(result));
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
        int_ll val_1 = res_stack.drop().value();
        int_ll val_2 = res_stack.drop().value();
        int_ll result = 0;
        bool can_multiply = ((val_1 > 0 && val_2 > 0) && (MAX / val_1 >= val_2)) || (val_1 == 0 || val_2 == 0);
        can_multiply = can_multiply || ((val_1 < 0 && val_2 < 0) && (MIN / (-val_1) <= val_2));
        can_multiply = can_multiply || ((val_1 > 0 && val_2 < 0) && (MIN / val_1 <= val_2));
        can_multiply = can_multiply || ((val_1 < 0 && val_2 > 0) && (MIN/ val_1 >= val_2));
        can_multiply = can_multiply || (val_1 == 0 || val_2 == 0);
        if (can_multiply)
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
        int_ll val_1 = res_stack.drop().value();
        int_ll val_2 = res_stack.drop().value();
        if (val_1 != 0)
        {
          int_ll result = val_2 / val_1;
          res_stack.push(Data(result));
        }
        else
        {
          std::cerr << "Divide 0\n";
          return 1;
        }
      }
      else if (oper == '%')
      {
        int_ll val_1 = res_stack.drop().value();
        int_ll val_2 = res_stack.drop().value();
        if (val_1 != 0)
        {
          int_ll result = val_2 % val_1;
          if (result < 0)
          {
            result += val_1;
          }
          res_stack.push(Data(result));
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
  }
  catch (...) {
    std::cerr << "Exception\n";
    return 2;
  }
  return res_stack.drop().value();
}

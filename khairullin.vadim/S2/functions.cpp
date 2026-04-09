#include "functions.hpp"

using int_ll = long long int;

bool khairullin::is_char(const char c)
{
  bool cond = c == '(';
  cond = cond || c == ')';
  cond = cond || c == '+';
  cond = cond || c == '-';
  cond = cond || c == '*';
  cond = cond || c == '/';
  cond = cond || c == '%';
  cond = cond || c == '#';
  return cond;
}

khairullin::Queue<khairullin::Data> khairullin::input(std::string line)
{
  khairullin::Queue<khairullin::Data> q{};
  size_t i = 0;
  while(i < line.length())
  {
    char temp = line[i];
    if (khairullin::is_char(temp))
    {
      q.push(khairullin::Data(temp));
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
      q.push(khairullin::Data(num));
    }
    if (i < line.length() && line[i] == ' ')
    {
      i++;
    }
  }
  return q;
}

size_t khairullin::get_priority(const char & op)
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

bool khairullin::priority(const char & op1, const char & op2)
{
  return khairullin::get_priority(op1) >= khairullin::get_priority(op2);
}

void khairullin::postfix(khairullin::Queue<khairullin::Data> & q,
  khairullin::Queue<khairullin::Data> & q1)
{
  khairullin::Stack<khairullin::Data> s2;
  while (q.not_empty())
  {
    khairullin::Data val = q.drop();
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
      while (s2.not_empty() && s2.value().char_value() != '(')
      {
        q1.push(s2.drop());
      }
      if (s2.not_empty() && s2.value().char_value() == '(')
      {
        s2.drop();
      }
    }
    else
    {
      while (s2.not_empty() && s2.value().char_value() != '('
        && khairullin::priority(s2.value().char_value(), val.char_value()))
      {
        q1.push(s2.drop());
      }
      s2.push(val);
    }
  }
  while (s2.not_empty())
  {
    q1.push(s2.drop());
  }
}

int_ll khairullin::degree(int_ll n1, int_ll n2)
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
  int_ll result = 0;
  int_ll sign = number >= 0 ? 1 : -1;
  number = number * sign;
  size_t counter = 0;
  int_ll temp[10] = {0};
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
    result += temp[id] * khairullin::degree(10, grade);
    id++;
    grade--;
  }
  return sign * result;
}

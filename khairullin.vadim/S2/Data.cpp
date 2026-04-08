#include "Data.hpp"

khairullin::Data::Data(const int & temp):
int_value(temp),
is_integer(true),
symbol(' ')
{}

khairullin::Data::Data(const char & temp):
int_value(0),
is_integer(false),
symbol(temp)
{}

bool khairullin::Data::is_int() const noexcept
{
  return is_integer;
}

int khairullin::Data::value() const noexcept
{
  return int_value;
}

char khairullin::Data::char_value() const noexcept
{
  return symbol;
}

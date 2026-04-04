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

const bool khairullin::Data::is_int() const
{
  return is_integer;
}

const int khairullin::Data::value() const
{
  return int_value;
}

const char khairullin::Data::char_value() const
{
  return symbol;
}

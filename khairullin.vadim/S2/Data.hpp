#ifndef DATA_HPP
#define DATA_HPP

using int_ll = long long int;
namespace khairullin
{
  struct Data
  {
    Data(const int & temp);
    Data(const char & temp);
    Data() = default;
    bool is_int() const noexcept;
    int_ll value() const noexcept;
    char char_value() const noexcept;
  private:
    int_ll int_value;
    bool is_integer;
    char symbol;
  };
}
#endif

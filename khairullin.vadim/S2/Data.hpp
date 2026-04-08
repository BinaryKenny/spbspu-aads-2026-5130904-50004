#ifndef DATA_HPP
#define DATA_HPP
namespace khairullin
{
  struct Data
  {
    Data(const int & temp);
    Data(const char & temp);
    Data() = default;
    bool is_int() const noexcept;
    int value() const noexcept;
    char char_value() const noexcept;
  private:
    int int_value;
    bool is_integer;
    char symbol;
  };
}
#endif

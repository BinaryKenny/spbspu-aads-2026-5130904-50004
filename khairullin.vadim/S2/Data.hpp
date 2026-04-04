#ifndef DATA_HPP
#define DATA_HPP
namespace khairullin
{
  struct Data
  {
    Data(const int & temp);
    Data(const char & temp);
    Data() = default;
    const bool is_int() const ;
    const int value() const ;
    const char char_value() const ;
  private:
    int int_value;
    bool is_integer;
    char symbol;
  };
}
#endif

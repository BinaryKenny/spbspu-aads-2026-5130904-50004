#ifndef EQUAL_H
#define EQUAL_H

namespace khairullin
{
  template< class T >
  struct Equal
  {
    Equal() = default;
    bool operator()(const T & val1, const T & val2)
    {
      return val1 == val2;
    }
  };
}
#endif

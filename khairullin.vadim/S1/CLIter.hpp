#ifndef CLITER_HPP
#define CLITER_HPP
#include "List.hpp"
namespace khairullin
{
  template<class T>
  struct CLIter
  {
    friend struct List<T>;
    List<T> * list;
    CLIter(List<T> * h);
    static CLIter begin(List<T> * head);
    static CLIter end();
    bool hasNext();
    bool not_empty();
    CLIter next();
    T & value();
  };
}
#endif

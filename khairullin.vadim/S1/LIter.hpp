#ifndef LITER_HPP
#define LITER_HPP

#include "List.hpp"
template<class T>
struct LIter
{
  friend class List<T>;
  List<T> * list;
  LIter(List<T> * h);
  static LIter begin(List<T> * head);
  static LIter end();
  bool hasNext();
  bool not_empty();
  LIter next();
  T & value();
  LIter insert_value(T & value);
};

#endif

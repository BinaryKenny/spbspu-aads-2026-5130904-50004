#ifndef LIST_HPP
#define LIST_HPP

namespace khairullin
{
  template <class T>
  struct List
  {
    T val;
    List * next;
    List(const T& val, List * n);
    List * add(const T& val, List * n);
    List * insert(const T& val, List * n);
    List * cut(List * h);
    List * erase(List * h);
    List * clear(List * h);
    static List * fake(List * h);
    List * cut_fake(List * fake);
  };
}
#endif

#ifndef LIST_HPP
#define LIST_HPP

template <class T>
struct List
{
  T val;
  List<T> * next;
  List<T>(const T& val, List<T> * n);
  List<T> * add(const T& val, List<T> * n);
  List<T> * insert(const T& val, List<T> * n);
  List<T> * cut(List<T> * h);
  List<T> * erase(List<T> * h);
  List<T> * clear(List<T> * h);
  static List<T> * fake(List<T> * h);
  List<T> * cut_fake(List<T> * fake);
};

#endif

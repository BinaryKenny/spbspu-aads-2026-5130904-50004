#include <iostream>

template <class T>
struct List
{
  T val;
  List<T> * next, * prev;
  List<T>(const T& val, List<T> * n, List<T> * b);
  List<T> * add(const T& val, List<T> * n, List<T> * b);
  List<T> * insert(const T& val, List<T> * h);
  List<T> * cut(List<T> * h) noexcept;
  List<T> * erase(List<T> * h) noexcept;
  List<T> * clear(List<T> * h);
  void swap(List & other);
  List(List & other);
  List & operator=(List & other);
  List(List && other);
  List & operator=(List && other);
  ~List() = default;
};

int main(int argc, char ** argv)
{}

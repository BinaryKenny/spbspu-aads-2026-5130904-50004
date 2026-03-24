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

template<class T>
List<T>::List(const T& val, List<T> * n, List<T> * b):
val(val),
next(n),
prev(b)
{}

template<class T>
void List<T>::swap(List<T> & other)
{
  std::swap(val, other.val);
  std::swap(next, other.next);
  std::swap(prev, other.prev);
}

template<class T>
List<T>::List(List<T> & other):
val(other.val),
next(nullptr),
prev(nullptr)
{
  List<T> * head = this;
  List<T> * copy = other.next;
  List<T> * tail = head;
  while (copy)
  {
    try
    {
      tail = tail->insert(copy->val, tail);
      copy = copy->next;
    }
    catch(...)
    {
      clear(head);
      throw std::bad_alloc();
    }
  }
}

template<class T>
List<T> & List<T>::operator=(List<T> & other)
{
  if (this != &other)
  {
    try
    {
      List<T> list(other);
      swap(list);
    }
    catch(...)
    {
      throw std::exception();
    }
  }
  return *this;
}

template<class T>
List<T>::List(List<T> && other):
val(std::move(other.val)),
next(other.next),
prev(other.prev)
{
  other.next = nullptr;
  other.prev = nullptr;
}

template<class T>
List<T> & List<T>::operator=(List<T> && other)
{
  if (this != &other)
  {
    List<T> temp(std::move(other));
    swap(temp);
  }
  return *this;
}

int main(int argc, char ** argv)
{}

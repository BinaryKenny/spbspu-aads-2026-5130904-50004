#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <algorithm>
#include <stdexcept>
namespace khairullin
{
  template <class T>
  struct List
  {
    T val;
    List<T> * next, * prev;
    List(const T & val, List<T> * n, List<T> * b);
    List<T> * add(const T & val, List<T> * n, List<T> * b);
    List<T> * insert(const T & val, List<T> * h);
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
}

template<class T>
khairullin::List<T>::List(const T& val, khairullin::List<T> * n, khairullin::List<T> * b):
val(val),
next(n),
prev(b)
{}

template<class T>
void khairullin::List<T>::swap(khairullin::List<T> & other)
{
  std::swap(val, other.val);
  std::swap(next, other.next);
  std::swap(prev, other.prev);
}

template<class T>
khairullin::List<T>::List(khairullin::List<T> & other):
val(other.val),
next(nullptr),
prev(nullptr)
{
  khairullin::List<T> * head = this;
  khairullin::List<T> * copy = other.next;
  khairullin::List<T> * tail = head;
  while (copy)
  {
    try
    {
      tail = tail->insert(copy->val, tail);
      copy = copy->next;
    }
    catch(...)
    {
      if (head->next)
      {
        head->next->clear(head->next);
        head->next = nullptr;
      }
      throw std::bad_alloc();
    }
  }
}

template<class T>
khairullin::List<T> & khairullin::List<T>::operator=(khairullin::List<T> & other)
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
      throw;
    }
  }
  return *this;
}

template<class T>
khairullin::List<T>::List(khairullin::List<T> && other):
val(std::move(other.val)),
next(other.next),
prev(other.prev)
{
  other.next = nullptr;
  other.prev = nullptr;
}

template<class T >
khairullin::List<T> * khairullin::List<T>::add(const T & val, khairullin::List<T> * n, khairullin::List<T> * b)
{
  return new khairullin::List<T>{val, n, b};
}

template<class T>
khairullin::List<T> & khairullin::List<T>::operator=(khairullin::List<T> && other)
{
  if (this != &other)
  {
    List<T> temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template<class T>
khairullin::List<T> * khairullin::List<T>::insert(const T & val, khairullin::List<T> * h)
{
  h->next = add(val, h->next, h);
  if (h->next->next)
  {
    h->next->next->prev = h->next;
  }
  return h->next;
}

template<class T>
khairullin::List<T> * khairullin::List<T>::cut(khairullin::List<T> * h) noexcept
{
  List<T> * res = h->next;
  if (h->prev)
  {
    h->prev->next = h->next;
  }
  if (h->next)
  {
    h->next->prev = h->prev;
  }
  delete h;
  return res;
}

template<class T>
khairullin::List<T> * khairullin::List<T>::erase(khairullin::List<T> * h) noexcept
{
  return h->next = cut(h->next);
}

template<class T>
khairullin::List<T> * khairullin::List<T>::clear(khairullin::List<T> * h)
{
  while(h)
  {
    h = cut(h);
  }
  return h;
}

#endif

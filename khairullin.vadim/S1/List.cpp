#include "List.hpp"

template<class T>
khairullin::List<T> * khairullin::List<T>::fake(List<T> * h)
{
  List<T> * fake = static_cast<List<T> *>(::operator new (sizeof(List<T>)));
  fake->next = h;
  return fake;
}

template<class T>
khairullin::List<T> * khairullin::List<T>::cut_fake(List<T> * fake)
{
  List<T> * result = fake->next;
  ::operator delete(fake);
  return result;
}

template<class T>
khairullin::List<T>::List(const T& val, List<T> * n):
val(val),
next(n)
{}

template<class T>
khairullin::List<T> * khairullin::List<T>::add(const T& val, List<T> * n)
{
  return new List<T>(val, n);
}

template<class T>
khairullin::List<T> * khairullin::List<T>::insert(const T& val, List<T> * n)
{
  return n->next = add(val, n->next);
}

template<class T>
khairullin::List<T> * khairullin::List<T>::cut(List<T> * h)
{
  List<T> * result = h->next;
  delete h;
  return result;
}

template<class T>
khairullin::List<T> * khairullin::List<T>::erase(List<T> * h)
{
  return h->next = cut(h->next);
}

template<class T>
khairullin::List<T> * khairullin::List<T>::clear(List<T> * h)
{
  while (h != nullptr)
  {
     h = cut(h);
  }
  return h;
}

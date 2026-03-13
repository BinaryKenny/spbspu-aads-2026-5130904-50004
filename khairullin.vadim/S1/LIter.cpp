#include "LIter.hpp"

template <class T>
LIter<T>::LIter(List<T> * h):
list(h)
{}

template <class T>
LIter<T> LIter<T>::begin(List<T> * head)
{
  return LIter<T>(head);
}

template <class T>
LIter<T> LIter<T>::end()
{
  return LIter(nullptr);
}

template <class T>
bool LIter<T>::hasNext()
{
  if (list)
  {
    if (list->next)
    {
       return true;
    }
  }
  return false;
}

template<class T>
bool LIter<T>::not_empty()
{
  if (list)
  {
    return true;
  }
  else
  {
    return false;
  }
}

template <class T>
LIter<T> LIter<T>::next()
{
  return LIter(list->next);
}

template <class T>
T & LIter<T>::value()
{
  return list->val;
}

template<class T>
LIter<T> LIter<T>::insert_value(T & value)
{
  return LIter(list->insert(value, list));
}

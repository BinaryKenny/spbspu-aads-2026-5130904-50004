#include "CLIter.hpp"

template <class T>
CLIter<T>::CLIter(List<T> * h):
list(h)
{}

template <class T>
CLIter<T> CLIter<T>::begin(List<T> * head)
{
  return CLIter(head);
}

template <class T>
CLIter<T> CLIter<T>::end()
{
  return CLIter(nullptr);
}

template <class T>
bool CLIter<T>::hasNext()
{
  if (list->next)
  {
    return true;
  }
  return false;
}

template<class T>
bool CLIter<T>::not_empty()
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
CLIter<T> CLIter<T>::next()
{
  return CLIter(list->next);
}

template <class T>
T & CLIter<T>::value()
{
  return list->val;
}

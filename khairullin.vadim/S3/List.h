#ifndef LIST_H
#define LIST_H

namespace khairullin {
  template< class T >
  struct List {
    T value;
    List * next;

    List(const T & value, List * next);
    bool operator==(const List * other) const;

    List * add(const T & value, List * head);
    List * insert(const T & value, List * head);
    List * cut(List * head);
    List * clear(List * head);
    List * copy(List * head);
  };
}

template< class T >
khairullin::List< T >::List(const T & value, List * next):
  value(value),
  next(next)
{}

template< class T >
bool khairullin::List< T >::operator==(const List * other) const
{
  List< T > * List = *this;
  auto otherList = other;
  while (otherList && List) {
    if (List->value != otherList->value) {
      return false;
    }
    List = List->next;
    otherList = otherList->next;
  }
  if (List || otherList) {
    return false;
  }
  return true;
}

template< class T >
khairullin::List< T > * khairullin::List< T >::add(const T & value, List * head)
{
  return new List< T >(value, head);
}

template< class T >
khairullin::List< T > * khairullin::List< T >::insert(const T & value, List * head)
{
  return head->next = add(value, head->next);
}

template< class T >
khairullin::List< T > * khairullin::List< T >::cut(List * head)
{
  List * result = head->next;
  delete head;
  return result;
}

template< class T >
khairullin::List< T > * khairullin::List< T >::clear(List< T > * h)
{
  while (h != nullptr) {
    h = cut(h);
  }
  return h;
}

template< class T >
khairullin::List< T > * khairullin::List< T >::copy(khairullin::List< T > * head)
{
  List * result = new List(head->value, nullptr);
  List * result_tail = result;
  List * tail = head;
  while (tail != nullptr) {
    result_tail = result_tail->insert(tail->value, result_tail);
    tail = tail->next;
  }
  return result;
}
#endif

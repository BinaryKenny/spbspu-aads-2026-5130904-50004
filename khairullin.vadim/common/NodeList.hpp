#ifndef NODELIST_HPP
#define NODELIST_HPP
#include <algorithm>
#include <stdexcept>
namespace khairullin
{
  template< class T >
  struct NodeList
  {
    T val;
    NodeList * next, * prev;
    NodeList(const T & val, NodeList * n, NodeList * b);
    NodeList * add(const T & val, NodeList * n, NodeList * b);
    NodeList * insert(const T & val, NodeList * h);
    NodeList * cut(NodeList * h) noexcept;
    NodeList * erase(NodeList * h) noexcept;
    NodeList * clear(NodeList * h);
    void swap(NodeList & other);
    NodeList(const NodeList & other);
    NodeList & operator=(NodeList & other);
    NodeList(NodeList && other);
    NodeList & operator=(NodeList && other);
    ~NodeList() = default;
  };
}

template< class T >
khairullin::NodeList< T >::NodeList(const T & val, NodeList * n, NodeList * b):
val(val),
next(n),
prev(b)
{}

template< class T >
void khairullin::NodeList< T >::swap(NodeList & other)
{
  std::swap(val, other.val);
  std::swap(next, other.next);
  std::swap(prev, other.prev);
}

template<class T>
khairullin::NodeList< T >::NodeList(const NodeList & other):
val(other.val),
next(nullptr),
prev(nullptr)
{
  NodeList * head = this;
  NodeList * copy = other.next;
  NodeList * tail = head;
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

template< class T >
khairullin::NodeList< T > & khairullin::NodeList< T >::operator=(NodeList & other)
{
  if (this != &other)
  {
    NodeList list(other);
    swap(list);
  }
  return *this;
}

template< class T >
khairullin::NodeList< T >::NodeList(NodeList && other):
val(std::move(other.val)),
next(other.next),
prev(other.prev)
{
  other.next = nullptr;
  other.prev = nullptr;
}

template< class T >
khairullin::NodeList< T > * khairullin::NodeList< T >::add(const T & val, NodeList * n, NodeList * b)
{
  return new NodeList{val, n, b};
}

template<class T>
khairullin::NodeList< T > & khairullin::NodeList< T >::operator=(NodeList && other)
{
  if (this != &other)
  {
    NodeList temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template<class T>
khairullin::NodeList< T > * khairullin::NodeList< T >::insert(const T & val, NodeList * h)
{
  h->next = add(val, h->next, h);
  if (h->next->next)
  {
    h->next->next->prev = h->next;
  }
  return h->next;
}

template<class T>
khairullin::NodeList< T > * khairullin::NodeList< T >::cut(NodeList * h) noexcept
{
  NodeList * res = h->next;
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
khairullin::NodeList< T > * khairullin::NodeList< T >::erase(NodeList< T > * h) noexcept
{
  return h->next = cut(h->next);
}

template<class T>
khairullin::NodeList< T > * khairullin::NodeList< T >::clear(NodeList< T > * h)
{
  while(h)
  {
    h = cut(h);
  }
  return h;
}

#endif

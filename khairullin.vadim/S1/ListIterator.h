#ifndef LISTITERATOR_H
#define LISTITERATOR_H
#include "Node.h"
#include <iterator>

namespace khairullin {
  template< class T >
  struct ListIterator : public std::iterator<std::forward_iterator_tag, T>
  {

    ListIterator() noexcept;
    ListIterator(Node< T > * curr) noexcept;
    ~ListIterator() = default;
    ListIterator(const ListIterator & other) noexcept;
    ListIterator & operator=(const ListIterator & other) noexcept;
    ListIterator(ListIterator && other) noexcept;
    ListIterator & operator=(ListIterator && other) noexcept;
    ListIterator & operator++();
    ListIterator operator++(int);
    T & operator*() const;
    bool hasNext() const noexcept;
    void insert(const T & val);
    bool operator==(const ListIterator & other) const noexcept;
    bool operator!=(const ListIterator & other) const noexcept;

    private:
      Node< T > * current;
  };
}

template< class T >
khairullin::ListIterator< T >::ListIterator() noexcept:
  current(nullptr)
{}

template< class T >
khairullin::ListIterator< T >::ListIterator(Node< T > * curr) noexcept:
  current(curr)
{}

template< class T >
khairullin::ListIterator< T >::ListIterator(const ListIterator & other) noexcept:
  current(other.current)
{}

template< class T >
khairullin::ListIterator< T > & khairullin::ListIterator< T >::operator=(const ListIterator & other) noexcept
{
  if (*this == other) {
    return *this;
  }
  current = other.current;
  return *this;
}

template< class T >
khairullin::ListIterator< T >::ListIterator(ListIterator && other) noexcept:
  current(other.current)
{
  other.current = nullptr;
}

template< class T >
khairullin::ListIterator< T > & khairullin::ListIterator< T >::operator=(ListIterator && other) noexcept
{
  if (*this == other) {
    return *this;
  }
  current = other.current;
  other.current = nullptr;
  return *this;
}

template< class T >
khairullin::ListIterator< T > & khairullin::ListIterator< T >::operator++()
{
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator++() called on empty list");
  }
  current = current->next;
  return *this;
}

template< class T >
khairullin::ListIterator< T > khairullin::ListIterator< T >::operator++(int)
{
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator++() called on empty list");
  }
  auto copy = *this;
  current = current->next;
  return copy;
}

template< class T >
T & khairullin::ListIterator< T >::operator*() const
{
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator*() called on empty list");
  }
  return current->val;
}

template< class T >
bool khairullin::ListIterator< T >::hasNext() const noexcept
{
  if (current) {
    return current->next;
  }
  return false;
}

template< class T >
void khairullin::ListIterator< T >::insert(const T & val)
{
  auto next = current->next;
  auto new_node = new Node< T >(val);
  current->next = new_node;
  new_node->next = next;
}

template< class T >
bool khairullin::ListIterator< T >::operator==(const ListIterator & other) const noexcept
{
  return current == other.current;
}

template< class T >
bool khairullin::ListIterator< T >::operator!=(const ListIterator & other) const noexcept
{
  return !(*this == other);
}
#endif

#ifndef CONSTLISTITERATOR_H
#define CONSTLISTITERATOR_H
#include "Node.h"
#include <iterator>

namespace khairullin {
  template< class T >
  struct ConstListIterator : public std::iterator< std::forward_iterator_tag, const T >
  {
    ConstListIterator() noexcept;
    ConstListIterator(Node< T > * curr) noexcept;
    ~ConstListIterator() = default;
    ConstListIterator(const ConstListIterator & other) noexcept;
    ConstListIterator & operator=(const ConstListIterator & other) noexcept;
    ConstListIterator(ConstListIterator && other) noexcept;
    ConstListIterator & operator=(ConstListIterator && other) noexcept;
    ConstListIterator & operator++();
    ConstListIterator operator++(int);
    const T & operator*() const;
    bool hasNext() const noexcept;
    bool operator==(const ConstListIterator & other) const noexcept;
    bool operator!=(const ConstListIterator & other) const noexcept;

    private:
      Node< T > * current;
  };
}

template< class T >
khairullin::ConstListIterator< T >::ConstListIterator() noexcept:
  current(nullptr)
{}

template< class T >
khairullin::ConstListIterator< T >::ConstListIterator(Node< T > * curr) noexcept:
  current(curr)
{}

template< class T >
khairullin::ConstListIterator< T >::ConstListIterator(const ConstListIterator & other) noexcept:
  current(other.current)
{}

template< class T >
khairullin::ConstListIterator< T > & khairullin::ConstListIterator< T >::operator=
(const ConstListIterator & other) noexcept
{
  if (*this == other) {
    return *this;
  }
  current = other.current;
  return *this;
}

template< class T >
khairullin::ConstListIterator< T >::ConstListIterator(ConstListIterator && other) noexcept:
  current(other.current)
{
  other.current = nullptr;
}

template< class T >
khairullin::ConstListIterator< T > & khairullin::ConstListIterator< T >::operator=(
    ConstListIterator && other) noexcept
{
  if (*this == other) {
    return *this;
  }
  current = other.current;
  other.current = nullptr;
  return *this;
}

template< class T >
khairullin::ConstListIterator< T > & khairullin::ConstListIterator< T >::operator++()
{
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator++() called on empty list");
  }
  current = current->next;
  return *this;
}

template< class T >
khairullin::ConstListIterator< T > khairullin::ConstListIterator< T >::operator++(int)
{
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator++() called on empty list");
  }
  auto copy = *this;
  current = current->next;
  return copy;
}

template< class T >
const T & khairullin::ConstListIterator< T >::operator*() const
{
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator*() called on empty list");
  }
  return current->val;
}

template< class T >
bool khairullin::ConstListIterator< T >::hasNext() const noexcept
{
  if (current) {
    return current->next;
  }
  return false;
}

template< class T >
bool khairullin::ConstListIterator< T >::operator==(const ConstListIterator & other) const noexcept
{
  return current == other.current;
}

template< class T >
bool khairullin::ConstListIterator< T >::operator!=(const ConstListIterator & other) const noexcept
{
  return !(*this == other);
}
#endif

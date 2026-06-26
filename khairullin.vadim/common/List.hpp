#ifndef LIST_HPP
#define LIST_HPP
#include "Node.h"
#include "ConstListIterator.h"
#include "ListIterator.h"
#include <iostream>
#include <stdexcept>

namespace khairullin {
  template< class T >
  struct List
  {
    List();
    ~List() noexcept;
    List(const List & other);
    List & operator=(const List & other);
    List(List && other) noexcept;
    List & operator=(List && other) noexcept;
    bool operator==(const List & other) const;
    bool operator!=(const List & other) const;
    void push_front(const T & val);
    void push_back(const T & val);
    void insert(const T & val, const T & after);
    void cut(const T & val);
    void clear();
    ConstListIterator< T > cbegin() const noexcept;
    ConstListIterator< T > cend() const noexcept;
    ListIterator< T > begin() const noexcept;
    ListIterator< T > end() const noexcept;
    void swap(List & other) noexcept;

    private:
      Node< T > * fake;
      Node< T > * head;
  };

  template< class T >
  std::ostream & operator<<(std::ostream & os, const ListIterator< T > & iter);

  template< class T >
  std::ostream & operator<<(std::ostream & os, const ConstListIterator< T > & iter);
}

template< class T >
std::ostream & khairullin::operator<<(std::ostream & os, const ListIterator< T > & iter)
{
  return os << *iter;
}

template< class T >
std::ostream & khairullin::operator<<(std::ostream & os, const ConstListIterator< T > & iter)
{
  return os << *iter;
}

template< class T >
khairullin::List< T >::List():
fake(new Node< T >(T{})),
head(nullptr)
{
  fake->next = head;
}

template< class T >
khairullin::List< T >::~List() noexcept
{
  clear();
  delete fake;
}

template< class T >
khairullin::List< T >::List(const List & other):
  fake(new Node< T >(T{})),
  head(nullptr)
{
  Node< T > * beg = other.head;
  while (beg) {
    push_back(beg->val);
    beg = beg->next;
  }
}

template< class T >
khairullin::List< T > & khairullin::List< T >::operator=(const List & other)
{
  if (this == &other) {
    return *this;
  }
  auto temp(other);
  swap(temp);
  return *this;
}

template< class T >
khairullin::List< T >::List(List && other) noexcept:
  fake(new Node< T >(T{})),
  head(nullptr)
{
  swap(other);
}

template< class T >
khairullin::List< T > & khairullin::List< T >::operator=(List && other) noexcept
{
  if (this == &other) {
    return *this;
  }
  List temp(std::move(other));
  swap(temp);
  return *this;
}

template< class T >
bool khairullin::List< T >::operator==(const List & other) const
{
  auto iter1 = begin();
  auto iter2 = other.begin();
  while (iter1 != end() && iter2 != other.end()) {
    if (*iter1 != *iter2) {
      return false;
    }
    iter1++;
    iter2++;
  }
  if (iter1 == end() && iter2 == other.end()) {
    return true;
  }
  return false;
}

template< class T >
bool khairullin::List< T >::operator!=(const List & other) const
{
  return !(*this == other);
}

template< class T >
void khairullin::List< T >::push_front(const T & val)
{
  Node< T > * new_node = new Node< T >(val);
  fake->next = new_node;
  auto next = head;
  head = new_node;
  new_node->next = next;
}

template< class T >
void khairullin::List< T >::push_back(const T & val)
{
  Node< T > * new_node = new Node< T >(val);
  if (head == nullptr) {
    head = new_node;
    fake->next = new_node;
    return;
  }
  auto curr = head;
  while (curr->next != nullptr) {
    curr = curr->next;
  }
  curr->next = new_node;
}

template< class T >
void khairullin::List< T >::insert(const T & val, const T & after)
{
  Node< T > * curr = head;
  while (curr && curr->val != after) {
    curr = curr->next;
  }
  if (curr == nullptr) {
    throw std::logic_error("This value doesn't exist");
  }
  Node< T > * new_node = new Node< T >(val);
  new_node->next = curr->next;
  curr->next = new_node;
}

template< class T >
void khairullin::List< T >::cut(const T & val)
{
  Node< T > * curr = head;
  Node< T > * prev = fake;
  while (curr && curr->val != val) {
    prev = curr;
    curr = curr->next;
  }
  if (curr == nullptr) {
    throw std::logic_error("This value doesn't exist");
  }
  prev->next = curr->next;
  if (curr == head) {
    head = curr->next;
    fake->next = head;
  }
  curr->next = nullptr;
  delete curr;
}

template< class T >
void khairullin::List< T >::clear()
{
  while (head != nullptr) {
    auto next = head->next;
    delete head;
    head = next;
  }
  head = nullptr;
  fake->next = nullptr;
}

template< class T >
khairullin::ConstListIterator< T > khairullin::List< T >::cbegin() const noexcept
{
  return ConstListIterator< T >(head);
}

template< class T >
khairullin::ConstListIterator< T > khairullin::List< T >::cend() const noexcept
{
  return ConstListIterator< T >(nullptr);
}

template< class T >
khairullin::ListIterator< T > khairullin::List< T >::begin() const noexcept
{
  return ListIterator< T >(head);
}

template< class T >
khairullin::ListIterator< T > khairullin::List< T >::end() const noexcept
{
  return ListIterator< T >(nullptr);
}

template< class T >
void khairullin::List< T >::swap(List & other) noexcept
{
  std::swap(head, other.head);
  std::swap(fake, other.fake);
}
#endif

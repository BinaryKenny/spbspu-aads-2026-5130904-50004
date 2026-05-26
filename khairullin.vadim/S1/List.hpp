#ifndef LIST_HPP
#define LIST_HPP
#include "Node.h"
#include <iostream>
#include <stdexcept>
#include <iterator>

namespace khairullin{

  template< class T >
  struct ConstListIterator;
  template< class T >
  struct ListIterator;

  template< class T >
  struct List {
    List();
    ~List();
    List(const List &other);
    List & operator=(const List &other);
    List(List &&other) noexcept;
    List & operator=(List &&other);
    bool operator==(const List &other) const;
    bool operator!=(const List &other) const;
    void addBegin(const T &val);
    void addEnd(const T &val);
    void insert(const T &val, const T &after);
    void cut(const T & val);
    void clear();
    ConstListIterator< T > cbegin() const;
    ConstListIterator< T > cend() const;
    ListIterator< T > begin() const;
    ListIterator< T > end() const;
    void swap(List &other);
    private:
      Node< T > *fake = new Node< T >(T{});
      Node< T > *head = nullptr;
  };

  template< class T >
  struct ListIterator {

    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::forward_iterator_tag;

    ListIterator();
    ListIterator(Node< T > * curr);
    ~ListIterator() = default;
    ListIterator(const ListIterator & other);
    ListIterator & operator=(const ListIterator & other);
    ListIterator(ListIterator && other);
    ListIterator & operator=(ListIterator && other);
    ListIterator operator++();
    ListIterator operator++(int);
    T & operator*() const;
    bool hasNext() const;
    void insert(const T &val);
    bool operator==(const ListIterator & other) const;
    bool operator!=(const ListIterator & other) const;
    private:
      Node< T > * current;
  };

  template< class T >
  struct ConstListIterator {

    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;
    using iterator_category = std::forward_iterator_tag;

    ConstListIterator();
    ConstListIterator(Node< T > * curr);
    ~ConstListIterator() = default;
    ConstListIterator(const ConstListIterator & other);
    ConstListIterator & operator=(const ConstListIterator & other);
    ConstListIterator(ConstListIterator && other);
    ConstListIterator & operator=(ConstListIterator && other);
    ConstListIterator operator++();
    ConstListIterator operator++(int);
    const T & operator*() const;
    bool hasNext() const;
    bool operator==(const ConstListIterator & other) const;
    bool operator!=(const ConstListIterator & other) const;
    private:
      Node< T > * current;
  };

  template< class T >
  std::ostream & operator<<(std::ostream & os, const ListIterator< T > & iter);

  template< class T >
  std::ostream & operator<<(std::ostream & os, const ConstListIterator< T > & iter);
}

template< class T >
std::ostream& khairullin::operator<<(std::ostream& os, const ListIterator< T > & iter) {
  return os << *iter;
}

template< class T >
std::ostream & khairullin::operator<<(std::ostream& os, const ConstListIterator< T > & iter)
{
  return os << *iter;
}

template< class T >
khairullin::List< T >::List() {
  fake->next = head;
}

template< class T >
khairullin::List< T >::~List() {
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
    addEnd(beg->val);
    beg = beg->next;
  }
}

template< class T >
khairullin::List< T > & khairullin::List< T >::operator=(const List & other) {
  if (this == &other) {
    return *this;
  }
  clear();
  fake = new Node<T>(T{});
  head = nullptr;
  Node<T>* cur = other.head;
  while (cur) {
    addEnd(cur->val);
    cur = cur->next;
  }

  return *this;
}

template< class T >
khairullin::List< T >::List(List &&other) noexcept:
  fake(other.fake),
  head(other.head)
{
  other.head = nullptr;
  other.fake = new Node< T >(T{});
}

template< class T >
khairullin::List< T > & khairullin::List< T >::operator=(List &&other) {
  if (this == &other) {
    return *this;
  }

  clear();

  head = other.head;
  fake = other.fake;

  other.head = nullptr;
  other.fake = new Node<T>(T{});

  return *this;
}

template< class T >
bool khairullin::List<T>::operator==(const List & other) const
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
bool khairullin::List<T>::operator!=(const List & other) const
{
  return !(*this == other);
}

template< class T >
void khairullin::List< T >::addBegin(const T &val) {
  Node< T > *new_node = new Node< T >(val);
  fake->next = new_node;
  auto next = head;
  head = new_node;
  new_node->next = next;
}

template< class T >
void khairullin::List< T >::addEnd(const T &val) {
  Node< T > *new_node = new Node< T >(val);
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
void khairullin::List< T >::insert(const T &val, const T &after) {
  Node< T > *curr = head;
  while (curr && curr->val != after) {
    curr = curr->next;
  }
  if (curr == nullptr) {
    throw std::logic_error("This value doesn't exist");
  }
  Node< T > *new_node = new Node< T >(val);
  new_node->next = curr->next;
  curr->next = new_node;
}

template< class T >
void khairullin::List< T >::cut(const T & val) {
  Node< T > *curr = head;
  Node< T > *prev = fake;
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
void khairullin::List< T >::clear() {
  while (head != nullptr) {
    auto next = head->next;
    delete head;
    head = next;
  }
  head = nullptr;
  fake->next = nullptr;
}

template< class T >
khairullin::ConstListIterator<T> khairullin::List<T>::cbegin() const
{
  return ConstListIterator< T >(head);
}

template< class T >
khairullin::ConstListIterator< T > khairullin::List< T >::cend() const
{
  return ConstListIterator< T >(nullptr);
}

template< class T >
khairullin::ListIterator< T > khairullin::List< T >::begin() const
{
  return ListIterator< T >(head);
}

template< class T >
khairullin::ListIterator< T > khairullin::List< T >::end() const
{
  return ListIterator< T >(nullptr);
}

template< class T >
void khairullin::List< T >::swap(List & other) {
  std::swap(head, other.head);
  std::swap(fake, other.fake);
}

template< class T >
khairullin::ListIterator< T >::ListIterator():
  current(nullptr)
{}

template< class T >
khairullin::ListIterator< T >::ListIterator(Node< T > * curr):
  current(curr)
{}

template< class T >
khairullin::ListIterator< T >::ListIterator(const ListIterator & other):
  current(other.current)
{}

template< class T >
khairullin::ListIterator< T > & khairullin::ListIterator< T >::operator=(const ListIterator & other) {
  if (*this == other) {
    return *this;
  }
  current = other.current;
  return *this;
}

template< class T >
khairullin::ListIterator< T >::ListIterator(ListIterator &&other):
  current(other.current)
{
  other.current = nullptr;
}

template< class T >
khairullin::ListIterator< T > & khairullin::ListIterator< T >::operator=(ListIterator &&other) {
  if (*this == other) {
    return *this;
  }
  current = other.current;
  other.current = nullptr;
  return *this;
}

template< class T >
khairullin::ListIterator< T > khairullin::ListIterator< T >::operator++() {
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator++() called on empty list");
  }
  current = current->next;
  return *this;
}

template< class T >
khairullin::ListIterator< T > khairullin::ListIterator< T >::operator++(int) {
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator++() called on empty list");
  }
  auto copy = *this;
  current = current->next;
  return copy;
}

template< class T >
T & khairullin::ListIterator< T >::operator*() const {
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator*() called on empty list");
  }
  return current->val;
}

template< class T >
bool khairullin::ListIterator<T>::hasNext() const
{
  if (current) {
    return current->next;
  }
  return false;
}

template< class T >
void khairullin::ListIterator< T >::insert(const T &val) {
  auto next = current->next;
  auto new_node = new Node< T >(val);
  current->next = new_node;
  new_node->next = next;
}

template< class T >
bool khairullin::ListIterator<T>::operator==(const ListIterator & other) const
{
  return current == other.current;
}

template< class T >
bool khairullin::ListIterator<T>::operator!=(const ListIterator & other) const
{
  return !(*this == other);
}

template< class T >
khairullin::ConstListIterator< T >::ConstListIterator():
  current(nullptr)
{}

template< class T >
khairullin::ConstListIterator< T >::ConstListIterator(Node< T > * curr):
  current(curr)
{}

template< class T >
khairullin::ConstListIterator< T >::ConstListIterator(const ConstListIterator & other):
  current(other.current)
{}

template< class T >
khairullin::ConstListIterator< T > & khairullin::ConstListIterator< T >::operator=
  (const ConstListIterator & other) {
  if (*this == other) {
    return *this;
  }
  current = other.current;
  return *this;
}

template< class T >
khairullin::ConstListIterator< T >::ConstListIterator(ConstListIterator &&other):
  current(other.current)
{
  other.current = nullptr;
}

template< class T >
khairullin::ConstListIterator< T > & khairullin::ConstListIterator< T >::operator=(ConstListIterator &&other) {
  if (*this == other) {
    return *this;
  }
  current = other.current;
  other.current = nullptr;
  return *this;
}

template< class T >
khairullin::ConstListIterator< T > khairullin::ConstListIterator< T >::operator++() {
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator++() called on empty list");
  }
  current = current->next;
  return *this;
}

template< class T >
khairullin::ConstListIterator< T > khairullin::ConstListIterator< T >::operator++(int) {
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator++() called on empty list");
  }
  auto copy = *this;
  current = current->next;
  return copy;
}

template< class T >
const T & khairullin::ConstListIterator< T >::operator*() const {
  if (current == nullptr) {
    throw std::logic_error("ListIterator::operator*() called on empty list");
  }
  return current->val;
}

template< class T >
bool khairullin::ConstListIterator<T>::hasNext() const
{
  if (current) {
    return current->next;
  }
  return false;
}

template< class T >
bool khairullin::ConstListIterator<T>::operator==(const ConstListIterator & other) const
{
  return current == other.current;
}

template< class T >
bool khairullin::ConstListIterator<T>::operator!=(const ConstListIterator & other) const
{
  return !(*this == other);
}
#endif

#ifndef STACK_HPP
#define STACK_HPP
#include "NodeList.hpp"
namespace khairullin
{
template < class T >
  class Stack
  {
    public:
      void push(const T & rhs);
      void pop();
      T & top();
      T drop();
      size_t size() const;
      void swap(Stack & other);
      bool empty() const;
      Stack() = default;
      Stack(const T & rhs);
      Stack(Stack & other);
      Stack & operator=(Stack & other);
      Stack(Stack && other) noexcept;
      Stack & operator=(Stack && other) noexcept;
      ~Stack();
    private:
      NodeList<T> * head = nullptr;
  };
}

template< class T >
khairullin::Stack< T >::Stack(const T & rhs):
head(new NodeList< T >(rhs, nullptr, nullptr))
{}

template< class T >
void khairullin::Stack< T >::swap(Stack & other)
{
  std::swap(head, other.head);
}

template< class T >
bool khairullin::Stack< T >::empty() const
{
  return !head;
}

template< class T >
khairullin::Stack< T >::Stack(Stack & other):
head(nullptr)
{
  if (!other.empty())
  {
    try
    {
      head = new NodeList< T >(other.head->val, nullptr, nullptr);
      NodeList< T > * tail = head;
      NodeList< T > * copy = other.head->prev;
      while (copy)
      {
        tail->prev = new NodeList< T >(copy->val, tail, nullptr);
        tail = tail->prev;
        copy = copy->prev;
      }
    }
    catch(...)
    {
      throw std::bad_alloc();
    }
  }
}

template< class T >
khairullin::Stack< T > & khairullin::Stack< T >::operator=(Stack & other)
{
  if (this != &other)
  {
    Stack temp(other);
    swap(temp);
  }
  return *this;
}

template< class T >
khairullin::Stack< T >::Stack(Stack && other) noexcept:
head(other.head)
{
    other.head = nullptr;
}

template< class T >
khairullin::Stack< T > & khairullin::Stack<T>::operator=(Stack< T > && other) noexcept
{
  if (this != &other)
  {
    Stack temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template< class T >
khairullin::Stack< T >::~Stack()
{
  while (head)
  {
    NodeList< T > * prev = head->prev;
    delete head;
    head = prev;
    if (head)
    {
      head->next = nullptr;
    }
  }
}

template< class T >
void khairullin::Stack< T >::push(const T & rhs)
{
  try
  {
    if (head)
    {
      head = head->insert(rhs, head);
    }
    else
    {
      head = new NodeList< T >(rhs, nullptr, nullptr);
    }
  }
  catch(...)
  {
    throw;
  }
}

template< class T >
void khairullin::Stack<T>::pop()
{
  if (!head) {
    throw std::logic_error("The stack is empty");
  }
  NodeList< T > * prev = head->prev;
  delete head;
  head = prev;
  if (head) {
    head->next = nullptr;
  }
}

template< class T >
T & khairullin::Stack<T>::top()
{
  if (!head) {
    throw std::logic_error("The stack is empty");
  }
  return head->val;
}

template< class T >
T khairullin::Stack< T >::drop()
{
  try
  {
    if (!head)
    {
      throw std::logic_error("The stack is empty");
    }
    T val = head->val;
    NodeList< T > * prev = head->prev;
    delete head;
    head = prev;
    if (head)
    {
      head->next = nullptr;
    }
    return val;
  }
  catch(...)
  {
    throw;
  }
}

template< class T >
size_t khairullin::Stack<T>::size() const
{
  auto begin = head;
  size_t counter = 0;
  while (begin) {
    ++counter;
    begin = begin->prev;
  }
  return counter;
}
#endif

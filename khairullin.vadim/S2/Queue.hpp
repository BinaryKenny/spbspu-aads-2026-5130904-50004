#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "NodeList.hpp"
namespace khairullin
{
  template <class T>
  class Queue
  {
    public:
      void push(const T & rhs);
      T & front() const;
      T & back() const;
      T drop();
      void pop();
      size_t size() const;
      bool empty() const;
      void swap(Queue & other) noexcept;
      Queue(const T & rhs);
      Queue() = default;
      Queue(Queue & other);
      Queue & operator=(Queue & other);
      Queue(Queue && other) noexcept;
      Queue & operator=(Queue && other) noexcept;
      ~Queue();
    private:
      NodeList< T > * head = nullptr;
      NodeList< T > * tail = nullptr;
  };
}

template< class T >
khairullin::Queue< T >::Queue(const T & rhs):
head(new NodeList< T >(rhs, nullptr, nullptr)),
tail(head)
{}

template< class T >
bool khairullin::Queue< T >::empty() const
{
  return !head;
}

template< class T >
void khairullin::Queue< T >::swap(Queue & other) noexcept
{
  std::swap(head, other.head);
  std::swap(tail, other.tail);
}

template< class T >
khairullin::Queue< T >::Queue(Queue & other):
head(nullptr),
tail(nullptr)
{
  if (!other.empty())
  {
    try
    {
      head = new NodeList< T >(*other.head);
      tail = head;
      while (tail->next)
      {
        tail = tail->next;
      }
    }
    catch(...)
    {
      if (head)
      {
          head->clear(head);
      }
      throw;
    }
  }
}

template< class T >
khairullin::Queue< T > & khairullin::Queue< T >::operator=(Queue & other)
{
  if (this != & other)
  {
    try
    {
      Queue<T> temp(other);
      swap(temp);
    }
    catch(...)
    {
      throw;
    }
  }
  return *this;
}

template< class T >
khairullin::Queue< T >::Queue(Queue && other) noexcept:
head(nullptr),
tail(nullptr)
{
  head = std::move(other.head);
  tail = std::move(other.tail);
  other.head = nullptr;
  other.tail = nullptr;
}

template<class T>
khairullin::Queue< T > & khairullin::Queue< T >::operator=(Queue && other) noexcept
{
  if (this != &other)
  {
    Queue<T> temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template<class T>
void khairullin::Queue< T >::push(const T & rhs)
{
  try
  {
    if (tail)
    {
      tail = tail->insert(rhs, tail);
    }
    else
    {
      head = new NodeList< T >(rhs, nullptr, nullptr);
      tail = head;
    }
  }
  catch(...)
  {
    throw;
  }
}

template< class T >
T & khairullin::Queue<T>::front() const
{
  if (!head) {
    throw std::out_of_range("The queue is empty");
  }
  T & result = head->val;
  return result;
}

template< class T >
T & khairullin::Queue<T>::back() const
{
  if (!tail) {
    throw std::out_of_range("The queue is empty");
  }
  T & result = tail->val;
  return result;
}

template< class T >
T khairullin::Queue< T >::drop()
{
  try
  {
    if (!head)
    {
      throw std::logic_error("The queue is empty");
    }
    T val = head->val;
    NodeList< T > * next = head->next;
    delete head;
    head = next;
    if (!head)
    {
      tail = nullptr;
    }
    else
    {
      head->prev = nullptr;
    }
    return val;
  }
  catch(...)
  {
    throw;
  }
}

template< class T >
void khairullin::Queue<T>::pop()
{
  if (empty()) {
    throw std::out_of_range("The queue is empty");
  }
  NodeList< T > * next = head->next;
  delete head;
  head = next;
  if (empty()) {
    tail = nullptr;
  }
  else {
    head->prev = nullptr;
  }
}

template< class T >
size_t khairullin::Queue<T>::size() const
{
  auto begin = head;
  size_t counter = 0;
  while (begin != tail) {
    counter++;
    begin = begin->next;
  }
  return counter;
}

template<class T>
khairullin::Queue< T >::~Queue()
{
  if (head)
  {
     head->clear(head);
  }
}
#endif

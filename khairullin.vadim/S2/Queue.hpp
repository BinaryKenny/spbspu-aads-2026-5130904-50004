#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "List.hpp"
namespace khairullin
{
  template <class T>
  class Queue
  {
    public:
      void push(const T & rhs);
      T drop();
      const T & value() const;
      bool not_empty() const;
      void swap(Queue & other) noexcept;
      Queue(const T & rhs);
      Queue() = default;
      Queue(Queue & other);
      Queue & operator=(Queue & other);
      Queue(Queue && other) noexcept;
      Queue & operator=(Queue && other) noexcept;
      ~Queue();
    private:
      List<T> * head = nullptr;
      List<T> * tail = nullptr;
  };
}

template<class T>
khairullin::Queue<T>::Queue(const T & rhs):
head(new khairullin::List<T>(rhs, nullptr, nullptr)),
tail(head)
{}

template<class T>
bool khairullin::Queue<T>::not_empty() const
{
  return head;
}

template<class T>
const T & khairullin::Queue<T>::value() const
{
  return head->val;
}

template<class T>
void khairullin::Queue<T>::swap(khairullin::Queue<T> & other) noexcept
{
  std::swap(head, other.head);
  std::swap(tail, other.tail);
}

template<class T>
khairullin::Queue<T>::Queue(Queue<T> & other):
head(nullptr),
tail(nullptr)
{
  if (other.not_empty())
  {
    try
    {
      head = new List<T>(*other.head);
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

template<class T>
khairullin::Queue<T> & khairullin::Queue<T>::operator=(khairullin::Queue<T> & other)
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

template<class T>
khairullin::Queue<T>::Queue(khairullin::Queue<T> && other) noexcept:
head(nullptr),
tail(nullptr)
{
  head = std::move(other.head);
  tail = std::move(other.tail);
  other.head = nullptr;
  other.tail = nullptr;
}

template<class T>
khairullin::Queue<T> & khairullin::Queue<T>::operator=(khairullin::Queue<T> && other) noexcept
{
  if (this != &other)
  {
    Queue<T> temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template<class T>
void khairullin::Queue<T>::push(const T & rhs)
{
  try
  {
    if (tail)
    {
      tail = tail->insert(rhs, tail);
    }
    else
    {
      head = new List<T>(rhs, nullptr, nullptr);
      tail = head;
    }
  }
  catch(...)
  {
    throw;
  }
}

template<class T>
T khairullin::Queue<T>::drop()
{
  try
  {
    if (!head)
    {
      throw std::logic_error("The queue is empty");
    }
    T val = head->val;
    List<T> * next = head->next;
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

template<class T>
khairullin::Queue<T>::~Queue()
{
  if (head)
  {
     head->clear(head);
  }
}
#endif

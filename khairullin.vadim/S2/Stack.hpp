#ifndef STACK_HPP
#define STACK_HPP
#include "List.hpp"
namespace khairullin
{
template <class T>
  class Stack
  {
    public:
      void push(const T & rhs);
      T drop();
      const T & value() const;
      void swap(Stack & other);
      bool not_empty() const;
      Stack() = default;
      Stack(const T & rhs);
      Stack(Stack & other);
      Stack & operator=(Stack & other);
      Stack(Stack && other) noexcept;
      Stack & operator=(Stack && other) noexcept;
      ~Stack();
    private:
      List<T> * head = nullptr;
  };
}

template<class T>
khairullin::Stack<T>::Stack(const T & rhs):
head(new khairullin::List<T>(rhs, nullptr, nullptr))
{}

template<class T>
const T & khairullin::Stack<T>::value() const
{
  return head->val;
}

template<class T>
void khairullin::Stack<T>::swap(khairullin::Stack<T> & other)
{
  std::swap(head, other.head);
}

template<class T>
bool khairullin::Stack<T>::not_empty() const
{
  return head;
}

template<class T>
khairullin::Stack<T>::Stack(Stack<T> & other):
head(nullptr)
{
  if (other.not_empty())
  {
    try
    {
      head = new khairullin::List<T>(other.head->val, nullptr, nullptr);
      khairullin::List<T> * tail = head;
      khairullin::List<T> * copy = other.head->prev;
      while (copy)
      {
        tail->prev = new khairullin::List<T>(copy->val, tail, nullptr);
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

template<class T>
khairullin::Stack<T> & khairullin::Stack<T>::operator=(khairullin::Stack<T> & other)
{
  if (this != &other)
  {
    Stack<T> temp(other);
    swap(temp);
  }
  return *this;
}

template<class T>
khairullin::Stack<T>::Stack(khairullin::Stack<T> && other) noexcept:
head(other.head)
{
    other.head = nullptr;
}

template<class T>
khairullin::Stack<T> & khairullin::Stack<T>::operator=(khairullin::Stack<T> && other) noexcept
{
  if (this != &other)
  {
    Stack<T> temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template<class T>
khairullin::Stack<T>::~Stack()
{
  while (head)
  {
    khairullin::List<T> * prev = head->prev;
    delete head;
    head = prev;
    if (head)
    {
      head->next = nullptr;
    }
  }
}

template<class T>
void khairullin::Stack<T>::push(const T & rhs)
{
  try
  {
    if (head)
    {
      head = head->insert(rhs, head);
    }
    else
    {
      head = new List<T>(rhs, nullptr, nullptr);
    }
  }
  catch(...)
  {
    throw;
  }
}

template<class T>
T khairullin::Stack<T>::drop()
{
  try
  {
    if (!head)
    {
      throw std::logic_error("The stack is empty");
    }
    T val = head->val;
    List<T> * prev = head->prev;
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

#endif

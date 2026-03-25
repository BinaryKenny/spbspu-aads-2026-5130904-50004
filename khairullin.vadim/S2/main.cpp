#include <iostream>
#include <limits>
#include <fstream>
#include <cstddef>

namespace khairullin
{
  template <class T>
  struct List
  {
    T val;
    List<T> * next, * prev;
    List<T>(const T & val, List<T> * n, List<T> * b);
    List<T> * add(const T & val, List<T> * n, List<T> * b);
    List<T> * insert(const T & val, List<T> * h);
    List<T> * cut(List<T> * h) noexcept;
    List<T> * erase(List<T> * h) noexcept;
    List<T> * clear(List<T> * h);
    void swap(List & other);
    List(List & other);
    List & operator=(List & other);
    List(List && other);
    List & operator=(List && other);
    ~List() = default;
  };
}

using khairullin::List;

template<class T>
List<T>::List(const T& val, List<T> * n, List<T> * b):
val(val),
next(n),
prev(b)
{}

template<class T>
void List<T>::swap(List<T> & other)
{
  std::swap(val, other.val);
  std::swap(next, other.next);
  std::swap(prev, other.prev);
}

template<class T>
List<T>::List(List<T> & other):
val(other.val),
next(nullptr),
prev(nullptr)
{
  List<T> * head = this;
  List<T> * copy = other.next;
  List<T> * tail = head;
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

template<class T>
List<T> & List<T>::operator=(List<T> & other)
{
  if (this != &other)
  {
    try
    {
      List<T> list(other);
      swap(list);
    }
    catch(...)
    {
      throw;
    }
  }
  return *this;
}

template<class T>
List<T>::List(List<T> && other):
val(std::move(other.val)),
next(other.next),
prev(other.prev)
{
  other.next = nullptr;
  other.prev = nullptr;
}

template<class T >
List<T> * List<T>::add(const T & val, List<T> * n, List<T> * b)
{
  return new List<T>{val, n, b};
}

template<class T>
List<T> & List<T>::operator=(List<T> && other)
{
  if (this != &other)
  {
    List<T> temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template<class T>
List<T> * List<T>::insert(const T & val, List<T> * h)
{
  h->next = add(val, h->next, h);
  if (h->next->next)
  {
    h->next->next->prev = h->next;
  }
  return h->next;
}

template<class T>
List<T> * List<T>::cut(List<T> * h) noexcept
{
  List<T> * res = h->next;
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
List<T> * List<T>::erase(List<T> * h) noexcept
{
  return h->next = cut(h->next);
}

template<class T>
List<T> * List<T>::clear(List<T> * h)
{
  while(h)
  {
    h = cut(h);
  }
  return h;
}


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

using khairullin::Queue;

template<class T>
Queue<T>::Queue(const T & rhs):
head(new List<T>(rhs, nullptr, nullptr)),
tail(head)
{}

template<class T>
bool Queue<T>::not_empty() const
{
  return head;
}

template<class T>
const T & Queue<T>::value() const
{
  return head->val;
}

template<class T>
void Queue<T>::swap(Queue<T> & other) noexcept
{
  std::swap(head, other.head);
  std::swap(tail, other.tail);
}

template<class T>
Queue<T>::Queue(Queue<T> & other):
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
Queue<T> & Queue<T>::operator=(Queue<T> & other)
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
Queue<T>::Queue(Queue<T> && other) noexcept:
head(nullptr),
tail(nullptr)
{
  head = std::move(other.head);
  tail = std::move(other.tail);
  other.head = nullptr;
  other.tail = nullptr;
}

template<class T>
Queue<T> & Queue<T>::operator=(Queue<T> && other) noexcept
{
  if (this != &other)
  {
    Queue<T> temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template<class T>
void Queue<T>::push(const T & rhs)
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
T Queue<T>::drop()
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
Queue<T>::~Queue()
{
  if (head)
  {
     head->clear(head);
  }
}


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

using khairullin::Stack;

template<class T>
Stack<T>::Stack(const T & rhs):
head(new List<T>(rhs, nullptr, nullptr))
{}

template<class T>
const T & Stack<T>::value() const
{
  return head->val;
}

template<class T>
void Stack<T>::swap(Stack<T> & other)
{
  std::swap(head, other.head);
}

template<class T>
bool Stack<T>::not_empty() const
{
  return head;
}

template<class T>
Stack<T>::Stack(Stack<T> & other):
head(nullptr)
{
  if (other.not_empty())
  {
    try
    {
      head = new List<T>(other.head->val, nullptr, nullptr);
      List<T> * tail = head;
      List<T> * copy = other.head->prev;
      while (copy)
      {
        tail->prev = new List<T>(copy->val, tail, nullptr);
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
Stack<T> & Stack<T>::operator=(Stack<T> & other)
{
  if (this != &other)
  {
     Stack<T> temp(other);
     swap(temp);
  }
  return *this;
}

template<class T>
Stack<T>::Stack(Stack<T> && other) noexcept:
head(other.head)
{
    other.head = nullptr;
}

template<class T>
Stack<T> & Stack<T>::operator=(Stack<T> && other) noexcept
{
  if (this != &other)
  {
    Stack<T> temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template<class T>
Stack<T>::~Stack()
{
  while (head)
  {
    List<T> * prev = head->prev;
    delete head;
    head = prev;
    if (head)
    {
      head->next = nullptr;
    }
  }
}

template<class T>
void Stack<T>::push(const T & rhs)
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
T Stack<T>::drop()
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

namespace khairullin
{
  struct Data
  {
    Data(const int & temp);
    Data(const char & temp);
    Data() = default;
    const bool is_int() const ;
    const int value() const ;
    const char char_value() const ;
  private:
    int int_value;
    bool is_integer;
    char symbol;
  };
}

using khairullin::Data;

Data::Data(const int & temp):
int_value(temp),
is_integer(true),
symbol(' ')
{}

Data::Data(const char & temp):
int_value(0),
is_integer(false),
symbol(temp)
{}

const bool Data::is_int() const
{
  return is_integer;
}

const int Data::value() const
{
  return int_value;
}

const char Data::char_value() const
{
  return symbol;
}

namespace khairullin
{
  bool is_char(const char c)
  {
    bool cond = c == '(';
    cond = cond || c == ')';
    cond = cond || c == '+';
    cond = cond || c == '-';
    cond = cond || c == '*';
    cond = cond || c == '/';
    cond = cond || c == '%';
    cond = cond || c == '#';
    return cond;
  }

  Queue<Data> input(std::string line)
  {
    Queue<Data> q{};
    size_t i = 0;
    while(i < line.length())
    {
      if (i >= line.length())
      {
        break;
      }
      char temp = line[i];
      if (is_char(temp))
      {
        q.push(Data(temp));
        i++;
      }
      else
      {
        int num = 0;
        std::string number = "";
        while(line[i] != ' ' && i < line.length())
        {
          number += line[i];
          i++;
        }
        try
        {
          num = std::stoi(number);
        }
        catch(...)
        {
          throw std::out_of_range("Overflow or undeflow");
        }
          q.push(Data(num));
      }
      if (i < line.length() && line[i] == ' ')
      {
        i++;
      }
    }
    return q;
  }

  size_t get_priority(const char& op)
  {
    if (op == '+' || op == '-')
    {
      return 1;
    }
    if (op == '*' || op == '/' || op == '%')
    {
      return 2;
    }
    if (op == '#')
    {
      return 3;
    }
    return 0;
  }

  bool priority(const char & op1, const char & op2)
  {
    return get_priority(op1) >= get_priority(op2);
  }

  void postfix(Queue<Data> & q, Queue<Data> & q1) {
    Stack<Data> s2;
    while (q.not_empty())
    {
      Data val = q.drop();
      if (val.is_int())
      {
        q1.push(val);
      }
      else if (val.char_value() == '(')
      {
        s2.push(val);
      }
      else if (val.char_value() == ')' )
      {
        while (s2.not_empty() && s2.value().char_value() != '(')
        {
          q1.push(s2.drop());
        }
        if (s2.not_empty() && s2.value().char_value() == '(')
        {
          s2.drop();
        }
      }
      else
      {
        while (s2.not_empty() && s2.value().char_value() != '(' && priority(s2.value().char_value(), val.char_value()))
        {
          q1.push(s2.drop());
        }
        s2.push(val);
      }
    }
    while (s2.not_empty())
    {
      q1.push(s2.drop());
    }
  }

  int degree(int n1, int n2)
  {
    int result = 1;
    while (n2 != 0)
    {
      result *= n1;
      n2--;
    }
    return result;
  }
  int reverse(int number)
  {
    int result = 0;
    int sign = number >= 0 ? 1 : -1;
    number = number * sign;
    size_t counter = 0;
    int temp[10] = {0};
    while (number)
    {
      temp[counter] = number % 10;
      counter++;
      number = number / 10;
    }
    size_t id = 0;
    int grade = static_cast<int>(counter - 1);
    while(id < counter)
    {
      result += temp[id] * khairullin::degree(10, grade);
      id++;
      grade--;
    }
    return sign * result;
  }
}
int main(int argc, char ** argv)
{
  std::string line = "";
  std::ifstream file;
  std::istream * in = &std::cin;
  if (argc > 1)
  {
    file.open(argv[1]);
    in = &file;
  }
  Stack<int> output_stack;

  constexpr int MAX = std::numeric_limits<int>::max();
  constexpr int MIN = std::numeric_limits<int>::min();
    
  while (std::getline(*in, line))
  {
    Queue<Data> input_queue;
    try
    {
      input_queue = khairullin::input(line);
    }
    catch (const std::exception & err)
    {
      std::cerr << err.what() << "\n";
      return 1;
    }
    if (!input_queue.not_empty())
    {
      continue;
    }
    Queue<Data> postfix_queue;
    try
    {
      khairullin::postfix(input_queue, postfix_queue);
    }
    catch (const std::bad_alloc & err)
    {
      std::cerr << err.what() << "\n";
      return 2;
    }
      
    Stack<Data> res_stack;
    try
    {
      while (postfix_queue.not_empty())
      {
        while(postfix_queue.value().is_int())
        {
          res_stack.push(postfix_queue.drop());
        }
        char oper = postfix_queue.drop().char_value();
        if (oper == '#')
        {
          int number = res_stack.drop().value();
          res_stack.push(Data(khairullin::reverse(number)));
        }
        else if (oper == '+')
        {
          int val_1 = res_stack.drop().value();
          int val_2 = res_stack.drop().value();
          int result = 0;
          if (MAX - val_1 > val_2)
          {
            result = val_1 + val_2;
            res_stack.push(Data(result));
          }
          else
          {
            std::cerr << "Overflow\n";
            return 2;
          }
        }
        else if (oper == '-')
        {
          int val_1 = res_stack.drop().value();
          int val_2 = res_stack.drop().value();
          int result = 0;
          if (val_1 > 0)
          {
            if (MIN + val_1 < val_2)
            {
              result = val_2 - val_1;
              res_stack.push(Data(result));
            }
            else
            {
              std::cerr << "Underflow\n";
              return 2;
            }
          }
          else
          {
            if (MAX + val_1 > val_2 )
            {
              result = val_2 - val_1;
              res_stack.push(Data(result));
            }
            else
            {
              std::cerr << "Overflow\n";
              return 2;
            }
          }
        }
        else if (oper == '*')
        {
          int val_1 = res_stack.drop().value();
          int val_2 = res_stack.drop().value();
          int result = 0;
          if (MAX / val_1 > val_2)
          {
            result = val_1 * val_2;
            res_stack.push(Data(result));
          }
          else
          {
            std::cerr << "Overflow\n";
            return 2;
          }
        }
        else if (oper == '/')
        {
          int val_1 = res_stack.drop().value();
          int val_2 = res_stack.drop().value();
          if (val_1 != 0)
          {
            int result = val_2 / val_1;
            res_stack.push(Data(result));
          }
          else
          {
            std::cerr << "Divide 0\n";
            return 1;
          }
        }
        else if (oper == '%')
        {
          int val_1 = res_stack.drop().value();
          int val_2 = res_stack.drop().value();
          if (val_1 != 0)
          {
            int result = val_2 % val_1;
            res_stack.push(Data(result));
          }
          else
          {
            std::cerr << "Divide 0\n";
            return 1;
          }
        }
        else
        {
          std::cerr << "Fail input\n";
          return 1;
        }
      }
      output_stack.push(res_stack.drop().value());
      if (res_stack.not_empty())
      {
        std::cerr << "Wrong\n";
        return 3;
      }
    }
    catch (...)
    {
      std::cerr << "Exception\n";
      return 2;
    }
  }
  if (output_stack.not_empty())
  {
    std::cout << output_stack.drop();
  }
  while (output_stack.not_empty())
  {
    std::cout << " " << output_stack.drop();
  }
  std::cout << "\n";
}

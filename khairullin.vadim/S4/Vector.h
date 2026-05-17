#ifndef VECTOR_H
#define VECTOR_H
#include <stdexcept>
#include <utility>
#include <vector>

namespace khairullin
{
  template< class T >
  struct Iterator;

  template< class T >
  struct CIterator;

  template< class T >
  struct Vector {
    Vector();
    ~Vector();
    Vector(size_t size, const T & value);
    Vector(const Vector< T > & rhs);
    Vector(Vector< T > && rhs) noexcept;
    Vector< T > & operator=(const Vector< T > & rhs);
    Vector< T > & operator=(Vector< T > && rhs) noexcept;
    void swap(Vector< T > & rhs) noexcept;

    T & operator[](size_t id) noexcept;
    T & at(size_t id);
    const T & operator[](size_t id) const noexcept;
    const T & at(size_t id) const;
    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    bool operator==(const Vector< T > & rhs) const noexcept;
    bool operator!=(const Vector< T > & rhs) const noexcept;

    void insert(size_t id, const T & t);
    void insert(size_t id, const Vector< T > & rhs, size_t beg, size_t end);
    void insert(Iterator< T > pos, const T & value);
    void insert(Iterator< T > pos, CIterator< T > begin, CIterator< T > end);
    void insert(Iterator< T > pos, const T & value, size_t k);

    void erase(size_t id);
    void erase(size_t beg, size_t end);
    void erase(Iterator< T > pos);
    void erase(Iterator< T > begin, Iterator< T > end);
    void erase(CIterator< T > begin, size_t k);

    void pushBack(const T &);
    void popBack();
    void pushFront(const T &);

    T max();
    T min();
    std::pair< bool, size_t > has(const T & value);

    Iterator< T > begin();
    Iterator< T > end();
    Iterator< T > iterator(size_t i);
    CIterator< T > cbegin();
    CIterator< T > cend();
    CIterator< T > citerator(size_t i);

    private:
      explicit Vector(size_t size);
      T * data;
      size_t size_, capacity;
  };

  template< class T >
  struct Iterator {
    Iterator(Vector< T > &, size_t);
    ~Iterator() = default;
    void operator+=(size_t i);
    void operator-=(size_t i);
    Iterator operator++();
    Iterator operator++(int);
    Iterator operator--();
    Iterator operator--(int);
    void write(const T & value);
    void cut();
    T & operator*() const noexcept;
    bool operator==(const Iterator< T > & rhs) const noexcept;
    bool operator!=(const Iterator< T > & rhs) const noexcept;

    Vector< T > & vector;
    size_t id;
  };

  template< class T >
  struct CIterator {
    CIterator(Vector< T > &, size_t);
    ~CIterator() = default;
    void operator+=(size_t i);
    void operator-=(size_t i);
    CIterator operator++();
    CIterator operator++(int);
    CIterator operator--();
    CIterator operator--(int);
    T & read();
    T & operator*() const noexcept;
    bool operator==(const CIterator< T > & rhs) const noexcept;
    bool operator!=(const CIterator< T > & rhs) const noexcept;

    Vector< T > & vector;
    size_t id;
  };
}

template< class T >
khairullin::Vector< T >::Vector():
  data(nullptr),
  size_(0),
  capacity(0)
{
}

template< class T >
khairullin::Vector< T >::~Vector()
{
  delete [] data;
}

template< class T >
khairullin::Vector< T >::Vector(size_t size, const T & value):
  Vector(size)
{
  for (size_t i = 0; i < size; i++) {
    data[i] = value;
  }
}

template< class T >
khairullin::Vector< T >::Vector(size_t size):
  data(size ? new T[size * 2]() : nullptr),
  size_(size),
  capacity(size * 2)
{
}

template< class T >
khairullin::Vector< T >::Vector(const Vector< T > & rhs):
  Vector(rhs.getSize())
{
  for (size_t i = 0; i < rhs.getSize(); i++)
  {
    data[i] = rhs.data[i];
  }
}

template< class T >
khairullin::Vector< T >::Vector(Vector< T > && rhs) noexcept:
  data(rhs.data),
  size_(rhs.size_),
  capacity(rhs.capacity)
{
  rhs.data = nullptr;
}

template< class T >
khairullin::Vector< T > & khairullin::Vector< T >::operator=(const Vector< T > & rhs)
{
  if (this == std::addressof(rhs))   //для взятия именно адреса
  {
    return *this;
  }
  Vector< T > cpy(rhs);
  swap(cpy);
  return *this;
}

template< class T >
khairullin::Vector< T > & khairullin::Vector< T >::operator=(Vector< T > && rhs) noexcept
{
  if (this == &rhs) {
    return *this;
  }
  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template< class T >
void khairullin::Vector< T >::swap(Vector & rhs) noexcept
{
  std::swap(data, rhs.data);
  std::swap(size_, rhs.size_);
  std::swap(capacity, rhs.capacity);
}

template< class T >
T & khairullin::Vector< T >::operator[](size_t id) noexcept
{
  return data[id];
}

template< class T >
T & khairullin::Vector< T >::at(size_t id)
{
  const Vector< T > * cthis = this;
  const T & cr = cthis->at(id);
  T & r = const_cast< T & >(cr);
  return r;
}

template< class T >
const T & khairullin::Vector< T >::operator[](size_t id) const noexcept
{
  return data[id];
}

template< class T >
const T & khairullin::Vector< T >::at(size_t id) const
{
  if (id < getSize()) {
    return (*this)[id];
  }
  throw std::out_of_range("id out of bound");
}

template< class T >
bool khairullin::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template< class T >
size_t khairullin::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t khairullin::Vector< T >::getCapacity() const noexcept
{
  return capacity;
}

template< class T >
bool khairullin::Vector< T >::operator==(const Vector< T > & rhs) const noexcept
{
  if (getSize() != rhs.getSize()) {
    return false;
  }
  for (size_t i = 0; i < getSize(); i++) {
    if (data[i] != rhs.data[i]) {
      return false;
    }
  }
  return true;
}

template< class T >
bool khairullin::Vector< T >::operator!=(const Vector< T > & rhs) const noexcept
{
  return !(*this == rhs);
}

template< class T >
void khairullin::Vector< T >::insert(size_t id, const T & t)
{
  if (id > size_ - 1) {
    throw std::out_of_range("id out of bound");
  }
  Vector< T > new_vec(size_ + 1);
  size_t index = 0;
  for (size_t i = 0; i < size_ + 1; i++) {
    try {
      if (i == id) {
        new_vec[i] = t;
      } else {
        new_vec[i] = (*this)[index];
        index++;
      }
    } catch (...) {
      throw std::bad_alloc();
    }
  }
  swap(new_vec);
}

template< class T >
void khairullin::Vector< T >::insert(size_t id, const Vector< T > & rhs, size_t beg, size_t end)
{
  if (id > size_ - 1) {
    throw std::out_of_range("id out of bound");
  }
  size_t sizeOfSegment = end - beg;
  Vector< T > segment(sizeOfSegment);
  Vector< T > result(size_ + sizeOfSegment);
  try {
    for (size_t i = 0; i < sizeOfSegment; i++) {
      segment[i] = rhs[beg + i];
    }
    for (size_t i = 0; i < id; i++) {
      result[i] = rhs[i];
    }
    for (size_t i = id; i < id + sizeOfSegment; i++) {
      result[i] = segment[i - id];
    }
    for (size_t i = id + sizeOfSegment; i < size_ + sizeOfSegment; i++) {
      result[i] = rhs[i - sizeOfSegment];
    }
  } catch (...) {
    throw std::bad_alloc();
  }
  swap(result);
}

template< class T >
void khairullin::Vector< T >::insert(Iterator< T > pos, const T & value)
{
  pos.vector.insert(pos.id, value);
}

template< class T >
void khairullin::Vector< T >::insert(Iterator< T > pos, CIterator< T > begin, CIterator< T > end)
{
  pos.vector.insert(pos.id, begin.vector, begin.id, end.id);
}

template< class T >
void khairullin::Vector< T >::insert(Iterator< T > pos, const T & value, size_t k)
{
  Vector< T > copy_v(size_ + k);
  try {
    for (size_t i = 0; i < pos.id; i++) {
      copy_v[i] = (*this)[i];
    }
    for (size_t i = pos.id; i < pos.id + k; i++) {
      copy_v[i] = value;
    }
    for (size_t i = pos.id + k; i < size_ + k; i++) {
      copy_v[i] = (*this)[i - k];
    }
  } catch (...) {
    throw std::bad_alloc();
  }
  swap(copy_v);
}

template< class T >
void khairullin::Vector< T >::erase(size_t id)
{
  if (id > size_ - 1) {
    throw std::out_of_range("id out of bound");
  }
  Vector< T > copy_v(size_ - 1);
  size_t index = 0;
  for (size_t i = 0; i < size_; i++) {
    if (i == id) {
      continue;
    }
    try {
      copy_v[index] = (*this)[i];
      index++;
    } catch (...) {
      throw std::bad_alloc();
    }
  }
  swap(copy_v);
}

template< class T >
void khairullin::Vector< T >::erase(size_t beg, size_t end)
{
  if (beg > size_ - 1 || end > size_) {
    throw std::out_of_range("id is out of bound");
  }
  size_t sizeOfSegment = end - beg;
  Vector< T > copy_v(size_ - sizeOfSegment);
  size_t index = 0;
  try {
    for (size_t i = 0; i < size_; i++) {
      if (i < beg || i >= end) {
        copy_v[index] = (*this)[i];
        index++;
      } else {
        continue;
      }
    }
  } catch (...) {
    throw std::bad_alloc();
  }
  swap(copy_v);
}

template< class T >
void khairullin::Vector< T >::erase(Iterator< T > pos)
{
  erase(pos.id);
}

template< class T >
void khairullin::Vector< T >::erase(Iterator< T > begin, Iterator< T > end)
{
  erase(begin.id, end.id);
}

template< class T >
void khairullin::Vector< T >::erase(CIterator< T > begin, size_t k)
{
  if (k + begin.id > size_) {
    throw std::out_of_range("Too many element must be deleted");
  }
  if (*this != begin.vector) {
    throw std::logic_error("Iterator indicates to other vector");
  }
  Vector< T > v(size_ - k);
  auto it = v.begin();
  auto main_iter = cbegin();
  for (; main_iter != begin; main_iter += 1) {
    *it = *main_iter;
    it += 1;
  }
  main_iter += k;
  for (; main_iter != cend(); main_iter += 1) {
    *it = *main_iter;
    it += 1;
  }
  swap(v);
}

template< class T >
void khairullin::Vector< T >::pushBack(const T & value)
{
  if (size_ == capacity) {
    T * new_data = nullptr;
    try {
      new_data = new T[capacity * 2 + 1];
      capacity = capacity * 2 + 1;
      for (size_t i = 0; i < size_; i++) {
        new_data[i] = data[i];
      }
      delete [] data;
      data = new_data;
    } catch (...) {
      delete new_data;
      throw;
    }
  }
  data[size_++] = value;
}

template< class T >
void khairullin::Vector< T >::popBack()
{
  if (size_ == 0) {
    throw std::logic_error("Logic error: vector is empty");
  }
  size_--;
}

template< class T >
void khairullin::Vector< T >::pushFront(const T & t)
{
  Vector< T > v(getSize() + 1);
  v[0] = t;
  {
    for (size_t i = 1; i < v.getSize(); ++i) {
      v[i] = (*this)[i - 1];
    }
  }
  swap(v);
}

template< class T >
T khairullin::Vector< T >::max()
{
  auto maximum = data[0];
  for (size_t i = 1; i < size_; i++) {
    if (data[i] > maximum) {
      maximum = data[i];
    }
  }
  return maximum;
}

template< class T >
T khairullin::Vector< T >::min()
{
  auto minimum = data[0];
  for (size_t i = 1; i < size_; i++) {
    if (data[i] < minimum) {
      minimum = data[i];
    }
  }
  return minimum;
}

template< class T >
std::pair< bool, size_t > khairullin::Vector< T >::has(const T & value)
{
  for (size_t i = 0; i < size_ - 1; i++) {
    if (data[i] == value) {
      return std::make_pair(true, i);
    }
  }
  return std::make_pair(false, 0);
}

template< class T >
khairullin::Iterator< T > khairullin::Vector< T >::begin()
{
  return Iterator< T >(*this, 0);
}

template< class T >
khairullin::Iterator< T > khairullin::Vector< T >::end()
{
  return Iterator< T >(*this, size_);
}

template< class T >
khairullin::Iterator< T > khairullin::Vector< T >::iterator(size_t i)
{
  return Iterator< T >(*this, i);
}

template< class T >
khairullin::CIterator< T > khairullin::Vector< T >::cbegin()
{
  return CIterator< T >(*this, 0);
}

template< class T >
khairullin::CIterator< T > khairullin::Vector< T >::cend()
{
  return CIterator< T >(*this, size_);
}

template< class T >
khairullin::CIterator< T > khairullin::Vector< T >::citerator(size_t i)
{
  return CIterator< T >(*this, i);
}

template< class T >
khairullin::Iterator< T >::Iterator(Vector< T > & v, size_t i):
  vector(v),
  id(i)
{
}

template< class T >
void khairullin::Iterator< T >::operator+=(size_t i)
{
  if (id + i > vector.getSize()) {
    throw std::out_of_range("Iterator is out of bound");
  }
  id += i;
}

template< class T >
void khairullin::Iterator< T >::operator-=(size_t i)
{
  if (id + 1 - i < 0) {
    throw std::out_of_range("Iterator is out of bound");
  }
  id -= i;
}

template< class T >
khairullin::Iterator< T > khairullin::Iterator< T >::operator++()
{
  if (id + 1 >= vector.getSize()) {
    throw std::out_of_range("Iterator is out of bound");
  }
  id++;
  return *this;
}

template< class T >
khairullin::Iterator< T > khairullin::Iterator< T >::operator++(int)
{
  if (id + 1 >= vector.getSize()) {
    throw std::out_of_range("Iterator is out of bound");
  }
  auto copy = *this;
  id++;
  return copy;
}

template< class T >
khairullin::Iterator< T > khairullin::Iterator< T >::operator--()
{
  if (id - 1 < 0) {
    throw std::out_of_range("Iterator is out of bound");
  }
  id--;
  return *this;
}

template< class T >
khairullin::Iterator< T > khairullin::Iterator< T >::operator--(int)
{
  if (id - 1 < 0) {
    throw std::out_of_range("Iterator is out of bound");
  }
  Iterator< T > copy = *this;
  id--;
  return copy;
}

template< class T >
void khairullin::Iterator< T >::write(const T & value)
{
  vector[id] = value;
}

template< class T >
void khairullin::Iterator< T >::cut()
{
  vector.erase(id);
}

template< class T >
T & khairullin::Iterator< T >::operator*() const noexcept
{
  return vector[id];
}

template< class T >
bool khairullin::Iterator< T >::operator==(const Iterator< T > & rhs) const noexcept
{
  return (*this).vector == rhs.vector && (*this).id == rhs.id;
}

template< class T >
bool khairullin::Iterator< T >::operator!=(const Iterator< T > & rhs) const noexcept
{
  return !(*this == rhs);
}

template< class T >
khairullin::CIterator< T >::CIterator(Vector< T > & v, size_t i):
  vector(v),
  id(i)
{
}

template< class T >
void khairullin::CIterator< T >::operator+=(size_t i)
{
  if (id + i > vector.getSize()) {
    throw std::out_of_range("Const Iterator is out of bound");
  }
  id += i;
}

template< class T >
void khairullin::CIterator< T >::operator-=(size_t i)
{
  if (id + 1 - i < 0) {
    throw std::out_of_range("Const Iterator is out of bound");
  }
  id -= i;
}

template< class T >
khairullin::CIterator< T > khairullin::CIterator< T >::operator++()
{
  if (id + 1 >= vector.getSize()) {
    throw std::out_of_range("Iterator is out of bound");
  }
  id++;
  return *this;
}

template< class T >
khairullin::CIterator< T > khairullin::CIterator< T >::operator++(int)
{
  if (id + 1 >= vector.getSize()) {
    throw std::out_of_range("Iterator is out of bound");
  }
  auto copy = *this;
  id++;
  return copy;
}

template< class T >
khairullin::CIterator< T > khairullin::CIterator< T >::operator--(int)
{
  if (id - 1 < 0) {
    throw std::out_of_range("Iterator is out of bound");
  }
  auto copy = *this;
  id--;
  return copy;
}

template< class T >
T & khairullin::CIterator< T >::read()
{
  return vector[id];
}

template< class T >
T & khairullin::CIterator< T >::operator*() const noexcept
{
  return vector[id];
}

template< class T >
bool khairullin::CIterator< T >::operator==(const CIterator< T > & rhs) const noexcept
{
  return (*this).vector == rhs.vector && (*this).id == rhs.id;
}

template< class T >
bool khairullin::CIterator< T >::operator!=(const CIterator< T > & rhs) const noexcept
{
  return !(*this == rhs);
}
#endif

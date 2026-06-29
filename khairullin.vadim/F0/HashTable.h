#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "../common/Vector.h"
#include "Slot.h"
#include <iostream>
#include <initializer_list>

const size_t DEFAULT_TABLE_SIZE = 11;
namespace khairullin
{
  template< class Key, class T, class Hash, class Equal >
  struct TIterator;

  template< class Key, class T, class Hash, class Equal >
  struct ConstTIterator;

  template< class Key, class T, class Hash, class Equal >
  struct HashTable {
    Vector< Slot< Key, T > > table;
    Equal equal;
    Hash hasher;

    HashTable();
    HashTable(size_t k);
    HashTable(std::initializer_list< std::pair< T, Key > > list);
    ~HashTable() = default;
    HashTable(Vector< Slot< Key, T > > & t, size_t k, size_t count);
    bool operator==(const HashTable & other) const;
    bool operator!=(const HashTable & other) const;

    size_t findHomeSlot(const Key & key) const;
    ConstTIterator< Key, T, Hash, Equal > find(const Key & key) const;
    void insert(const T & val, const Key & key);
    void rehash(size_t newSize);
    bool remove(const Key & key);
    void swap(HashTable & other);
    size_t getSize() const noexcept;
    size_t getCountOfElement() const noexcept;

    private:
      size_t size = DEFAULT_TABLE_SIZE;
      size_t count = 0;
  };

  template< class Key, class T, class Hash, class Equal >
  struct TIterator {
    TIterator() = delete;
    TIterator(size_t index, HashTable< Key, T, Hash, Equal > & table);
    ~TIterator() = default;
    TIterator(const TIterator & other);
    TIterator & operator=(const TIterator & other);
    bool operator==(const TIterator & other) const;
    bool operator!=(const TIterator & other) const;


    T & value();
    bool hasNext();
    TIterator & operator++();
    TIterator & operator++(int);
    private:
      size_t index;
      HashTable< Key, T, Hash, Equal > & table;
  };

  template< class Key, class T, class Hash, class Equal >
  struct ConstTIterator {
    ConstTIterator() = delete;
    ConstTIterator(size_t index, const HashTable< Key, T, Hash, Equal > & table);
    ~ConstTIterator() = default;
    ConstTIterator(const ConstTIterator & other);
    ConstTIterator & operator=(const ConstTIterator & other);
    bool operator==(const ConstTIterator & other) const;
    bool operator!=(const ConstTIterator & other) const;
    const T & value();
    bool hasNext();
    ConstTIterator operator++();
    ConstTIterator operator++(int);
    private:
      size_t index;
      const HashTable< Key, T, Hash, Equal > & table;
  };
}

template < class Key, class T, class Hash, class Equal >
khairullin::HashTable< Key, T, Hash, Equal >::HashTable():
  table(Vector< Slot< Key, T > >(DEFAULT_TABLE_SIZE, Slot< Key, T >{})),
  equal(Equal{}),
  hasher(Hash{})
{}

template < class Key, class T, class Hash, class Equal >
khairullin::HashTable< Key, T, Hash, Equal >::HashTable(size_t k):
  table(Vector< Slot< Key, T > >(k, Slot< Key, T >{})),
  equal(Equal{}),
  hasher(Hash{}),
  size(k)
{}

template< class Key, class T, class Hash, class Equal >
khairullin::HashTable<Key, T, Hash, Equal>::HashTable(std::initializer_list<std::pair< T, Key>> list):
HashTable(list.size() * 2)
{
  for (const auto & i : list) {
    insert(i.first, i.second);
  }
}

template < class Key, class T, class Hash, class Equal >
khairullin::HashTable< Key, T, Hash, Equal >::HashTable(Vector< Slot < Key, T > > & t, size_t k, size_t count):
  table(t),
  equal(Equal{}),
  hasher(Hash{}),
  size(k),
  count(count)
{}

template< class Key, class T, class Hash, class Equal >
bool khairullin::HashTable<Key, T, Hash, Equal>::operator==(const HashTable & other) const
{
  for (size_t i = 0; i < size; i++) {
    if (table[i] != other.table[i]) {
      return false;
    }
  }
  return true;
}

template< class Key, class T, class Hash, class Equal >
bool khairullin::HashTable<Key, T, Hash, Equal>::operator!=(const HashTable & other) const
{
  return !(*this == other);
}

template< class Key, class T, class Hash, class Equal >
size_t khairullin::HashTable<Key, T, Hash, Equal>::findHomeSlot(const Key & key) const
{
  return hasher(key) % size;
}

template< class Key, class T, class Hash, class Equal >
khairullin::ConstTIterator< Key, T, Hash, Equal >
  khairullin::HashTable< Key, T, Hash, Equal >::find(const Key & key) const
{
  size_t index = hasher(key) % size;
  for (size_t i = index; i < size; i++) {
    if (equal(table[i].key, key)) {
      return ConstTIterator< Key, T, Hash, Equal >{i, *this};
    }
  }
  throw std::out_of_range("This value does not exist");
}

template< class Key, class T, class Hash, class Equal >
void khairullin::HashTable<Key, T, Hash, Equal>::insert(const T & val, const Key & key)
{
  size_t index = hasher(key) % size;
  Slot< Key, T > slot(val, key, index);
  while (!slot.Empty) {
    Slot< Key, T > & other = table[index];
    if (other.Empty) {
      slot.swap(other);
      count++;
    }
    if (slot.PSL > other.PSL) {
      slot.swap(other);
    }
    slot.PSL++;
    index = index == size ? 0 : index + 1;
  }
  if (count * 2 > size) {
    rehash(size * 2);
  }
}

template < class Key, class T, class Hash, class Equal >
void khairullin::HashTable<Key, T, Hash, Equal>::rehash(size_t newSize)
{
  HashTable< Key, T, Hash, Equal > newTable(newSize);
  for (size_t i = 0; i < size; i++) {
    if (!table[i].Empty) {
      newTable.insert(table[i].value, table[i].key);
    }
  }
  swap(newTable);
}

template< class Key, class T, class Hash, class Equal >
bool khairullin::HashTable<Key, T, Hash, Equal>::remove(const Key & key)
{
  Slot< Key, T > removeSlot;
  size_t index = hasher(key) % size;
  for (size_t i = index; i < size; i++) {
    if (equal(table[i].key, key)) {
      index = i;
      removeSlot.swap(table[i]);
      break;
    }
  }
  count = 0;
  for (size_t i = 0; i < size; i++) {
    if (!table[i].Empty) {
      Slot< Key, T > slot;
      table[i].swap(slot);
      insert(slot.value, slot.key);
    }
  }
  return true;
}

template< class Key, class T, class Hash, class Equal >
void khairullin::HashTable<Key, T, Hash, Equal>::swap(HashTable & other)
{
  std::swap(table, other.table);
  std::swap(equal, other.equal);
  std::swap(hasher, other.hasher);
  std::swap(size, other.size);
  std::swap(count, other.count);
}

template< class Key, class T, class Hash, class Equal >
size_t khairullin::HashTable<Key, T, Hash, Equal>::getSize() const noexcept
{
  return size;
}

template< class Key, class T, class Hash, class Equal >
size_t khairullin::HashTable<Key, T, Hash, Equal>::getCountOfElement() const noexcept
{
  return count;
}

template< class Key, class T, class Hash, class Equal >
khairullin::TIterator<Key, T, Hash, Equal>::TIterator(size_t index,
    HashTable<Key, T, Hash, Equal> & table):
  index(index),
  table(table)
{}

template< class Key, class T, class Hash, class Equal >
khairullin::TIterator<Key, T, Hash, Equal>::TIterator(const TIterator & other):
  index(other.index),
  table(other.table)
{}

template< class Key, class T, class Hash, class Equal >
khairullin::TIterator<Key, T, Hash, Equal> & khairullin::TIterator<Key, T, Hash, Equal>::operator=(
    const TIterator & other)
{
  if (*this == other) {
    return *this;
  }
  (*this).index = other.index;
  (*this).table = other.table;
}

template< class Key, class T, class Hash, class Equal >
bool khairullin::TIterator<Key, T, Hash, Equal>::operator==(const TIterator & other) const
{
  return index == other.index && table == other.table;
}

template< class Key, class T, class Hash, class Equal >
bool khairullin::TIterator<Key, T, Hash, Equal>::operator!=(const TIterator & other) const
{
  return !(*this == other);
}

template< class Key, class T, class Hash, class Equal >
T & khairullin::TIterator< Key, T, Hash, Equal >::value()
{
  return table.table[index].value;
}

template< class Key, class T, class Hash, class Equal >
bool khairullin::TIterator< Key, T, Hash, Equal >::hasNext()
{
  for (size_t i = index + 1; i < table.getSize(); i++) {
    if (!table.table[i].Empty) {
      return true;
    }
  }
  return false;
}

template< class Key, class T, class Hash, class Equal >
khairullin::TIterator< Key, T, Hash, Equal > & khairullin::TIterator< Key, T, Hash, Equal >::operator++()
{
  for (size_t i = index + 1; i < table.getSize(); i++) {
    if (!table.table[i].Empty) {
      return TIterator(i, table);
    }
  }
}

template< class Key, class T, class Hash, class Equal >
khairullin::TIterator<Key, T, Hash, Equal> & khairullin::TIterator<Key, T, Hash, Equal>::operator
++(int)
{
  auto temp = *this;
  ++*this;
  return temp;
}

template< class Key, class T, class Hash, class Equal >
khairullin::ConstTIterator<Key, T, Hash, Equal>::ConstTIterator(size_t index,
    const HashTable<Key, T, Hash, Equal> & table):
  index(index),
  table(table)
{}

template< class Key, class T, class Hash, class Equal >
khairullin::ConstTIterator<Key, T, Hash, Equal>::ConstTIterator(const ConstTIterator & other):
  index(other.index),
  table(other.table)
{}

template< class Key, class T, class Hash, class Equal >
khairullin::ConstTIterator< Key, T, Hash, Equal > &
    khairullin::ConstTIterator< Key, T, Hash, Equal >::operator=(const ConstTIterator & other)
{
  if (*this == other) {
    return *this;
  }
  (*this).index = other.index;
  table = other.table;
  return *this;
}

template< class Key, class T, class Hash, class Equal >
bool khairullin::ConstTIterator<Key, T, Hash, Equal>::operator==(const ConstTIterator & other) const
{
  return index == other.index && table == other.table;
}

template< class Key, class T, class Hash, class Equal >
bool khairullin::ConstTIterator<Key, T, Hash, Equal>::operator!=(const ConstTIterator & other) const
{
  return !(*this == other);
}

template< class Key, class T, class Hash, class Equal >
const T & khairullin::ConstTIterator<Key, T, Hash, Equal>::value()
{
  return table.table[index].value;
}

template< class Key, class T, class Hash, class Equal >
bool khairullin::ConstTIterator<Key, T, Hash, Equal>::hasNext()
{
  for (size_t i = index + 1; i < table.getSize(); i++) {
    if (!table.table[i].Empty) {
      return true;
    }
  }
  return false;
}

template< class Key, class T, class Hash, class Equal >
khairullin::ConstTIterator<Key, T, Hash, Equal>
    khairullin::ConstTIterator<Key, T, Hash, Equal>::operator++()
{
  for (size_t i = index + 1; i < table.table.getSize(); i++) {
    if (!table.table[i].Empty) {
      return ConstTIterator(i, table);
    }
  }
}

template< class Key, class T, class Hash, class Equal >
khairullin::ConstTIterator<Key, T, Hash, Equal>
    khairullin::ConstTIterator<Key, T, Hash, Equal >::operator++(int)
{
  auto temp = *this;
  ++*this;
  return temp;
}
#endif

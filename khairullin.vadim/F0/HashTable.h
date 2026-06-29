#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "Vector.h"
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
    HashTable(const HashTable & other);
    HashTable & operator=(const HashTable & other);
    HashTable(HashTable && other) noexcept;
    HashTable & operator=(HashTable && other) noexcept;
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

template< class Key, class T, class Hash, class Equal >
khairullin::HashTable<Key, T, Hash, Equal>::HashTable(const HashTable & other):
  table(other.table),
  equal(other.equal),
  hasher(other.hasher),
  size(other.size),
  count(other.count)
{}

template< class Key, class T, class Hash, class Equal >
khairullin::HashTable<Key, T, Hash, Equal> & khairullin::HashTable<Key, T, Hash, Equal>::operator=(
    const HashTable & other)
{
  if (*this == other) {
    return *this;
  }
  auto temp = other;
  swap(temp);
  return *this;
}

template< class Key, class T, class Hash, class Equal >
khairullin::HashTable<Key, T, Hash, Equal>::HashTable(HashTable && other) noexcept:
  table(std::move(other.table)),
  equal(other.equal),
  hasher(other.hasher),
  size(other.size),
  count(other.count)
{
  other.count = 0;
  other.size = 0;
}

template< class Key, class T, class Hash, class Equal >
khairullin::HashTable<Key, T, Hash, Equal> & khairullin::HashTable<Key, T, Hash, Equal>::operator=(
    HashTable && other) noexcept
{
  if (*this == other) {
    return *this;
  }
  auto temp= std::move(other);
  swap(temp);
  return *this;
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
  if (size != other.size || count != other.count) {
    return false;
  }
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
  for (size_t i = 0; i < size; i++) {
    size_t yai = (i + index) % size;
    if (table[yai].Empty) {
      break;
    }
    if (equal(table[yai].key, key)) {
      return ConstTIterator< Key, T, Hash, Equal >{yai, *this};
    }
  }
  throw std::out_of_range("This value does not exist");
}

template< class Key, class T, class Hash, class Equal >
void khairullin::HashTable<Key, T, Hash, Equal>::insert(const T & val, const Key & key)
{
  auto copy = *this;
  size_t index = hasher(key) % copy.size;
  Slot< Key, T > slot(val, key, index);
  while (true) {
    Slot< Key, T > & other = copy.table[index];
    if (other.Empty) {
      slot.swap(other);
      copy.count++;
      break;
    }
    if (slot.PSL > other.PSL) {
      slot.swap(other);
    }
    slot.PSL++;
    index = index == copy.size ? 0 : index + 1;
  }
  if (copy.count * 2 > copy.size) {
    copy.rehash(copy.size * 2);
  }
  swap(copy);
}

template < class Key, class T, class Hash, class Equal >
void khairullin::HashTable<Key, T, Hash, Equal>::rehash(size_t newSize)
{
  HashTable newTable(newSize);
  for (size_t i = 0; i < size; i++) {
    if (!table[i].Empty) {
      size_t index = hasher(table[i].key) % newTable.size;
      Slot< Key, T > slot(table[i].value, table[i].key, index);
      while (true) {
        Slot< Key, T > & other = newTable.table[index];
        if (other.Empty) {
          slot.swap(other);
          newTable.count++;
          break;
        }
        if (slot.PSL > other.PSL) {
          slot.swap(other);
        }
        slot.PSL++;
        index = index == newTable.size ? 0 : index + 1;
      }
    }
  }
  swap(newTable);
}

template< class Key, class T, class Hash, class Equal >
bool khairullin::HashTable<Key, T, Hash, Equal>::remove(const Key & key)
{
  auto copy = *this;
  Slot< Key, T > removeSlot;
  size_t index = hasher(key) % copy.size;
  for (size_t i = index; i < copy.size; i++) {
    if (equal(copy.table[i].key, key)) {
      index = i;
      removeSlot.swap(copy.table[i]);
      break;
    }
  }
  copy.count = 0;
  for (size_t i = 0; i < copy.size; i++) {
    if (!copy.table[i].Empty) {
      Slot< Key, T > slot;
      copy.table[i].swap(slot);
      copy.insert(slot.value, slot.key);
    }
  }
  (*this).swap(copy);
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

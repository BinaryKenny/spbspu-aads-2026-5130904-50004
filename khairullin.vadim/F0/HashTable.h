#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "Vector.h"
#include "Slot.h"
#include <iostream>
namespace khairullin
{
  template< class Key, class T, class Hash, class Equal >
  struct HashTable {
    Vector< Slot< Key, T > > table;
    Equal equal;
    Hash hasher;

    HashTable();
    HashTable(size_t k);
    ~HashTable() = default;
    HashTable(Vector< Slot< Key, T > > & t, size_t k, size_t count);

    size_t findHomeSlot(const Key & key) const;
    T & find(const Key & key) const noexcept;
    void insert(const T & val, const Key & key);
    void rehash(size_t newSize);
    bool remove(const Key & key);
    void swap(HashTable & other);
    size_t getSize() const noexcept;
    size_t getCount() const noexcept;

    private:
      size_t size = 11;
      size_t count = 0;
  };
}

template < class Key, class T, class Hash, class Equal >
khairullin::HashTable< Key, T, Hash, Equal >::HashTable():
table(Vector< Slot< Key, T > >(11, Slot< Key, T >{})),
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

template < class Key, class T, class Hash, class Equal >
khairullin::HashTable< Key, T, Hash, Equal >::HashTable(Vector< Slot < Key, T > > & t, size_t k, size_t count):
table(t),
equal(Equal{}),
hasher(Hash{}),
size(k),
count(count)
{}

template< class Key, class T, class Hash, class Equal >
size_t khairullin::HashTable<Key, T, Hash, Equal>::findHomeSlot(const Key & key) const
{
  return hasher(key) % size;
}

template< class Key, class T, class Hash, class Equal >
T & khairullin::HashTable<Key, T, Hash, Equal>::find(const Key & key) const noexcept
{
  size_t index = hasher(key) % size;
  for (size_t i = index; i < size; i++) {
    if (equal(table[i].key, key)) {
      return table[i].value;
    }
  }
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
size_t khairullin::HashTable<Key, T, Hash, Equal>::getCount() const noexcept
{
  return count;
}
#endif

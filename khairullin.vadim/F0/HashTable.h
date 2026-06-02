#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <cstddef>
#include "Vector.h"
#include "Slot.h"
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
    Key find(const T & val) const;
    void write(const T & val, const Key & key);
    void rehash(size_t newSize);
    bool remove(const T & val);
    bool remove(const Key & key);
    void swap(HashTable & other);
    void show();
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
void khairullin::HashTable<Key, T, Hash, Equal>::write(const T & val, const Key & key)
{
  size_t index = hasher(key) % size;
  Slot< Key, T > slot(val, key);
  Slot< Key, T > & other = table[index];
  if (other.Empty) {
    other = slot;
    return;
  }
  while (slot.PLS <= other.PLS) {
    slot.PLS = slot.PLS++;
    other = table[++index];
  }
  other.swap(slot);
  index++;
  for (size_t i = index; i < size; i++) {
    if (table[i].Empty) {
      slot.swap(table[i]);
    }
    slot.PLS++;
  }
  count++;
  if (count * 2 == size) {
    rehash(size * 2);
  }
}

#endif

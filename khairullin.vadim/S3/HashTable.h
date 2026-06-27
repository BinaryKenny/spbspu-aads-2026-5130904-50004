#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "../common/NodeList.hpp"
#include "../common/Vector.h"
#include "Equal.h"
#include "Hash.h"

static const size_t DEFAULT_TABLE_SIZE = 20;

namespace khairullin {
  template< class T, class Key, class Hash, class Equal >
  struct HashTable {
    Vector< NodeList< std::pair< T, Key > > * > table;
    Equal equal;
    Hash hasher;

    void add(const Key & key, const T & value);
    size_t findIndex(const Key & key) const;
    Vector< T > drop(const Key & key);
    void cut(const Key & key, const T & value);
    bool has(const Key & key, const T & value);
    void rehash(size_t new_size);

    HashTable();
    HashTable(size_t size);
    ~HashTable();
    HashTable(const HashTable & other);
    HashTable & operator=(const HashTable & other);
    HashTable(HashTable && other) noexcept;
    HashTable & operator=(HashTable && other);
    void swap(HashTable & other);
    bool operator==(const HashTable & other) const noexcept;
    bool operator!=(const HashTable & other) const noexcept;

    private:
      size_t size = DEFAULT_TABLE_SIZE;
      size_t count = 0;
  };
}

template< class T, class Key, class Hash, class Equal >
khairullin::HashTable< T, Key, Hash, Equal >::HashTable():
  table(Vector< NodeList< std::pair< T, Key > > * >(20, nullptr)),
  equal(Equal()),
  hasher(Hash())
{}

template< class T, class Key, class Hash, class Equal >
khairullin::HashTable< T, Key, Hash, Equal >::HashTable(size_t size):
  table(Vector< NodeList< std::pair< T, Key > > * >(size, nullptr)),
  equal(Equal()),
  hasher(Hash()),
  size(size)
{}

template< class T, class Key, class Hash, class Equal >
khairullin::HashTable< T, Key, Hash, Equal >::HashTable(const HashTable & other):
  HashTable(other.size)
{
  for (size_t i = 0; i < size; i++) {
    NodeList< std::pair< T, Key > > * slot = other.table[i];
    NodeList< std::pair< T, Key > > * head = nullptr;
    if (slot) {
      try {
        head = new NodeList< std::pair< T, Key > >(slot->val, nullptr, nullptr);
        slot = slot->next;
      } catch (...) {
        throw std::bad_alloc();
      }
    }
    auto tail = head;
    while (slot) {
      try {
        tail = tail->insert(slot->val, tail);
        slot = slot->next;
      } catch (...) {
        throw std::bad_alloc();
      }
    }
    std::swap(head, table[i]);
  }
  count = other.count;
}

template< class T, class Key, class Hash, class Equal >
khairullin::HashTable< T, Key, Hash, Equal > & khairullin::HashTable< T, Key, Hash,
  Equal >::operator=(const HashTable & other)
{
  if (*this == other) {
    return *this;
  }
  auto temp(other);
  swap(temp);
  return *this;
}

template< class T, class Key, class Hash, class Equal >
khairullin::HashTable< T, Key, Hash, Equal >::HashTable(HashTable && other) noexcept:
  HashTable(other.size)
{
  table.swap(other.table);
  std::swap(count, other.count);
}

template< class T, class Key, class Hash, class Equal >
khairullin::HashTable< T, Key, Hash, Equal > & khairullin::HashTable< T, Key, Hash,
  Equal >::operator=(HashTable && other)
{
  if (*this == other) {
    return *this;
  }
  auto temp(std::move(other));
  swap(temp);
  return *this;
}

template< class T, class Key, class Hash, class Equal >
void khairullin::HashTable< T, Key, Hash, Equal >::swap(HashTable & other)
{
  std::swap(table, other.table);
  std::swap(size, other.size);
  std::swap(count, other.count);
}

template< class T, class Key, class Hash, class Equal >
bool khairullin::HashTable< T, Key, Hash, Equal >::operator==(
    const HashTable & other) const noexcept
{
  if (count != other.count || size != other.size) {
    return false;
  }
  for (size_t i = 0; i < size; i++) {
    NodeList< std::pair< T, Key > > * otherSlot = other.table[i];
    NodeList< std::pair< T, Key > > * Slot = table[i];
    while (Slot != nullptr && otherSlot != nullptr) {
      if (Slot->val != otherSlot->val) {
        return false;
      }
      Slot = Slot->next;
      otherSlot = otherSlot->next;
    }
    if (Slot || otherSlot) {
      return false;
    }
  }
  return true;
}

template< class T, class Key, class Hash, class Equal >
bool khairullin::HashTable< T, Key, Hash, Equal >::operator!=(
    const HashTable & other) const noexcept
{
  return !(*this == other);
}

template< class T, class Key, class Hash, class Equal >
khairullin::HashTable< T, Key, Hash, Equal >::~HashTable()
{
  for (size_t i = 0; i < size; i++) {
    if (table[i] != nullptr) {
      table[i]->clear(table[i]);
    }
  }
}

template< class T, class Key, class Hash, class Equal >
void khairullin::HashTable< T, Key, Hash, Equal >::add(const Key & key, const T & value)
{
  size_t index = hasher(key) % size;
  if (table[index] == nullptr) {
    table[index] = new NodeList< std::pair< T, Key > >(std::make_pair(value, key), nullptr, nullptr);
  } else {
    auto tail = table[index];
    while (tail->next != nullptr) {
      tail = tail->next;
    }
    tail->insert(std::make_pair(value, key), tail);
  }
  count++;
}

template< class T, class Key, class Hash, class Equal >
khairullin::Vector< T > khairullin::HashTable< T, Key, Hash, Equal >::drop(const Key & key)
{
  size_t index = hasher(key) % size;
  auto slot = table[index];
  Vector< T > result;
  while (slot) {
    if (slot->value.second == key) {
      try {
        result.pushBack(slot->val.first);
      } catch (...) {
        throw std::bad_alloc();
      }
    }
    slot = slot->next;
  }
  return result;
}

template< class T, class Key, class Hash, class Equal >
void khairullin::HashTable< T, Key, Hash, Equal >::cut(const Key & key, const T & value)
{
  size_t index = hasher(key) % size;
  if (!has(key, value)) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto slot = table[index];
  if (equal(slot->val, std::make_pair(value, key))) {
    table[index] = slot->cut(slot);
  } else {
    NodeList< std::pair< T, Key > > * prev = slot;
    slot = slot->next;
    while (slot && !equal(slot->val, std::make_pair(value, key))) {
      prev = slot;
      slot = slot->next;
    }
    prev->next = slot->cut(slot);
  }
  count--;
}

template< class T, class Key, class Hash, class Equal >
bool khairullin::HashTable< T, Key, Hash, Equal >::has(const Key & key, const T & value)
{
  size_t index = hasher(key) % size;
  auto slot = table[index];
  while (slot && !equal(slot->val, std::make_pair(value, key))) {
    slot = slot->next;
  }
  return slot;
}

template< class T, class Key, class Hash, class Equal >
void khairullin::HashTable< T, Key, Hash, Equal >::rehash(size_t new_size)
{
  Vector< NodeList< std::pair< T, Key > > * > new_table;
  try {
    new_table = Vector< NodeList< std::pair< T, Key > > * >(new_size, nullptr);
  } catch (...) {
    throw std::bad_alloc();
  }
  for (size_t i = 0; i < size; i++) {
    auto slot = table[i];
    while (slot != nullptr) {
      size_t index = hasher(slot->val.second) % new_size;
      if (new_table[index] == nullptr) {
        new_table[index] = new NodeList< std::pair< T, Key > >(std::make_pair(slot->val.first,
            slot->val.second), nullptr);
      } else {
        auto tail = new_table[index];
        while (tail->next != nullptr) {
          tail = tail->next;
        }
        tail->insert(std::make_pair(slot->val.first, slot->val.second), tail);
      }
      slot = slot->next;
    }
  }
  try {
    table.swap(new_table);
    size = new_size;
  } catch (...) {
    throw std::bad_alloc();
  }
}

template< class T, class Key, class Hash, class Equal >
size_t khairullin::HashTable< T, Key, Hash, Equal >::findIndex(const Key & key) const
{
  size_t index = hasher(key) % size;
  return index;
}
#endif

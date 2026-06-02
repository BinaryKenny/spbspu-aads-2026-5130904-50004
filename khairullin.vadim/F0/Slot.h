#ifndef SLOT_H
#define SLOT_H
#include <string>
namespace khairullin {
    template < class Key , class T >
    struct Slot {
        T value;
        Key key;
        bool Empty;
        size_t PLS;

        bool operator==(const Slot & x) const;
        void swap(Slot & other);

        Slot();
        ~Slot() = default;
        Slot(const T & value, const Key & key);
        Slot(const Slot & other);
        Slot & operator=(const Slot & slot);
    };
}

template< class T >
khairullin::Slot< T >::Slot():
value(),
key(),
Empty(true),
PLS(0)
{}

template< class T >
khairullin::Slot< T >::Slot(const T & value, const Key & key):
value(value),
key(key),
Empty(false),
PLS(0)
{}

template< class T >
khairullin::Slot< T >::Slot(const Slot & other):
value(other.value),
key(other.key),
Empty(other.Empty),
PLS(other.PLS)
{}

template<class T>
khairullin::Slot<T> & khairullin::Slot<T>::operator=(const Slot & slot)
{
    auto temp(slot);
    swap(temp);
    return *this;
}

template< class T >
bool khairullin::Slot< T >::operator==(const Slot & x) const {
    return (value == x.value && isEmpty == x.isEmpty && Tombstone == x.Tombstone);
}

template< class T >
void khairullin::Slot< T >::swap(Slot & other) {
    std::swap(value, other.value);
    std::swap(key, other.key);
    std::swap(isEmpty, other.isEmpty);
    std::swap(Tombstone, other.Tombstone);
}
#endif

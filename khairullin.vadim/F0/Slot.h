#ifndef SLOT_H
#define SLOT_H
#include <string>
namespace khairullin {
    template < class Key , class T >
    struct Slot {
        T value;
        Key key;
        bool Empty;
        size_t PSL;
        size_t home;

        bool operator==(const Slot & x) const;
        void swap(Slot & other);

        Slot();
        ~Slot() = default;
        Slot(const T & value, const Key & key, size_t home);
        Slot(const Slot & other);
        Slot & operator=(const Slot & slot);
    };
}

template< class Key, class T >
khairullin::Slot< Key, T >::Slot():
value(),
key(),
Empty(true),
PSL(0),
home(0)
{}

template< class Key, class T >
khairullin::Slot< Key, T >::Slot(const T & value, const Key & key, size_t home):
value(value),
key(key),
Empty(false),
PSL(0),
home(home)
{}

template< class Key, class T >
khairullin::Slot< Key, T >::Slot(const Slot & other):
value(other.value),
key(other.key),
Empty(other.Empty),
PSL(other.PSL),
home(other.home)
{}

template< class Key, class T >
khairullin::Slot< Key, T > & khairullin::Slot< Key, T>::operator=(const Slot & slot)
{
    auto temp(slot);
    swap(temp);
    return *this;
}

template< class Key, class T >
bool khairullin::Slot< Key, T>::operator==(const Slot & other) const {
    return (value == other.value && Empty == other.Empty && PSL == other.PSL
      && key == other.key && home == other.home);
}

template< class Key, class T >
void khairullin::Slot< Key, T >::swap(Slot & other) {
    std::swap(value, other.value);
    std::swap(key, other.key);
    std::swap(Empty, other.Empty);
    std::swap(PSL, other.PSL);
    std::swap(home, other.home);
}
#endif

#ifndef EQUAL_H
#define EQUAL_H

namespace khairullin {
  template< class Key >
  struct Equal {
    bool operator()(const Key & first, const Key & second) const;
  };
}

template< class Key >
bool khairullin::Equal< Key >::operator()(const Key & first, const Key & second) const
{
  return first == second;
}
#endif

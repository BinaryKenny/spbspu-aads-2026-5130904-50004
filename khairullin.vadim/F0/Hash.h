#ifndef HASH_H
#define HASH_H
namespace khairullin {
  template< class Key >
  struct Hash {
    size_t operator()(Key skey) const;
  };
}

template< class Key >
size_t khairullin::Hash< Key >::operator()(Key skey) const
{
  return std::hash<Key>()(skey);
}
#endif

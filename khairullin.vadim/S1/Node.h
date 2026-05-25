#ifndef NODE_H
#define NODE_H
#include <cstddef>

namespace khairullin {
  template< class T >
  struct Node {
    T val;
    Node *next = nullptr;

    Node(const T &val);
  };
}

template< class T >
khairullin::Node< T >::Node(const T &val):
  val(val),
  next(nullptr) {
}

#endif //NODE_H

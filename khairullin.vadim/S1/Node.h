#ifndef NODE_H
#define NODE_H
#include <cstddef>
#include <algorithm>
namespace khairullin {
  template< class T >
  struct Node {
    T val;
    Node *next = nullptr;
    Node(const T &val);
    Node(const Node & node);
    Node & operator=(const Node & node);
  };
}

template< class T >
khairullin::Node< T >::Node(const T &val):
  val(val),
  next(nullptr) {
}

template< class T >
khairullin::Node<T>::Node(const Node & node):
val(node.val),
next(nullptr)
{}

template< class T >
khairullin::Node< T > & khairullin::Node< T >::operator=(const Node< T > & node)
{
  auto temp(node);
  std::swap(val, temp.val);
  std::swap(next, temp.next);
  return *this;
}
#endif //NODE_H

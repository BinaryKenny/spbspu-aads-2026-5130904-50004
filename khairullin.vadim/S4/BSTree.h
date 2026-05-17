#ifndef BSTREE_H
#define BSTREE_H
#include <utility>

template< class Key, class T, class Compare >
struct BSTree {
  BSTree * root;
  BSTree * left, * right;
  std::pair< Key, T > data;

  void push(Key key, T value);
  T get(Key key);
  T drop(Key key);
};

#endif //BSTREE_H

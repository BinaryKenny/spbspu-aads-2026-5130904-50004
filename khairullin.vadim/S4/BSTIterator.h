#ifndef BSTITERATOR_H
#define BSTITERATOR_H
#include "BSTree.h"

template< class Key, class T >
struct BSTIterator {
  BSTree * tree;

  bool hasNext();
  BSTIterator next();
  T read();
  void write(Key key, T value);
};

#endif //BSTITERATOR_H

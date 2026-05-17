#ifndef BSTCONSTITERATOR_H
#define BSTCONSTITERATOR_H
#include "BSTree.h"

template< class Key, class T >
struct BSTConstIterator {
  BSTree * tree;

  bool hasNext();
  BSTConstIterator next();
  T read();
};

#endif //BSTCONSTITERATOR_H

#ifndef BSTREE_H
#define BSTREE_H
#include <utility>

#include "../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/new"

template< class Key, class T, class Compare >
struct BSTree {
  BSTree * root;
  BSTree * left, * right;
  std::pair< Key, T > data;

  void push(Key key, T value);
  T get(Key key);
  T drop(Key key);
  BSTree *  remove(Key key);
};

template< class Key, class T, class Compare >
void BSTree<Key, T, Compare>::push(Key key, T value)
{
  BSTree * root = *this;
  BSTree * parent = nullptr;
  Compare less;
  while (root) {
    Key yakey = root->data.first;
    parent = root;
    if (less(key, yakey)) {
      root = root->left;
    }
    else if (less(yakey, key)) {
      root = root->right;
    }
    else {
      throw std::logic_error("This key is already occupied");
    }
  }
  try {
    BSTree * child = new BSTree(key, value);
  }
  catch (std::bad_alloc & e) {
    throw std::bad_alloc();
  }
  if (less(key, parent->data.first)) {
    parent->left = child;
  }
  else if (less(parent->data.first, key)) {
    parent->right = child;
  }
  else {
    throw std::logic_error("This key is already occupied");
  }
}

template< class Key, class T, class Compare >
T BSTree<Key, T, Compare>::get(Key key)
{
  BSTree * root = *this;
  while (root) {
    if (less(key, root->data.first)) {
      root = root->left;
    }
    else if (less(root->data.first, key)) {
      root = root->right;
    }
    else {
      return root->data.second;
    }
  }
  if (!root) {
    throw std::logic_error("No such element");
  }
}

template< class Key, class T, class Compare >
T BSTree<Key, T, Compare>::drop(Key key)
{
  BSTree * root = *this;
  T result = root->data.second;
  while (root) {
    if (less(key, root->data.first)) {
      root = root->left;
    }
    else if (less(root->data.first, key)) {
      root = root->right;
    }
    else {
      try {
        result = root->data.second;
      }
      catch (std::bad_alloc & e) {
        throw std::bad_alloc();
      }
      root = remove(key);
    }
  }
  return result;
}

#endif //BSTREE_H
#ifndef BSTREE_H
#define BSTREE_H
#include <utility>
#include <stdexcept>

#include "Compare.h"

namespace khairullin {

  template< class Key, class T, class Compare >
  struct BSTIterator;
  template< class Key, class T, class Compare >
  struct BSTConstIterator;

  template< class Key, class T, class Compare >
  using iterator = BSTIterator< Key, T, Compare >;
  template< class Key, class T, class Compare >
  using const_iterator = BSTConstIterator< Key, T, Compare >;

  template< class Key, class T, class Compare >
  struct BSTree {
    BSTree * left, * right, * parent;
    std::pair< Key, T > data;
    Compare less;

    using iterator = BSTIterator< Key, T, Compare >;
    using const_iterator = BSTConstIterator< Key, T, Compare >;

    BSTree(Key key, T value, BSTree * parent);
    BSTree();
    ~BSTree() = default;

    void push(Key key, T value);
    T get(Key key);
    BSTree * drop(Key key);

    size_t height();
    size_t height(BSTree * root);

    BSTree * fallLeft();
    BSTree * fallRight();
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator bigLeftRotate(const_iterator it);
    const_iterator bigRightRotate(const_iterator it);
  };

  template< class Key, class T, class Compare >
  struct BSTConstIterator {
    BSTree< Key, T, Compare> * root;
    BSTConstIterator(BSTree< Key, T, Compare > * root);

    bool hasNext();
    BSTConstIterator next();
    T read();
  };

  template< class Key, class T, class Compare >
  struct BSTIterator {
    BSTree< Key, T, Compare> * root;
    BSTIterator(BSTree< Key, T, Compare > * root);

    bool hasNext();
    BSTIterator next();
    T read();
    void write(Key key, T value);
  };
}

template< class Key, class T, class Compare >
khairullin::BSTree< Key, T, Compare >::BSTree(Key key, T value, BSTree * parent):
data(std::make_pair(key, value)),
parent(parent),
left(nullptr),
right(nullptr)
{}

template< class Key, class T, class Compare >
khairullin::BSTree< Key, T, Compare >::BSTree():
parent(nullptr),
left(nullptr),
right(nullptr),
data(std::make_pair(Key(), T()))
{}

template< class Key, class T, class Compare >
void khairullin::BSTree<Key, T, Compare>::push(Key key, T value)
{
  BSTree * root = this;
  if (!root) {
    throw std::logic_error("<EMPTY>");
  }
  BSTree * par = nullptr;
  while (root) {
    Key yakey = root->data.first;
    par = root;
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
  BSTree * child = nullptr;
  try {
    child = new BSTree(key, value, par);
  }
  catch (std::bad_alloc & e) {
    throw std::bad_alloc();
  }
  if (less(key, par->data.first)) {
    par->left = child;
  }
  else if (less(par->data.first, key)) {
    par->right = child;
  }
  else {
    throw std::logic_error("This key is already occupied");
  }
}

template< class Key, class T, class Compare >
T khairullin::BSTree<Key, T, Compare>::get(Key key)
{
  BSTree * root = this;
  if (!root) {
    throw std::logic_error("<EMPTY>");
  }
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
khairullin::BSTree<Key, T, Compare> * khairullin::BSTree<Key, T, Compare>::drop(Key key)
{
  BSTree * root = this;
  BSTree * tree = this;
  while (root) {
    if (less(key, root->data.first)) {
      root = root->left;
    }
    else if (less(root->data.first, key)) {
      root = root->right;
    }
    else {
      if (root->left) {
        auto instead = root->left->fallRight();
        if (root->left == instead) {
          instead->parent = root->parent;
          if (!root->parent) {
            tree = instead;
          }
          else if (root->parent->left == root) {
            root->parent->left = instead;
          }
          else if (root->parent->right == root) {
            root->parent->right = instead;
          }
          root->parent = nullptr;
          instead->right = root->right;
          if (root->right) {
            root->right->parent = instead;
          }
          root->left = nullptr;
          root->right = nullptr;
          delete root;
          return tree;
        }
        auto lt = instead->left;
        auto pr = instead->parent;
        pr->right = lt;
        if (lt) {
          lt->parent = pr;
        }
        instead->parent = root->parent;
        if (!root->parent) {
          tree = instead;
        }
        else if (root->parent->left == root){
          root->parent->left = instead;
        }
        else if (root->parent->right == root) {
          root->parent->right = instead;
        }
        instead->right = root->right;
        instead->left = root->left;
        root->left = nullptr;
        root->right = nullptr;
        root->parent = nullptr;
        instead->left->parent = instead;
        if (instead->right) {
          instead->right->parent = instead;
        }
        delete root;
        return tree;
      }
      else if (root->right) {
        auto instead = root->right->fallLeft();
        if (root->right == instead) {
          instead->parent = root->parent;
          if (!root->parent) {
            tree = instead;
          }
          else if (root->parent->left == root) {
            root->parent->left = instead;
          }
          else if (root->parent->right == root) {
            root->parent->right = instead;
          }
          root->parent = nullptr;
          instead->left = root->left;
          if (root->left) {
            root->left->parent = instead;
          }
          root->left = nullptr;
          root->right = nullptr;
          delete root;
          return tree;
        }
        auto rt = instead->right;
        auto pr = instead->parent;
        pr->left = rt;
        if (rt) {
          rt->parent = pr;
        }
        instead->parent = root->parent;
        if (!root->parent) {
          tree = instead;
        }
        else if (root->parent->left == root){
          root->parent->left = instead;
        }
        else if (root->parent->right == root) {
          root->parent->right = instead;
        }
        instead->right = root->right;
        instead->left = root->left;
        root->left = nullptr;
        root->right = nullptr;
        root->parent = nullptr;
        instead->right->parent = instead;
        if (instead->left) {
          instead->left->parent = instead;
        }
        delete root;
        return tree;
      }
      else {
        if (root->parent) {
          if (root->parent->left == root) {
            root->parent->left = nullptr;
          }
          else {
            root->parent->right = nullptr;
          }
          delete root;
          return tree;
        }
        else {
          return root;
        }
      }
    }
  }
  if (root == nullptr) {
    throw std::logic_error("No such element");
  }
  return tree;
}

template< class Key, class T, class Compare >
size_t khairullin::BSTree<Key, T, Compare>::height(BSTree * root)
{
  if (root == nullptr) {
    return 0;
  }
  return 1 + std::max(height(root->left), height(root->right));
}

template< class Key, class T, class Compare >
size_t khairullin::BSTree<Key, T, Compare>::height()
{
  auto root = this;
  return height(root);
}

template< class Key, class T, class Compare >
khairullin::BSTree<Key, T, Compare> * khairullin::BSTree<Key, T, Compare>::fallLeft()
{
  BSTree * root = this;
  while (root->left) {
    root = root->left;
  }
  return root;
}

template< class Key, class T, class Compare >
khairullin::BSTree<Key, T, Compare> * khairullin::BSTree<Key, T, Compare>::fallRight()
{
  BSTree * root = this;
  while (root->right) {
    root = root->right;
  }
  return root;
}

template< class Key, class T, class Compare >
khairullin::const_iterator<Key, T, Compare> khairullin::BSTree<Key, T, Compare>::rotateLeft(const_iterator it)
{
  BSTree * result = it.root->left;
  if (!result) {
    return it.root;
  }
  BSTree * rt = result->right;
  result->parent = it.root->parent;
  it.root->parent = result;
  result->right = it.root;
  it.root->left = rt;
  return result;
}

template< class Key, class T, class Compare >
khairullin::const_iterator<Key, T, Compare> khairullin::BSTree< Key, T, Compare>::rotateRight(const_iterator it)
{
  BSTree * result = it.root->right;
  if (!result) {
    return it.root;
  }
  BSTree * lt = result->left;
  result->parent = it.root->parent;
  it.root->parent = result;
  result->left = it.root;
  it.root->right = lt;
  return const_iterator{result};
}

template< class Key, class T, class Compare >
khairullin::const_iterator<Key, T, Compare> khairullin::BSTree< Key, T, Compare>::bigLeftRotate(const_iterator it)
{
  const_iterator result = it;
  result = rotateRight(result);
  result = rotateLeft(result);
  return result;
}

template< class Key, class T, class Compare >
khairullin::const_iterator<Key, T, Compare> khairullin::BSTree< Key, T, Compare>::bigRightRotate(const_iterator it)
{
  const_iterator result = it;
  result = rotateLeft(result);
  result = rotateRight(result);
  return result;
}

template< class Key, class T, class Compare >
khairullin::BSTConstIterator<Key, T, Compare>::BSTConstIterator(BSTree<Key, T, Compare> * root):
root(root)
{}

template< class Key, class T, class Compare >
khairullin::const_iterator<Key, T, Compare> khairullin::BSTConstIterator<Key, T, Compare>::next()
{
  auto list = (*this).root;
  auto original = list;
  if (!list) {
    throw std::out_of_range("Iterator was gone out");
  }
  if (list->right) {
    list = list->right->fallLeft();
    return const_iterator<Key, T, Compare>{list};
  }
  while (list && !list->less(original->data.first, list->data.first)) {
    list = list->parent;
  }
  return const_iterator<Key, T, Compare>{list};
}

template< class Key, class T, class Compare >
bool khairullin::BSTConstIterator<Key, T, Compare>::hasNext()
{
  return (*this).next().root;
}

template< class Key, class T, class Compare >
khairullin::iterator<Key, T, Compare> khairullin::BSTIterator<Key, T, Compare>::next()
{
  auto list = (*this).root;
  auto original = list;
  if (!list) {
    throw std::out_of_range("Iterator was gone out");
  }
  if (list->right) {
    list = list->right->fallLeft();
    return iterator<Key, T, Compare>{list};
  }
  while (list && !list->less(original->data.first, list->data.first)) {
    list = list->parent;
  }
  return iterator<Key, T, Compare>{list};
}

template< class Key, class T, class Compare >
bool khairullin::BSTIterator<Key, T, Compare>::hasNext()
{
  return (*this).next().root;
}
#endif //BSTREE_H
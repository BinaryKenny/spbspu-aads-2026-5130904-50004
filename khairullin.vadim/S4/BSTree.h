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
    T drop(Key key);

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
T khairullin::BSTree<Key, T, Compare>::drop(Key key)
{
  BSTree * root = this;
  if (!root) {
    throw std::logic_error("<EMPTY>");
  }
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

      if (root->left) {
        auto exchange = root->left;
        exchange = exchange->fallRight();
        auto exchangeLeft = exchange->left;
        std::swap(exchange->data, root->data);
        if (root->left == exchange) {
          delete exchange;
          root->left = exchangeLeft;
          exchangeLeft->parent = root;
        }
        else {
          auto exchangeParent = exchange->parent;
          delete exchange;
          exchangeParent->right = exchangeLeft;
          exchangeLeft->parent = exchangeParent;
        }
      }
      else if (root->right) {
        auto exchange = root->right;
        exchange = exchange->fallLeft();
        auto exchangeRight = exchange->right;
        std::swap(exchange->data, root->data);
        if (root->left == exchange) {
          delete exchange;
          root->right = exchangeRight;
          exchangeRight->parent = root;
        }
        else {
          auto exchangeParent = exchange->parent;
          delete exchange;
          exchangeParent->left = exchangeRight;
          exchangeRight->parent = exchangeParent;
        }
      }
      else {
        auto rootParent = root->parent;
        if (rootParent) {
          if (rootParent->left == root) {
            rootParent->left = nullptr;
          }
          else if (rootParent->right == root) {
            rootParent->right = nullptr;
          }
        }
        delete root;
        return result;
      }
    }
  }
  if (root == nullptr) {
    throw std::logic_error("No such element");
  }
  return result;
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
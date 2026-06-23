#ifndef BSTREE_H
#define BSTREE_H
#include <utility>
#include <stdexcept>
#include <algorithm>

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

    BSTree * copy(const BSTree * other);
    void push(Key key, T value);
    T get(Key key);
    BSTree * drop(Key key);
    bool has(Key key);
    void clear(BSTree * root);

    size_t height() const;
    size_t height(BSTree * root) const;

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
    BSTConstIterator(const BSTConstIterator & iter);
    BSTConstIterator & operator=(const BSTConstIterator & iter);

    bool operator==(const BSTConstIterator & other) const;
    bool operator!=(const BSTConstIterator & other) const;

    BSTConstIterator(BSTIterator<Key, T, Compare> iter);
    BSTConstIterator & operator=(BSTIterator<Key, T, Compare> * iter);

    bool hasNext() const;
    BSTConstIterator next();
    BSTConstIterator begin();
    BSTConstIterator end();
    std::pair< Key, T > read();
    bool has(Key key) const;
    bool exists() const;
    BSTree< Key, T, Compare > * operator*() const;
  };

  template< class Key, class T, class Compare >
  struct BSTIterator {
    BSTree< Key, T, Compare> * root;
    BSTIterator(BSTree< Key, T, Compare > * root);
    BSTIterator(const BSTIterator & iter);
    BSTIterator & operator=(const BSTIterator & iter);

    BSTIterator(BSTConstIterator<Key, T, Compare> iter);
    BSTIterator & operator=(BSTConstIterator<Key, T, Compare> * iter);

    bool operator==(const BSTIterator & other);
    bool operator!=(const BSTIterator & other);

    bool hasNext();
    BSTIterator next();
    BSTIterator begin();
    BSTIterator end();
    std::pair< Key, T > read();
    BSTIterator write(Key key, T value);
    bool has(Key key);
    bool exists();
    BSTree< Key, T, Compare > * operator*();
  };
}

template< class Key, class T, class Compare >
khairullin::BSTree< Key, T, Compare >::BSTree(Key key, T value, BSTree * parent):
left(nullptr),
right(nullptr),
parent(parent),
data(std::make_pair(key, value)),
less(Compare())
{}

template< class Key, class T, class Compare >
khairullin::BSTree< Key, T, Compare >::BSTree():
left(nullptr),
right(nullptr),
parent(nullptr),
data(std::make_pair(Key(), T())),
less(Compare())
{}

template< class Key, class T, class Compare >
khairullin::BSTree<Key, T, Compare> * khairullin::BSTree<Key, T, Compare>::copy(
    const BSTree * other)
{
  if (other == nullptr) {
    return nullptr;
  }
  BSTree * newNode = new BSTree(other->data.first, other->data.second);
  newNode->parent = other->parent;
  newNode->left = copy(other->left);
  newNode->right = copy(other->right);
  return newNode;
}

template< class Key, class T, class Compare >
void khairullin::BSTree<Key, T, Compare>::push(Key key, T value)
{
  auto root = this;
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
  T result = root->data.second;
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
  return result;
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
bool khairullin::BSTree<Key, T, Compare>::has(Key key)
{
  BSTree * tree = this;
  while (tree) {
    if (less(tree->data.first, key)) {
      tree = tree->right;
    }
    else if (less(key, tree->data.first)) {
      tree = tree->left;
    }
    else {
      return true;
    }
  }
  return false;
}

template< class Key, class T, class Compare >
void khairullin::BSTree<Key, T, Compare>::clear(BSTree * root)
{
  if (!root) {
    return;
  }
  if (root->left) {
    clear(root->left);
  }
  if (root->right) {
    clear(root->right);
  }
  delete root;
}

template< class Key, class T, class Compare >
size_t khairullin::BSTree<Key, T, Compare>::height(BSTree * root) const
{
  if (root == nullptr) {
    return 0;
  }
  return 1 + std::max(height(root->left), height(root->right));
}

template< class Key, class T, class Compare >
size_t khairullin::BSTree<Key, T, Compare>::height() const
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
  return const_iterator{result};
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
  return const_iterator{result};
}

template< class Key, class T, class Compare >
khairullin::const_iterator<Key, T, Compare> khairullin::BSTree< Key, T, Compare>::bigRightRotate(const_iterator it)
{
  const_iterator result = it;
  result = rotateLeft(result);
  result = rotateRight(result);
  return const_iterator{result};
}

template< class Key, class T, class Compare >
khairullin::BSTConstIterator<Key, T, Compare>::BSTConstIterator(BSTree<Key, T, Compare> * root):
root(root)
{}

template< class Key, class T, class Compare >
khairullin::BSTConstIterator<Key, T, Compare>::BSTConstIterator(const BSTConstIterator & iter):
root(iter.root)
{}

template< class Key, class T, class Compare >
khairullin::BSTConstIterator<Key, T, Compare> & khairullin::BSTConstIterator<Key, T, Compare>::
operator=(const BSTConstIterator & iter)
{
  auto temp(iter);
  std::swap(root, temp.root);
  return *this;
}

template< class Key, class T, class Compare >
bool khairullin::BSTConstIterator<Key, T, Compare>::operator==(const BSTConstIterator & other) const
{
  return root == other.root;
}

template< class Key, class T, class Compare >
bool khairullin::BSTConstIterator<Key, T, Compare>::operator!=(const BSTConstIterator & other) const
{
  return !(*this == other);
}

template< class Key, class T, class Compare >
khairullin::BSTConstIterator<Key, T, Compare>::BSTConstIterator(BSTIterator<Key, T, Compare> iter):
root(iter.root)
{}

template< class Key, class T, class Compare >
khairullin::BSTConstIterator<Key, T, Compare> & khairullin::BSTConstIterator<Key, T, Compare>::
operator=(BSTIterator<Key, T, Compare> * iter)
{
  auto temp(iter);
  std::swap(root, temp.root);
  return *this;
}

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
khairullin::BSTConstIterator<Key, T, Compare> khairullin::BSTConstIterator<Key, T, Compare>::begin()
{
  BSTree< Key, T, Compare > * list = (*this).root;
  if (!list) {
    return const_iterator<Key, T, Compare>{list};
  }
  while (list->parent) {
    list = list->parent;
  }
  list = list->fallLeft();
  return const_iterator<Key, T, Compare>{list};
}

template< class Key, class T, class Compare >
khairullin::BSTIterator<Key, T, Compare> khairullin::BSTIterator<Key, T, Compare>::begin()
{
  BSTree< Key, T, Compare > * list = (*this).root;
  if (!list) {
    return const_iterator<Key, T, Compare>{list};
  }
  while (list->parent) {
    list = list->parent;
  }
  list = list->fallLeft();
  return iterator<Key, T, Compare>{list};
}

template< class Key, class T, class Compare >
khairullin::BSTConstIterator<Key, T, Compare> khairullin::BSTConstIterator<Key, T, Compare>::end()
{
  return const_iterator<Key, T, Compare>{nullptr};
}

template< class Key, class T, class Compare >
khairullin::BSTIterator<Key, T, Compare> khairullin::BSTIterator<Key, T, Compare>::end()
{
  return iterator<Key, T, Compare>{nullptr};
}

template< class Key, class T, class Compare >
std::pair< Key, T > khairullin::BSTConstIterator<Key, T, Compare>::read()
{
  return root->data;
}

template< class Key, class T, class Compare >
bool khairullin::BSTConstIterator<Key, T, Compare>::has(Key key) const
{
  if (!root) {
    return false;
  }
  return root->has(key);
}

template< class Key, class T, class Compare >
bool khairullin::BSTConstIterator<Key, T, Compare>::exists() const
{
  return root != nullptr;
}

template< class Key, class T, class Compare >
khairullin::BSTree<Key, T, Compare> * khairullin::BSTConstIterator<Key, T, Compare>::operator*() const
{
  return root;
}

template< class Key, class T, class Compare >
bool khairullin::BSTIterator<Key, T, Compare>::exists()
{
  return root != nullptr;
}

template< class Key, class T, class Compare >
khairullin::BSTree<Key, T, Compare> * khairullin::BSTIterator<Key, T, Compare>::operator*()
{
  return root;
}

template< class Key, class T, class Compare >
bool khairullin::BSTIterator<Key, T, Compare>::has(Key key)
{
  if (!root) {
    return false;
  }
  return root->has(key);
}

template< class Key, class T, class Compare >
bool khairullin::BSTConstIterator<Key, T, Compare>::hasNext() const
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
std::pair< Key, T > khairullin::BSTIterator<Key, T, Compare>::read()
{
  return root->data;
}

template< class Key, class T, class Compare >
khairullin::BSTIterator<Key, T, Compare> khairullin::BSTIterator<Key, T, Compare>::write(Key key, T value)
{
  if (root == nullptr) {
    try {
      root = new BSTree< Key, T, Compare >{key, value, nullptr};
    }
    catch (...) {
      throw std::bad_alloc();
    }
  }
  else {
    root->push(key, value);
  }
  return iterator<Key, T, Compare>{root};
}

template< class Key, class T, class Compare >
khairullin::BSTIterator<Key, T, Compare>::BSTIterator(BSTree<Key, T, Compare> * root):
root(root)
{}

template< class Key, class T, class Compare >
khairullin::BSTIterator<Key, T, Compare>::BSTIterator(const BSTIterator & iter):
root(iter.root)
{}

template< class Key, class T, class Compare >
khairullin::BSTIterator< Key, T, Compare> &
  khairullin::BSTIterator<Key, T, Compare>::operator=(const BSTIterator<Key, T, Compare> & iter)
{
  auto temp(iter);
  std::swap(root, temp.root);
  return *this;
}

template< class Key, class T, class Compare >
khairullin::BSTIterator<Key, T, Compare>::BSTIterator(BSTConstIterator<Key, T, Compare> iter):
root(iter.root)
{}


template< class Key, class T, class Compare >
khairullin::BSTIterator<Key, T, Compare> & khairullin::BSTIterator<Key, T, Compare>::operator=(
    BSTConstIterator<Key, T, Compare> * iter)
{
  auto temp(iter);
  std::swap(root, temp.root);
  return *this;
}

template< class Key, class T, class Compare >
bool khairullin::BSTIterator<Key, T, Compare>::operator==(const BSTIterator & other)
{
  return root == other.root;
}

template< class Key, class T, class Compare >
bool khairullin::BSTIterator<Key, T, Compare>::operator!=(const BSTIterator & other)
{
  return !(*this == other);
}

template< class Key, class T, class Compare >
bool khairullin::BSTIterator<Key, T, Compare>::hasNext()
{
  return (*this).next().root;
}
#endif

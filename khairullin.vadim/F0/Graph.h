#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "Vector.h"
#include "Queue.hpp"
#include "Hash.h"
#include "Equal.h"
#include "HashTable.h"

namespace khairullin {
  template< class Key >
  struct Graph {
    public:
      Vector< Vector< Key > > edges;
      HashTable< Key, size_t , Hash< Key >, Equal< Key > > values;

      explicit Graph();
      explicit Graph(const std::string & name);
      Graph(const Graph & other);
      Graph & operator=(const Graph & other);
      ~Graph() = default;
      Graph(Graph && other) noexcept;
      Graph & operator=(Graph && other) noexcept;
      void swap(Graph & other);
      bool operator==(const Graph & other) const;
      bool operator!=(const Graph & other) const;

      size_t getVertices() const noexcept;
      std::string getName() const noexcept;

      std::pair< bool, size_t > hasVertex(const Key & key);
      size_t path(const Key & start, const Key & target);
      void addNode(const Key & key);
      void connect(const Key & key1, const Key & key2);
      void addWithConnection(const Key & key1, const Key & key2);
      bool deleteNode(const Key & key);
      void disconnect(const Key & key1, const Key & key2);
    private:
      size_t vertices = 0;
      std::string name;
  };
}

template< class Key >
khairullin::Graph< Key >::Graph():
  edges(Vector< Vector< Key > >()),
  values(HashTable< Key, size_t, Hash< Key >, Equal< Key > > ()),
  name("")
{}

template< class Key >
khairullin::Graph< Key >::Graph(const std::string & name):
  edges(Vector< Vector< Key > >()),
  values(HashTable< Key, size_t, Hash< Key >, Equal< Key > > ()),
  name(name)
{}

template< class Key >
khairullin::Graph<Key>::Graph(const Graph & other):
  edges(other.edges),
  values(other.values),
  vertices(other.vertices),
  name(other.name)
{}

template< class Key >
khairullin::Graph<Key> & khairullin::Graph<Key>::operator=(const Graph & other)
{
  if (*this == other) {
    return *this;
  }
  auto temp = other;
  swap(temp);
  return *this;
}

template< class Key >
khairullin::Graph<Key>::Graph(Graph && other) noexcept:
edges(std::move(other.edges)),
values(std::move(other.values)),
vertices(other.vertices),
name(other.name)
{}

template< class Key >
khairullin::Graph<Key> & khairullin::Graph<Key>::operator=(Graph && other) noexcept
{
  if (*this == other) {
    return *this;
  }
  auto temp = std::move(other);
  swap(temp);
  return *this;
}

template< class Key >
void khairullin::Graph<Key>::swap(Graph & other)
{
  edges.swap(other.edges);
  values.swap(other.values);
  std::swap(name, other.name);
  std::swap(vertices, other.vertices);
}

template< class Key >
bool khairullin::Graph<Key>::operator==(const Graph & other) const
{
  return edges == other.edges && values == other.values && name == other.name
      && vertices == other.vertices;
}

template< class Key >
bool khairullin::Graph<Key>::operator!=(const Graph & other) const
{
  return !(*this == other);
}

template< class Key >
size_t khairullin::Graph< Key >::getVertices() const noexcept
{
  return vertices;
}

template< class Key >
std::string khairullin::Graph<Key>::getName() const noexcept
{
  return name;
}

template< class Key >
std::pair<bool, size_t> khairullin::Graph<Key>::hasVertex(const Key & key)
{
  size_t index = 0;
  try {
    auto iterator = values.find(key);
    index = iterator.value();
  }
  catch (...) {
    return std::make_pair(false, 0);
  }
  return std::make_pair(true, index);
}

template< class Key >
void khairullin::Graph< Key >::addNode(const Key & key)
{
  size_t temp = vertices;
  edges.pushBack(Vector< Key >());
  try {
    vertices++;
    values.insert(temp, key);
  }
  catch (...) {
    edges.popBack();
    vertices--;
    throw;
  }
}

template< class Key >
void khairullin::Graph< Key >::connect(const Key & key1, const Key & key2)
{
  size_t vertex1 = 0;
  size_t vertex2 = 0;
  try {
    auto iter1 = values.find(key1);
    auto iter2= values.find(key2);
    vertex1 = iter1.value();
    vertex2 = iter2.value();
  }
  catch (...) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (edges[vertex1].hasValue(key2).first) {
    throw std::out_of_range("invalid");
  }
  edges[vertex1].pushBack(key2);
  try {
    edges[vertex2].pushBack(key1);
  }
  catch (...) {
    edges[vertex1].popBack();
  }
}

template< class Key >
void khairullin::Graph< Key >::addWithConnection(const Key & key1, const Key & key2)
{
  Graph copy = *this;
  copy.addNode(key1);
  copy.addNode(key2);
  copy.connect(key1, key2);
  swap(copy);
}

template< class Key >
bool khairullin::Graph< Key >::deleteNode(const Key & key) {
  size_t vertex = 0;
  try {
    auto iter = values.find(key);
    vertex = iter.value();
  }
  catch (...) {
    throw std::logic_error("<LOGIC> This node doesn't exist");
  }
  Vector< Vector< Key > > copyEdges;
  HashTable< Key, size_t, Hash< Key >, Equal< Key > > copyValues;
  try {
    copyEdges = edges;
    copyValues = values;
    if (copyValues.remove(key)) {
      copyEdges.erase(vertex);
      for (size_t i = 0; i < copyEdges.getSize(); i++) {
        auto info = copyEdges[i].hasValue(key);
        if (info.first) {
          copyEdges[i].erase(info.second);
        }
        for (size_t j = 0; j < copyEdges[i].getSize(); j++) {
          Key & k = copyEdges[i][j];
          size_t weight = (copyValues.find(k)).value();
          if (weight > vertex) {
            copyValues.remove(k);
            copyValues.insert(weight - 1, k);
          }
        }
      }
    }
  }
  catch (...) {
    return false;
  }
  edges = std::move(copyEdges);
  values = std::move(copyValues);
  return true;
}

template< class Key >
void khairullin::Graph<Key>::disconnect(const Key & key1, const Key & key2)
{
  size_t vertex1 = 0;
  size_t vertex2 = 0;
  try {
    auto iter1 = values.find(key1);
    auto iter2 = values.find(key2);
    vertex1 = iter1.value();
    vertex2 = iter2.value();
  }
  catch (...) {
    throw std::logic_error("This node(s) doesn't exist");
  }
  auto infoVert1 = edges[vertex2].hasValue(key1);
  auto infoVert2 = edges[vertex1].hasValue(key2);
  if (!infoVert1.first || !infoVert2.first) {
    throw std::logic_error("This vertexes were not connected");
  }
  Vector< Key > vec1;
  Vector< Key > vec2;
  try {
    vec1 = (edges[vertex1]);
    vec2 = (edges[vertex2]);
    vec1.erase(infoVert2.second);
    vec2.erase(infoVert1.second);
    edges[vertex1].swap(vec1);
    edges[vertex2].swap(vec2);
  }
  catch (...) {
    throw;
  }
}

template< class Key >
size_t khairullin::Graph< Key >::path(const Key & k1, const Key & k2) {
  size_t start = 0;
  size_t target = 0;
  try {
    auto iter1 = values.find(k1);
    auto iter2 = values.find(k2);
    start = iter1.value();
    target = iter2.value();
  }
  catch (...) {
    throw std::logic_error("This node(s) doesn't exist");
  }
  if (start >= vertices || target >= vertices) {
    return vertices;
  }
  if (start == target) {
    return 0;
  }
  Vector< bool > visited(vertices, false);
  visited[start] = true;
  Queue< size_t > queue;
  queue.push(start);
  Vector< size_t > distances(vertices, 0);
  while (!queue.empty()) {
    size_t vertex = queue.drop();
    if (vertex == target) {
      return distances[vertex];
    }
    auto child = edges[vertex];
    for (size_t i = 0; i < child.getSize(); i++) {
      size_t temp = 0;
      try {
        temp = (values.find(child[i])).value();
      }
      catch (...) {
        throw std::logic_error("This node(s) doesn't exist");
      }
      if (!visited[temp]) {
        queue.push(temp);
        visited[temp] = true;
        distances[temp] = distances[vertex] + 1;
      }
    }
  }
  return vertices;
};
#endif

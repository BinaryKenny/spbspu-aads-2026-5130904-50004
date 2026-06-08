#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "Vector.h"
#include "Queue.h"
#include "Hash.h"
#include "Equal.h"
#include "HashTable.h"

namespace khairullin {
  template< class Key >
  struct Graph {
    Vector< Vector< Key > > edges;
    HashTable< Key, size_t , Hash< Key >, Equal< Key > > values;
    size_t vertices = 0;
    std::string name;

    Graph();
    Graph(std::string name);

    std::pair< bool, size_t > hasVertex(const Key & key);
    size_t path(const Key & start, const Key & target);
    void addNode(const Key & key);
    void connect(const Key & key1, const Key & key2);
    void addWithConnection(const Key & key1, const Key & key2);
    bool deleteNode(const Key & key);
    void disconnect(const Key & key1, const Key & key2);
  };
}

template< class Key >
khairullin::Graph< Key >::Graph():
  edges(Vector< Vector< Key > >()),
  values(HashTable< Key, size_t, Hash< Key >, Equal< Key > > ()),
  name("")
{}

template< class Key >
khairullin::Graph< Key >::Graph(std::string name):
  edges(Vector< Vector< Key > >()),
  values(HashTable< Key, size_t, Hash< Key >, Equal< Key > > ()),
  name(name)
{}

template< class Key >
std::pair<bool, size_t> khairullin::Graph<Key>::hasVertex(const Key & key)
{
  size_t index = 0;
  try {
    index = values.find(key);
  }
  catch (...) {
    return std::make_pair(false, 0);
  }
  return std::make_pair(true, index);
}

template< class Key >
void khairullin::Graph< Key >::addNode(const Key & key) {
  size_t temp = vertices;
  try {
    edges.pushBack(Vector< Key >());
    vertices++;
    values.insert(temp, key);
  }
  catch (...) {
    try {
      edges.erase(temp);
      vertices--;
    }
    catch (...) {
      return;
    }
  }
}

template< class Key >
void khairullin::Graph<Key>::connect(const Key & key1, const Key & key2)
{
  size_t vertex1 = 0;
  size_t vertex2 = 0;
  try {
    vertex1 = values.find(key1);
    vertex2 = values.find(key2);
  }
  catch (...) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  edges[vertex1].pushBack(key2);
  try {
    edges[vertex2].pushBack(key1);
  }
  catch (...) {
    edges[vertex2].popBack();
  }
}

template< class Key >
void khairullin::Graph< Key >::addWithConnection(const Key & key1, const Key & key2)
{
  try{
    addNode(key2);
    connect(key1, key2);
  }
  catch (...) {
    throw std::logic_error("Failed connection");
  }
}

template< class Key >
bool khairullin::Graph< Key >::deleteNode(const Key & key) {
  size_t vertex = values.find(key);
  Vector< Vector< Key > > copyEdges;
  try {
    copyEdges = edges;
    if (values.remove(key)) {
      copyEdges.erase(vertex);
      for (size_t i = 0; i < copyEdges.getSize(); i++) {
        auto info = copyEdges[i].hasValue(key);
        if (info.first) {
          copyEdges[i].erase(info.second);
        }
        for (size_t j = 0; j < copyEdges[i].getSize(); j++) {
          Key & k = copyEdges[i][j];
          size_t weight = values.find(k);
          if (weight > vertex) {
            values.remove(k);
            values.insert(weight - 1, k);
          }
        }
      }
    }
  }
  catch (...) {
    return false;
  }
  edges = std::move(copyEdges);
  return true;
}

template< class Key >
void khairullin::Graph<Key>::disconnect(const Key & key1, const Key & key2)
{
  size_t vertex1 = values.find(key1);
  size_t vertex2 = values.find(key2);
  auto infoVert1 = edges[vertex2].hasValue(key1);
  auto infoVert2 = edges[vertex1].hasValue(key2);
  if (!infoVert1.first || !infoVert2.first) {
    throw std::logic_error("This vertexes were not connected");
  }
  Vector< Key > vec1;
  Vector< Key > vec2;
  try {
    vec1 = edges[vertex1];
    vec2 = edges[vertex2];
    vec1.erase(infoVert2.second);
    vec2.erase(infoVert1.second);
    edges[vertex1] = std::move(vec1);
    edges[vertex2] = std::move(vec2);
  }
  catch (...) {
    throw std::bad_alloc();
  }
}

template< class Key >
size_t khairullin::Graph< Key >::path(const Key & k1, const Key & k2) {
  size_t start = values.find(k1);
  size_t target = values.find(k2);
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
  while (queue.notEmpty()) {
    size_t vertex = queue.drop();
    if (vertex == target) {
      return distances[vertex];
    }
    auto child = edges[vertex];
    for (size_t i = 0; i < child.getSize(); i++) {
      size_t temp = values.find(child[i]);
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

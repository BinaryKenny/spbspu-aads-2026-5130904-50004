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
    Vector< Vector< size_t > > graph;
    HashTable< Key, size_t , Hash< Key >, Equal< Key > > values;
    size_t vertices = 0;

    Graph();

    size_t path(const Key & start, const Key & target);
    void addNode(const Key & key);
    void connect(size_t vertex, size_t yav);
    void addWithConnection(const Key & key1, const Key & key2);
    bool deleteNode(const Key & key);
  };
}

template< class Key >
khairullin::Graph< Key >::Graph():
  graph(Vector< Vector< size_t > >()),
  values(HashTable< Key, size_t, Hash< Key >, Equal< Key > > ())
{}

template< class Key >
void khairullin::Graph< Key >::addNode(const Key & key) {
  size_t temp = vertices;
  try {
    graph.pushBack(Vector< size_t >{});
    vertices++;
    values.insert(vertices - 1, key);
  }
  catch (...) {
    try {
      graph.erase(temp);
    }
    catch (...) {
      throw;
    }
  }
}

template< class Key >
void khairullin::Graph< Key >::connect(size_t vertex, size_t yav) {
    if (vertex >= vertices || yav >= vertices) {
        throw std::out_of_range("This vertex doesn't exist");
    }
    graph[vertex].pushBack(yav);
    graph[yav].pushBack(vertex);
}

template< class Key >
void khairullin::Graph< Key >::addWithConnection(const Key & key1, const Key & key2)
{
  try{
    size_t vertex = values.find(key1);
    addNode(key2);
    connect(vertex, vertices - 1);
  }
  catch (...) {
    throw std::logic_error("Failed connection");
  }
}

template< class Key >
bool khairullin::Graph< Key >::deleteNode(const Key & key) {
  size_t vertex = values.find(key);
  if (values.remove(key)) {
    vertices--;
    graph.erase(vertex);
    return true;
  }
  return false;
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
    auto child = graph[vertex];
    for (size_t i = 0; i < child.getSize(); i++) {
      if (!visited[child[i]]) {
        queue.push(child[i]);
        visited[child[0]] = true;
        distances[child[0]] = distances[vertex] + 1;
      }
    }
  }
  return vertices;
};
#endif

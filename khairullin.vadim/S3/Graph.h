#ifndef GRAPH_H
#define GRAPH_H
#include "HashTable.h"
#include <iostream>

namespace khairullin {
  struct Graph {
    std::string name;
    Vector< std::string > vertexes;
    HashTable< size_t, std::string, Hash, Equal< std::pair< size_t, std::string > > > edges;

    Graph(const std::string & name);
    Graph();
    ~Graph() = default;
    Graph(const Graph & other);
    Graph & operator=(const Graph & other);
    Graph(Graph && other);
    Graph & operator=(Graph && other);
    bool operator==(const Graph & other) const noexcept;

    std::pair< bool, size_t > hasVertex(const std::string & vert) const;
    bool hasConnection(const std::string & vert1, const std::string & vert2) const;
    void swap(Graph & graph);

    void addEdge(const std::string & vert1, const std::string & vert2, size_t weight);
    void cutEdge(const std::string & vert1, const std::string & vert2, size_t weight);
    void addVertex(const std::string & vert);
  };

  std::ostream & operator<<(std::ostream & os, const Graph & g);
}
#endif

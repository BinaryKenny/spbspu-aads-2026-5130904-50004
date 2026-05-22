#ifndef GRAPHSYSTEM_H
#define GRAPHSYSTEM_H
#include <iostream>
#include <ostream>
#include "Functions.h"
#include "Graph.h"

namespace khairullin {
  struct GraphSystem {
    using func_t = void(GraphSystem::*)(std::string & line);
    Vector< Graph > vectorOfGraphs;
    HashTable< func_t, std::string, Hash, Equal< std::pair< func_t, std::string > > >
    functions;

    GraphSystem();

    void func(std::string & line);
    std::pair< bool, size_t > graphExists(std::string & name);

    void graphs(std::string & line);
    void vertexes(std::string & line);
    void outbound(std::string & line);
    void inbound(std::string & line);
    void bind(std::string & line);
    void cut(std::string & line);
    void create(std::string & line);
    void merge(std::string & line);
    void extract(std::string & line);
  };
}
#endif

#include <iostream>
#include "GraphSystem.h"
#include "fstream"

int main(int argc, char ** argv)
{
  if (argc != 2) {
    std::cerr << "No file or too many arguments\n";
    return 1;
  }
  const char * filename = argv[1];
  khairullin::GraphSystem graphSystem;
  std::ifstream input(filename);
  std::string line = "";
  while (std::getline(input, line)) {
    if (line.empty()) {
      continue;
    }
    std::string graphName = khairullin::getToken(line);
    std::string c = khairullin::getToken(line);
    size_t count = std::stoi(c);
    khairullin::Graph graph(graphName);
    for (size_t i = 0; i < count; i++) {
      std::getline(input, line);
      std::string vert1 = khairullin::getToken(line);
      std::string vert2 = khairullin::getToken(line);
      size_t weight = std::stoi(khairullin::getToken(line));
      if (!graph.hasVertex(vert1).first) {
        graph.addVertex(vert1);
      }
      if (!graph.hasVertex(vert2).first) {
        graph.addVertex(vert2);
      }
      graph.addEdge(vert1, vert2, weight);
    }
    graphSystem.vectorOfGraphs.pushBack(graph);
  }
  while (std::getline(std::cin, line)) {
    try {
      graphSystem.func(line);
    }
    catch (const std::exception & e) {
      std::cout << e.what() << "\n";
    }
    catch (...) {
      std::cerr << "Bad allocation\n";
      return 1;
    }
  }
}
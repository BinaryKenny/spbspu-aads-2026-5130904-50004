#include "Graph.h"

khairullin::Graph::Graph():
  name(""),
  vertexes(Vector< std::string >()),
  edges(HashTable< size_t, std::string, Hash, Equal< std::pair< size_t, std::string > > >())
{}

khairullin::Graph::Graph(const Graph & other):
  Graph(other.name)
{
  vertexes = other.vertexes;
  edges = other.edges;
}

khairullin::Graph & khairullin::Graph::operator=(const Graph & other)
{
  if (*this == other) {
    return *this;
  }
  Graph temp(other);
  swap(temp);
  return *this;
}

khairullin::Graph::Graph(Graph && other):
  Graph(other.name)
{
  vertexes.swap(other.vertexes);
  edges.swap(other.edges);
}

khairullin::Graph & khairullin::Graph::operator=(Graph && other)
{
  if (*this == other) {
    return *this;
  }
  auto temp(std::move(other));
  swap(temp);
  return *this;
}

khairullin::Graph::Graph(const std::string & name):
  name(name),
  vertexes(Vector< std::string >()),
  edges(HashTable< size_t, std::string, Hash, Equal< std::pair< size_t,
    std::string > > >())
{}

bool khairullin::Graph::operator==(const Graph & other) const noexcept
{
  if (name != other.name || vertexes != other.vertexes || edges != other.edges) {
    return false;
  }
  return true;
}

std::pair< bool, size_t > khairullin::Graph::hasVertex(const std::string & vert) const
{
  for (size_t i = 0; i < vertexes.getSize(); i++) {
    if (vertexes[i] == vert) {
      return std::make_pair(true, i);
    }
  }
  return std::make_pair(false, 0);
}

bool khairullin::Graph::hasConnection(const std::string & vert1, const std::string & vert2) const
{
  size_t index = edges.findIndex(vert1 + vert2);
  auto slot = edges.table[index];
  while (slot && slot->value.second != vert1 + vert2) {
    slot = slot->next;
  }
  if (!slot) {
    return false;
  }
  return true;
}

void khairullin::Graph::swap(Graph & graph)
{
  vertexes.swap(graph.vertexes);
  edges.swap(graph.edges);
  std::swap(name, graph.name);
}

void khairullin::Graph::addEdge(const std::string & vert1, const std::string & vert2, size_t weight)
{
  std::string key = vert1 + vert2;
  std::pair< bool, size_t > infoVert1 = hasVertex(vert1);
  std::pair< bool, size_t > infoVert2 = hasVertex(vert2);
  if (!infoVert1.first || !infoVert2.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  edges.add(key, weight);
}

void khairullin::Graph::cutEdge(const std::string & vert1, const std::string & vert2, size_t weight)
{
  std::string key = vert1 + vert2;
  std::pair< bool, size_t > infoVert1 = hasVertex(vert1);
  std::pair< bool, size_t > infoVert2 = hasVertex(vert2);
  if (!infoVert1.first || !infoVert2.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  edges.cut(key, weight);
}

void khairullin::Graph::addVertex(const std::string & vert)
{
  auto index = hasVertex(vert);
  if (index.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  try {
    vertexes.pushBack(vert);
  }
  catch (...)
  {
    throw;
  }
}

std::ostream& khairullin::operator<<(std::ostream& os, const Graph& g) {
  os << g.name;
  return os;
}

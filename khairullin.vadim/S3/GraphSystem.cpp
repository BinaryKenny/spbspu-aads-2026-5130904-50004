#include "GraphSystem.h"

khairullin::GraphSystem::GraphSystem():
  vectorOfGraphs(Vector< Graph >()),
  functions(
      HashTable< func_t, std::string, Hash, Equal< std::pair< func_t,
        std::string > > >())
{
  functions.add("graphs", &GraphSystem::graphs);
  functions.add("vertexes", &GraphSystem::vertexes);
  functions.add("outbound", &GraphSystem::outbound);
  functions.add("inbound", &GraphSystem::inbound);
  functions.add("bind", &GraphSystem::bind);
  functions.add("cut", &GraphSystem::cut);
  functions.add("create", &GraphSystem::create);
  functions.add("merge", &GraphSystem::merge);
  functions.add("extract", &GraphSystem::extract);
}

void khairullin::GraphSystem::func(std::string & line)
{
  std::string function = getToken(line);
  auto methods = functions.drop(function);
  func_t method = nullptr;
  if (methods.getSize() == 1) {
    method = methods[0];
  }
  else {
    throw std::logic_error("<INVALID COMMAND>");
  }
  (this->*method)(line);
}

void khairullin::GraphSystem::graphs(std::string & )
{
  for (size_t i = 0; i < vectorOfGraphs.getSize(); i++) {
    std::cout << vectorOfGraphs[i].name << "\n";
  }
}

std::pair< bool, size_t > khairullin::GraphSystem::graphExists(std::string & name)
{
  for (size_t i = 0; i < vectorOfGraphs.getSize(); i++) {
    if (vectorOfGraphs[i].name == name) {
      return std::make_pair(true, i);
    }
  }
  return std::make_pair(false, 0);
}

void khairullin::GraphSystem::vertexes(std::string & line)
{
  std::pair< bool, size_t > result = graphExists(line);
  if (!result.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph graph = vectorOfGraphs[result.second];
  for (size_t i = 0; i < graph.vertexes.getSize(); i++) {
    std::cout << graph.vertexes[i] << "\n";
  }
}

void khairullin::GraphSystem::outbound(std::string & line)
{
  std::string nameGraph = getToken(line);
  std::string vertex = getToken(line);
  std::pair< bool, size_t > index = graphExists(nameGraph);
  if (!index.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph graph;
  try {
    graph = vectorOfGraphs[index.second];
  } catch (std::bad_alloc & e) {
    throw std::bad_alloc();
  }

  std::pair< bool, size_t > infoVertex = graph.hasVertex(vertex);
  if (!infoVertex.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  for (size_t i = 0; i < graph.vertexes.getSize(); i++) {
    if (!graph.hasConnection(vertex, graph.vertexes[i])) {
      continue;
    }
    Vector< size_t > weights;
    try {
      weights = graph.edges.drop(vertex + graph.vertexes[i]);
    } catch (...) {
      throw std::bad_alloc();
    }
    sortVector(weights);
    std::cout << graph.vertexes[i];
    for (size_t j = 0; j < weights.getSize(); j++) {
      std::cout << " " << weights[j];
    }
    std::cout << "\n";
  }
}

void khairullin::GraphSystem::inbound(std::string & line)
{
  std::string nameGraph = getToken(line);
  std::string vertex = getToken(line);
  auto index = graphExists(nameGraph);
  if (!index.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph graph;
  try {
    graph = vectorOfGraphs[index.second];
  } catch (std::bad_alloc & e) {
    throw std::bad_alloc();
  }

  for (size_t i = 0; i < graph.vertexes.getSize(); i++) {
    if (!graph.hasConnection(graph.vertexes[i], vertex)) {
      continue;
    }
    Vector< size_t > weights;
    try {
      weights = graph.edges.drop(graph.vertexes[i] + vertex);
    } catch (...) {
      throw std::bad_alloc();
    }
    sortVector(weights);
    std::cout << graph.vertexes[i];
    for (size_t j = 0; j < weights.getSize(); j++) {
      std::cout << " " << weights[j];
    }
    std::cout << "\n";
  }
}

void khairullin::GraphSystem::bind(std::string & line)
{
  std::string graph = getToken(line);
  std::string Vertex1 = getToken(line);
  std::string Vertex2 = getToken(line);
  size_t weight = 0;
  auto infoGraph = graphExists(graph);
  if (!infoGraph.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph & gr = vectorOfGraphs[infoGraph.second];
  if (!gr.hasVertex(Vertex1).first) {
    gr.addVertex(Vertex1);
  }
  if (!gr.hasVertex(Vertex2).first) {
    gr.addVertex(Vertex2);
  }
  if (!infoGraph.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  try {
    weight = std::stoi(getToken(line));
  } catch (...) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  try {
    vectorOfGraphs[infoGraph.second].addEdge(Vertex1, Vertex2, weight);
  } catch (std::logic_error & e) {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void khairullin::GraphSystem::cut(std::string & line)
{
  std::string graph = getToken(line);
  std::string Vertex1 = getToken(line);
  std::string Vertex2 = getToken(line);
  size_t weight = 0;
  auto infoGraph = graphExists(graph);
  if (!infoGraph.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  try {
    weight = std::stoi(getToken(line));
  } catch (...) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  try {
    vectorOfGraphs[infoGraph.second].cutEdge(Vertex1, Vertex2, weight);
  } catch (std::logic_error & e) {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void khairullin::GraphSystem::create(std::string & line)
{
  std::string nameGraph = getToken(line);
  auto infoGraph = graphExists(nameGraph);
  if (infoGraph.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t count = 0;
  try {
    count = std::stoi(getToken(line));
  } catch (...) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph graph(nameGraph);
  for (size_t i = 0; i < count; i++) {
    std::string vertex = getToken(line);
    try {
      if (vertex == "") {
        throw std::logic_error("<INVALID COMMAND>");
      }
      graph.addVertex(vertex);
    } catch (std::logic_error & e) {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
  try {
    vectorOfGraphs.pushBack(graph);
  } catch (...) {
    throw std::bad_alloc();
  }
}

void khairullin::GraphSystem::merge(std::string & line)
{
  std::string resultGraph = getToken(line);
  std::string nameGraph1 = getToken(line);
  std::string nameGraph2 = getToken(line);
  auto infoResult = graphExists(resultGraph);
  auto infoGraph1 = graphExists(nameGraph1);
  auto infoGraph2 = graphExists(nameGraph2);
  if ((!infoGraph1.first || !infoGraph2.first) || infoResult.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph result;
  Graph & gr2 = vectorOfGraphs[infoGraph2.second];
  try {
    result = vectorOfGraphs[infoGraph1.second];
    result.name = resultGraph;
  } catch (...) {
    throw std::bad_alloc();
  }
  for (size_t i = 0; i < gr2.vertexes.getSize(); i++) {
    auto infoVert = result.hasVertex(gr2.vertexes[i]);
    if (!infoVert.first) {
      result.addVertex(gr2.vertexes[i]);
    }
  }
  for (size_t i = 0; i < result.vertexes.getSize(); i++) {
    std::string vertex1 = result.vertexes[i];
    for (size_t j = 0; j < gr2.vertexes.getSize(); j++) {
      std::string vertex2 = gr2.vertexes[j];
      Vector< size_t > weights1;
      Vector< size_t > weights2;
      try {
        weights1 = gr2.edges.drop(vertex1 + vertex2);
      } catch (...) {
        throw std::bad_alloc();
      }
      try {
        for (size_t k = 0; k < weights1.getSize(); k++) {
          result.addEdge(vertex1, vertex2, weights1[k]);
        }
      } catch (...) {
        throw std::bad_alloc();
      }
    }
  }
  try {
    vectorOfGraphs.pushBack(result);
  } catch (...) {
    throw std::bad_alloc();
  }
}

void khairullin::GraphSystem::extract(std::string & line)
{
  std::string result = getToken(line);
  std::string basis = getToken(line);
  size_t count = 0;
  try {
    count = std::stoi(getToken(line));
  } catch (...) {
    throw std::logic_error("<INVALID COMMAND>");
  }

  auto infoBasis = graphExists(basis);
  if (!infoBasis.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }

  Graph resultGraph(result);
  Graph & basisGraph = vectorOfGraphs[infoBasis.second];
  for (size_t i = 0; i < count; i++) {
    std::string vertex = getToken(line);
    auto infoVert = basisGraph.hasVertex(vertex);
    if (!infoVert.first) {
      throw std::logic_error("<INVALID COMMAND>");
    }
    try {
      resultGraph.addVertex(vertex);
    } catch (...) {
      throw std::bad_alloc();
    }
  }
  for (size_t i = 0; i < basisGraph.vertexes.getSize(); i++) {
    for (size_t j = 0; j < basisGraph.vertexes.getSize(); j++) {
      std::string vertex1 = basisGraph.vertexes[i];
      std::string vertex2 = basisGraph.vertexes[j];
      auto infoVert1 = resultGraph.hasVertex(vertex1);
      auto infoVert2 = resultGraph.hasVertex(vertex2);
      if (!infoVert1.first) {
        break;
      } else if (!infoVert2.first) {
        continue;
      }
      Vector< size_t > weights;
      try {
        weights = basisGraph.edges.drop(vertex1 + vertex2);
      } catch (...) {
        throw std::bad_alloc();
      }
      for (size_t k = 0; k < weights.getSize(); k++) {
        resultGraph.addEdge(vertex1, vertex2, weights[k]);
      }
    }
  }
  try {
    vectorOfGraphs.pushBack(resultGraph);
  } catch (...) {
    throw std::bad_alloc();
  }
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) int main(int argc, char * argv[]);
#endif

#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>
#include "GraphSystem.h"
#include <sstream>

BOOST_AUTO_TEST_CASE(main_test)
{
  khairullin::HashTable< int, std::string, khairullin::Hash, khairullin::Equal< std::pair< int,
        std::string > > > hashTable;
  hashTable.add("hello", 100);
  khairullin::Vector< int > result = hashTable.drop("hello");
  BOOST_TEST(result[0] == 100);
  bool info = hashTable.has("hello", 100);
  BOOST_TEST(info == true);
  hashTable.cut("hello", 100);
  BOOST_TEST(hashTable.has("hello", 100) == false);

  khairullin::Graph graph;
  khairullin::Graph graph2;
  graph.addVertex("a");
  graph.addVertex("b");
  graph.addEdge("a", "b", 76);
  graph2.addVertex("a");
  graph2.addVertex("b");
  graph2.addEdge("a", "b", 76);
  BOOST_TEST(graph.hasVertex("a").first == true);
  BOOST_TEST(graph.edges.has("ab", 76) == true);
  BOOST_TEST(graph == graph2);
  khairullin::Graph copy(graph);
  BOOST_TEST(graph == copy);
  khairullin::Graph otherCopy;
  otherCopy = graph;
  BOOST_TEST(graph == otherCopy);
  BOOST_TEST(graph.hasConnection("a", "b") == true);
  graph.cutEdge("a", "b", 76);
  BOOST_TEST(graph.hasConnection("a", "b") == false);

  khairullin::GraphSystem system;
  std::string command = "graph1 4 a b c d";
  system.create(command);
  BOOST_TEST(system.vectorOfGraphs.getSize() == 1);
  khairullin::Graph testGraph("graph1");
  testGraph.addVertex("a");
  testGraph.addVertex("b");
  testGraph.addVertex("c");
  testGraph.addVertex("d");
  BOOST_TEST(system.vectorOfGraphs[0] == testGraph);
  std::stringstream stream;
  std::streambuf * buffer = std::cout.rdbuf(stream.rdbuf());
  system.graphs("");
  BOOST_TEST(stream.str() == "graph1");
  stream.str("");
  stream.clear();
  std::string temp = "graph1";
  system.vertexes(temp);
  BOOST_TEST(stream.str() == "a\nb\nc\nd\n");
  stream.str("");
  stream.clear();
  std::string boundTemp = "graph1 a";
  stream.clear();
  system.outbound(boundTemp);
  BOOST_TEST(stream.str() == "");
  stream.str("");
  stream.clear();
  system.inbound(boundTemp);
  BOOST_TEST(stream.str() == "");
  stream.str("");
  stream.clear();

  std::string bindTemp = "graph1 a b 100";
  system.bind(bindTemp);
  BOOST_TEST(system.vectorOfGraphs[0].hasConnection("a", "b") == true);
  system.cut(bindTemp);
  BOOST_TEST(system.vectorOfGraphs[0].hasConnection("a", "b") == false);

  std::string createTemp = "gr2 3 a b e";
  system.create(createTemp);
  std::string mergeTemp = "gr3 graph1 gr2";
  system.merge(mergeTemp);
  std::string gr3 = "gr3";
  system.vertexes(gr3);
  BOOST_TEST(stream.str() == "a\nb\nc\nd\ne\n");
  std::cout.rdbuf(buffer);
}

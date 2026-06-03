#include <iostream>
#include <string>
#include "Graph.h"
int main()
{
  /*khairullin::HashTable< std::string, int, khairullin::Hash< std::string >,
    khairullin::Equal< std::string > > hash_t(10);

  hash_t.insert(192, "first");
  hash_t.insert(143, "second");
  //hash_t.insert(134, "third");
  hash_t.insert(93, "fourth");
  hash_t.insert(14, "fifth");
  //hash_t.insert(15, "sixth");
  hash_t.insert(16, "sixth");
  //hash_t.remove("sixth");
  //hash_t.remove("fifth");
  //hash_t.remove("second");
  //hash_t.remove("fourth");
  //size_t index = hash_t.findHomeSlot("sixth");
  //std::cout << index << hash_t.table[index].value << std::endl;
  for (size_t i = 0; i < hash_t.getSize(); i++) {
    auto slot = hash_t.table[i];
    //if (!slot.Empty) {
      std::cout << slot.key << ": " << slot.value << " " << slot.PSL << std::endl;
    //}
  }*/

  khairullin::Graph< std::string > graph;
  graph.addNode("Semen");
  graph.addWithConnection("Semen", "hello");
  //for (size_t i = 0; i < graph.values.getSize(); ++i) {
    //std::cout << graph.values.table[i].key << " " << graph.values.table[i].value << std::endl;
  //}
  std::cout << graph.graph[1][0] << std::endl;
}
#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) int main(int argc, char * argv[]);
#endif

#define BOOST_TEST_MODULE F0
#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include "Social.h"
#include <sstream>

using namespace khairullin;
BOOST_AUTO_TEST_CASE(main_tests)
{
  HashTable< std::string, int, Hash< std::string >, Equal< std::string > > hashTable;
  hashTable.insert(100, "hello");
  auto result = hashTable.find("hello");
  BOOST_TEST(result.value() == 100);
  hashTable.remove("hello");
  BOOST_CHECK_THROW(hashTable.find("hello"), std::out_of_range);
  BOOST_TEST(hashTable.getCountOfElement() == 0);
  HashTable< std::string, int, Hash< std::string >, Equal< std::string > >
    testTable{{100, "hello"}, {200, "world"}, {300, "pizza"}, {32, "rege"}};
  BOOST_TEST(testTable.getSize() == 16);
  BOOST_TEST(testTable.getCountOfElement() == 4);
  HashTable< std::string, int, Hash< std::string >, Equal< std::string > > copyTable(testTable);
  BOOST_TEST(copyTable.getCountOfElement() == 4);
  BOOST_TEST(copyTable.getSize() == 16);
  BOOST_TEST(copyTable.find("hello").value() == 100);
  BOOST_TEST(copyTable.find("world").value() == 200);
  BOOST_TEST(copyTable.find("pizza").value() == 300);
  BOOST_TEST(copyTable.find("rege").value() == 32);
  BOOST_CHECK(copyTable == testTable);

  Graph< std::string > graph("Friends");
  BOOST_TEST(graph.getName() == "Friends");
  BOOST_TEST(graph.getVertices() == 0);
  graph.addNode("Bob");
  graph.addNode("Anne");
  BOOST_TEST(graph.hasVertex("Bob").first);
  BOOST_TEST(graph.hasVertex("Anne").first);
  graph.connect("Bob", "Anne");
  BOOST_TEST(graph.path("Bob", "Anne") == 1);
  graph.disconnect("Bob", "Anne");
  graph.deleteNode("Bob");
  BOOST_TEST(!graph.hasVertex("Bob").first);
  BOOST_TEST(graph.getVertices() == 1);
  graph.addWithConnection("Bob", "Urik");
  BOOST_TEST(graph.path("Bob", "Urik") == 1);

  Social social;
  std::string line = "Twitter";
  social.makeSocial(line);
  BOOST_TEST(social.socials[0].getName() == "Twitter");
  BOOST_TEST(social.hasSocial("Twitter").first);
  line += " Cartman";
  social.addUser(line);
  BOOST_TEST(social.socials[0].hasVertex("Cartman").first);
  boost::test_tools::output_test_stream tester;
  std::streambuf * output = std::cout.rdbuf(tester.rdbuf());
  std::string name = "Cartman";
  std::string yasoc = "Facebook";
  line = yasoc + " " + name;
  social.makeSocial(yasoc);
  social.addUser(line);
  social.findUser(name);
  BOOST_TEST(tester.is_equal("Twitter, Facebook\n"));
  name = "Kyle";
  social.addUser(name);
  line = "Twitter Cartman Kyle";
  std::string line2 = "Twitter Cartman";
  social.makeFriends(line);
  BOOST_TEST(tester.is_equal("From now on, Cartman and Kyle are friends in Twitter\n"));
  social.checkFriendship(line);
  BOOST_TEST(tester.is_equal("They are friends in the Twitter\n"));
  social.countOfFriends(line2);
  BOOST_TEST(tester.is_equal("1"));
  social.stopFriendship(line);
  BOOST_TEST(tester.is_equal("Cartman and Kyle are not friends from this moment\n"));
  social.checkFriendship(line);
  BOOST_TEST(tester.is_equal("Cartman and Kyle are NOT friends in the Twitter\n"));
  line2 += " other";
  social.getRecommendation(line2);
  BOOST_CHECK(!tester.is_empty());
  social.seekPotentialFriends(line2);
  BOOST_TEST(!tester.is_empty());
  std::cout.rdbuf(output);
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) int main(int argc, char * argv[]);
#endif

#define BOOST_TEST_MODULE S4
#include <boost/test/included/unit_test.hpp>
#include "Datasets.h"
#include <sstream>

using namespace khairullin;

BOOST_AUTO_TEST_CASE(main_test)
{
  khairullin::BSTree< size_t, std::string, khairullin::Compare< size_t > > * tree =
    new khairullin::BSTree< size_t, std::string,
      khairullin::Compare< size_t > > (8, "", nullptr);
  tree->push(10, "");
  tree->push(3, "hello");
  tree->push(1,"");
  tree->push(6,"");
  tree->push(4, "");
  tree->push(14,"");
  tree->push(13,"");
  auto iterator = khairullin::iterator< size_t, std::string, khairullin::Compare< size_t > >(tree);
  BOOST_TEST(tree->has(3) == true);
  BOOST_TEST(tree->has(2) == false);
  BOOST_TEST(iterator.has(6) == true);
  BOOST_TEST(iterator.has(15) == false);
  BOOST_TEST(tree->drop(4) == tree);
  iterator = iterator.write(12, "");
  BOOST_TEST(tree->has(12) == true);
  BOOST_TEST(tree->get(3) == "hello");
  auto beg = iterator.begin();
  auto end = iterator.end();
  std::pair< size_t, std::string> expected1 = std::make_pair(1,"");
  std::pair< size_t, std::string> expected2 = std::make_pair(14,"");
  BOOST_TEST(beg.read() == expected1);
  BOOST_TEST(end.read() == expected2);
  BOOST_TEST(tree->height() == 5);
  BOOST_TEST(tree->height(beg.root) == 1);
  BOOST_TEST((tree->fallLeft())->data.first == 1);
  BOOST_TEST((tree->fallRight())->data.first == 14);
  BOOST_TEST(tree == *iterator);
}

#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "List.hpp"
BOOST_AUTO_TEST_CASE(List_test)
{
  khairullin::List< int > list;
  list.addBegin(6);
  list.addBegin(7);
  list.addEnd(89);
  list.addBegin(10);
  list.addBegin(20);
  list.addBegin(30);
  list.addEnd(1321);

  auto iter = list.begin();
  auto citer = list.cbegin();
  BOOST_TEST(*iter == *citer);
  BOOST_TEST(*iter == 30);
  iter++;
  iter.insert(39);
  iter++;
  BOOST_TEST(*iter == 39);

  khairullin::List< int > otherList(list);
  BOOST_TEST(list == otherList);
  otherList.clear();

  otherList = list;
  BOOST_TEST(list == otherList);

  khairullin::List< int > copy(std::move(list));
  BOOST_TEST(list == copy);
  copy.clear();
  copy = std::move(list);
  BOOST_TEST(list == copy);
}

BOOST_AUTO_TEST_CASE(List_test2)
{
  khairullin::List< int > list1;
  khairullin::List< int > list2;
  list1.addBegin(6);
  list1.addBegin(7);
  list1.addEnd(89);
  list2.addBegin(6);
  list2.addBegin(7);
  list2.addEnd(89);
  BOOST_TEST(list1 == list2);

  list1.cut(7);
  BOOST_TEST(*(list.begin()) == 6);

  list1.addEnd(2143);
  list1.addEnd(2144);
  auto iter = list1.begin();
  BOOST_TEST(iter.hasNext() == true);
  while (iter.hasNext()) {
    iter++;
  }
  BOOST_TEST(*iter == 2144);

  list2.cut(7);
  BOOST_TEST(*(list2.cbegin()) == 6);

  list2.addEnd(2143);
  list2.addEnd(2144);
  auto citer = list2.cbegin();
  BOOST_TEST(citer.hasNext() == true);
  while (citer.hasNext()) {
    citer++;
  }
  BOOST_TEST(*citer == 2144);
}

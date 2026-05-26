#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "List.hpp"
BOOST_AUTO_TEST_CASE(List_test)
{
  khairullin::List< int > intList;
  intList.addBegin(6);
  intList.addBegin(7);
  intList.addEnd(89);
  intList.addBegin(10);
  intList.addBegin(20);
  intList.addBegin(30);
  intList.addEnd(1321);

  auto iter = intList.begin();
  auto citer = intList.cbegin();
  BOOST_TEST(*iter == *citer);
  BOOST_TEST(*iter == 30);
  iter++;
  iter.insert(39);
  iter++;
  BOOST_TEST(*iter == 39);

  khairullin::List< int > otherList(intList);
  auto it1 = intList.begin();
  auto it2 = otherList.begin();
  while (it1 != intList.end()) {
    BOOST_TEST(*it1 == *it2);
    it1++;
    it2++;
  }
  otherList.clear();

  otherList = intList;
  it1 = intList.begin();
  it2 = otherList.begin();
  while (it1 != intList.end()) {
    BOOST_TEST(*it1 == *it2);
    it1++;
    it2++;
  }

  khairullin::List< int > copy(std::move(intList));
  BOOST_TEST(intList == copy);
  copy.clear();
  copy = std::move(intList);
  it1 = intList.begin();
  it2 = copy.begin();
  while (it1 != intList.end()) {
    BOOST_TEST(*it1 == *it2);
    it1++;
    it2++;
  }
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
  auto it1 = list1.begin();
  auto it2 = list2.begin();
  for (size_t i = 0; i < 3; i++) {
    BOOST_TEST(it1 == it2);
    it1++;
    it2++;
  }
  list1.cut(7);
  BOOST_TEST(*(list1.begin()) == 6);

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

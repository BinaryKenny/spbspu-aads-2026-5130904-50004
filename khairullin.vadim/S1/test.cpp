#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) int main(int argc, char* argv[]);
#endif

#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "List.hpp"
#include "List.cpp"
#include "LIter.hpp"
#include "LIter.cpp"
#include "CLIter.hpp"
#include "CLIter.cpp"

BOOST_AUTO_TEST_CASE(List_test)
{
  int number = 1000;
  int number_2 = 1001;
  khairullin::List<int> * head = new khairullin::List<int>(number, nullptr);
  khairullin::LIter<int> iter = khairullin::LIter<int>(head);
  khairullin::CLIter<int> const_iter = khairullin::CLIter<int>(head);
  BOOST_TEST(iter.value() == number);
  BOOST_TEST(const_iter.value() == number);
  BOOST_TEST(const_iter.hasNext() == false);
  iter = iter.insert_value(number_2);
  BOOST_TEST(iter.value() == number_2);
}

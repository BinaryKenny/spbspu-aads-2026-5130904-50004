#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) int main(int argc, char* argv[]);
#endif

#define BOOST_TEST_MODULE S2
#include "main.cpp"
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <sstream>

BOOST_AUTO_TEST_CASE(main_test)
{
  Queue<int> q;
  size_t size = 5;
  for (size_t i = 0; i < size; i++)
  {
    size_t num = i * i + 1;
    q.push(num);
  }
  Queue<int> new_q(q);
  size_t last = (size - 1) * (size - 1) + 1;
  BOOST_TEST(new_q.drop() == last);

  Stack<int> stack;
  while(q.not_empty())
  {
    stack.push(q.drop());
  }
  BOOST_TEST(stack.drop() == last);

  Queue<Data> q1;
  Queue<Data> q2;
  std::stringstream in;
  std::stringstream out;
  in << "4 * 3 - 17 / 10";
  q1 = input(in);
  postfix(q1, q2);
  while (q2.not_empty())
  {
    Data val = q2.drop();
    if (val.is_int())
    {
      out << val.value();
    }
    else
    {
      out << val.char_value();
    }
  }
  BOOST_TEST(out.str() == "43*1710/-")

  int number = 23546;
  BOOST_TEST(reverse(number) == 64532);
}

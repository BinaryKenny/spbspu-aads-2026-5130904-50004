#include "List.hpp"
#include "List.cpp"
#include "LIter.hpp"
#include "LIter.cpp"
#include "CLIter.hpp"
#include "CLIter.cpp"
#include <iostream>
#include <string>

int main()
{
  List<std::string> * fake_str = List<std::string>::fake(nullptr);
  List<List<int>*> * fake_ll = List<List<int>*>::fake(nullptr);
  List<LIter<int>> * fake_LIter = List<LIter<int>>::fake(nullptr);
  List<LIter<int>> * fake_CLIter = List<LIter<int>>::fake(nullptr);
  List<int> * fake_sum = List<int>::fake(nullptr);

  LIter<std::string> iter_str = LIter<std::string>(fake_str);
  LIter<List<int>*> iter_ll = LIter<List<int>*>(fake_ll);
  LIter<LIter<int>> iter_LIter = LIter<LIter<int>>(fake_LIter);
  LIter<int> iter_sum = LIter<int>(fake_sum);

  CLIter<std::string> Citer_str = CLIter<std::string>(fake_str);
  LIter<LIter<int>> Citer_LIter = LIter<LIter<int>>(fake_CLIter);
  CLIter<List<int>*> Citer_ll = CLIter<List<int>*>(fake_ll);
  CLIter<int> Citer_sum = CLIter<int>(fake_sum);

  while (true)
  {
    std::string temp_str = "";
    std::cin >> temp_str;
    if (std::cin.eof())
    {
      break;
    }
    iter_str = iter_str.insert_value(temp_str);
    int temp_int = 0;
    while (std::cin >> temp_int)
    {
      if (!iter_ll.hasNext())
      {
        List<int> * fake_int = List<int>::fake(nullptr);
        LIter<int> iter_int = LIter<int>(fake_int);
        iter_ll = iter_ll.insert_value(fake_int);
        iter_LIter = iter_LIter.insert_value(iter_int);
        Citer_LIter = Citer_LIter.insert_value(iter_int);
        iter_int = iter_int.insert_value(temp_int);
        iter_sum = iter_sum.insert_value(temp_int);
      }
      else
      {
        iter_sum = iter_sum.next();
        iter_LIter = iter_LIter.next();
        if (iter_LIter.value().hasNext())
        {
          iter_LIter.value() = iter_LIter.value().next();
        }
        iter_ll = iter_ll.next();
        iter_LIter.value() = iter_LIter.value().insert_value(temp_int);
        iter_sum.value() += temp_int;
      }
    }
    if (std::cin.eof())
    {
      break;
    }
    else
    {
      std::cin.clear();
      iter_ll = iter_ll.begin(fake_ll);
      iter_LIter = iter_LIter.begin(fake_LIter);
      iter_sum = iter_sum.begin(fake_sum);
    }
  }

  Citer_LIter = Citer_LIter.begin(fake_CLIter);

  if (!Citer_str.hasNext())
  {
    std::cout << "0\n";
    std::cerr << "There is no sequence, the amount can't be calculated\n";
    Citer_ll.list->cut_fake(fake_ll);
    Citer_LIter.list->cut_fake(fake_CLIter);
    iter_LIter.list->cut_fake(fake_LIter);
    Citer_sum.list->cut_fake(fake_sum);
    Citer_str.list->cut_fake(fake_str);
    return 1;
  }

  while (Citer_str.hasNext())
  {
    Citer_str = Citer_str.next();
    std::cout << Citer_str.value() << " ";
  }
  std::cout << "\n";

  while (Citer_LIter.hasNext())
  {
    Citer_LIter = Citer_LIter.next();
    LIter<int> iter_int = Citer_LIter.value();
    while (iter_int.hasNext())
    {
      iter_int = iter_int.next();
      std::cout << iter_int.value() << " ";
    }
    std::cout << "\n";
  }
  while(Citer_sum.hasNext())
  {
    Citer_sum = Citer_sum.next();
    std::cout << Citer_sum.value() << " ";
  }
  std::cout << "\n";

  Citer_sum = Citer_sum.begin(fake_sum);
  Citer_sum = Citer_sum.list->cut_fake(fake_sum);
  Citer_sum.list->clear(Citer_sum.list);

  Citer_str = Citer_str.begin(fake_str);
  Citer_str = Citer_str.list->cut_fake(fake_str);
  Citer_str.list->clear(Citer_str.list);

  Citer_LIter = Citer_LIter.begin(fake_CLIter);
  Citer_LIter = Citer_LIter.list->cut_fake(Citer_LIter.list);
  Citer_LIter.list->clear(Citer_LIter.list);

  iter_LIter = iter_LIter.begin(fake_LIter);
  iter_LIter = iter_LIter.list->cut_fake(iter_LIter.list);
  iter_LIter.list->clear(iter_LIter.list);
}

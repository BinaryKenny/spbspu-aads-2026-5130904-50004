#include "List.hpp"
#include "List.cpp"
#include "LIter.hpp"
#include "LIter.cpp"
#include "CLIter.hpp"
#include "CLIter.cpp"
#include "delete_list.hpp"
#include "delete_list.cpp"
#include <iostream>
#include <string>
#include <limits>

using ull = unsigned long long;
namespace khair = khairullin;
int main()
{
  khair::List<std::string> * fake_str = khair::List<std::string>::fake(nullptr);
  khair::List<khair::List<ull>*> * fake_ll = khair::List<khair::List<ull>*>::fake(nullptr);
  khair::List<khair::LIter<ull>> * fake_LIter = khair::List<khair::LIter<ull>>::fake(nullptr);
  khair::List<khair::LIter<ull>> * fake_CLIter = khair::List<khair::LIter<ull>>::fake(nullptr);
  khair::List<ull> * fake_sum = khair::List<ull>::fake(nullptr);

  khair::LIter<std::string> iter_str = khair::LIter<std::string>(fake_str);
  khair::LIter<khair::List<ull>*> iter_ll = khair::LIter<khair::List<ull>*>(fake_ll);
  khair::LIter<khair::LIter<ull>> iter_LIter = khair::LIter<khair::LIter<ull>>(fake_LIter);
  khair::LIter<ull> iter_sum = khair::LIter<ull>(fake_sum);

  khair::CLIter<std::string> Citer_str = khair::CLIter<std::string>(fake_str);
  khair::LIter<khair::LIter<ull>> Citer_LIter = khair::LIter<khair::LIter<ull>>(fake_CLIter);
  khair::CLIter<ull> Citer_sum = khair::CLIter<ull>(fake_sum);

  const ull MAX = std::numeric_limits<ull>::max();
  bool overflow = false;
  while (true)
  {
    std::string temp_str = "";
    std::cin >> temp_str;
    if (std::cin.eof())
    {
      break;
    }
    iter_str = iter_str.insert_value(temp_str);
    ull temp_ull = 0;
    while (std::cin >> temp_ull)
    {
      try
      {
        if (!iter_ll.hasNext())
        {
          khair::List<ull> * fake_ull = khair::List<ull>::fake(nullptr);
          khair::LIter<ull> iter_ull = khair::LIter<ull>(fake_ull);
          iter_ll = iter_ll.insert_value(fake_ull);
          iter_LIter = iter_LIter.insert_value(iter_ull);
          Citer_LIter = Citer_LIter.insert_value(iter_ull);
          iter_ull = iter_ull.insert_value(temp_ull);
          iter_sum = iter_sum.insert_value(temp_ull);
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
          iter_LIter.value() = iter_LIter.value().insert_value(temp_ull);
          if (iter_sum.value() < MAX - temp_ull)
          {
            iter_sum.value() += temp_ull;
          }
          else
          {
            overflow = true;
          }
        }
      }
      catch (...)
      {
        std::cerr << "Bad alloc\n";
        khair::delete_LL(fake_ll);
        khair::delete_List(fake_sum);
        khair::delete_List(fake_str);
        khair::delete_List(fake_CLIter);
        khair::delete_List(fake_LIter);
        return 1;
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

  while (Citer_str.hasNext())
  {
    Citer_str = Citer_str.next();
    std::cout << Citer_str.value();
    if (Citer_str.hasNext())
    {
      std::cout << " ";
    }
    else
    {
      std::cout << "\n";
    }
  }

  if (!Citer_sum.hasNext())
  {
    std::cout << "0\n";
  }
  while (Citer_LIter.hasNext())
  {
    Citer_LIter = Citer_LIter.next();
    khair::LIter<ull> iter_ull = Citer_LIter.value();
    while (iter_ull.hasNext())
    {
      iter_ull = iter_ull.next();
      std::cout << iter_ull.value();
      if (iter_ull.hasNext())
      {
        std::cout << " ";
      }
      else
      {
        std::cout << "\n";
      }
    }
  }

  if (!overflow)
  {
    while(Citer_sum.hasNext())
    {
      Citer_sum = Citer_sum.next();
      std::cout << Citer_sum.value();
      if (Citer_sum.hasNext())
      {
        std::cout << " ";
      }
      else
      {
        std::cout << "\n";
      }
    }
  }

  khair::delete_LL(fake_ll);
  khair::delete_List(fake_sum);
  khair::delete_List(fake_str);
  khair::delete_List(fake_CLIter);
  khair::delete_List(fake_LIter);

  if (overflow)
  {
    std::cerr << "Overflow\n";
    return 1;
  }
}

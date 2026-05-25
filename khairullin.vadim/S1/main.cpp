#include "List.hpp"
#include <iostream>
#include <string>
#include <limits>

using ull = unsigned long long;
namespace khair = khairullin;
using strList = khairullin::List<std::string>;
using LList = khairullin::List<khair::List<ull>>;
using numberList = khairullin::List<ull>;
using iterator = khairullin::ListIterator<ull>;
using const_iterator = khairullin::ConstListIterator<ull>;
using listsIterator = khairullin::ListIterator< numberList >;
using stringIterator = khairullin::ListIterator<std::string>;

static ull MAX = std::numeric_limits<ull>::max();
int main()
{
  strList stringList;
  LList listOfLists;
  size_t count = 0;
  while (std::cin) {
    std::string str = "";
    std::cin >> str;
    try {
      stringList.addEnd(str);
    }
    catch (std::exception & e) {
      std::cerr << e.what() << "\n";
      return 2;
    };
    ull temp = 0;
    numberList ullList;
    while (std::cin >> temp) {
      try {
        ullList.addEnd(temp);
      }
      catch (std::exception & e) {
        std::cerr << e.what() << "\n";
        return 2;
      }
    }
    listOfLists.addEnd(ullList);
    count++;
    if (std::cin.eof()) {
      break;
    }
    else {
      std::cin.clear();
    }
  }
  if (count == 0) {
    std::cout << 0;
    return 0;
  }
  iterator * iterators = new iterator[count];
  listsIterator listOfListsIterator = listOfLists.begin();
  for (size_t i = 0; i < count; i++) {
    iterators[i] = (*listOfListsIterator).begin();
    listOfListsIterator++;
  }
  stringIterator strIter = stringList.begin();
  std::cout << *strIter;
  strIter++;
  while (strIter != stringList.end()) {
    std::cout << " " << *strIter;
    strIter++;
  }
  std::cout << "\n";
  numberList sum;
  bool overflow = false;
  while (true) {
    size_t amount = 0;
    bool hasActive = false;
    for (size_t i = 0; i < count - 1; i++) {
      if (iterators[i] != iterator{}) {
        hasActive = true;
        std::cout << *iterators[i] << " ";
        if (MAX - amount < *(iterators[i])) {
          overflow = true;
        }
        amount += *(iterators[i]);
        iterators[i]++;
      }
    }
    size_t temp = count - 1;
    if (iterators[temp] != iterator{}) {
      std::cout << *iterators[temp];
      if (MAX - amount < *(iterators[temp])) {
        overflow = true;
      }
      amount += *(iterators[temp]);
      iterators[temp]++;
    }
    if (!hasActive) {
      break;
    }
    sum.addEnd(amount);
    std::cout << "\n";
  }
  const_iterator citer = sum.cbegin();
  std::cout << *citer;
  citer++;
  while (citer != sum.cend()) {
    std::cout << " " << *citer;
    citer++;
  }
  std::cout << "\n";
}

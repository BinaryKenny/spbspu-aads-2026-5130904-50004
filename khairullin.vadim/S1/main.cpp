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

static const ull MAX = std::numeric_limits<ull>::max();
int main()
{
  strList stringList;
  LList listOfLists;
  size_t count = 0;
  while (std::cin) {
    std::string str = "";
    if (!(std::cin >> str)) {
      break;
    }
    try {
      stringList.push_back(str);
    }
    catch (std::exception & e) {
      std::cerr << e.what() << "\n";
      return 2;
    };
    ull temp = 0;
    numberList ullList;
    while (std::cin >> temp) {
      try {
        ullList.push_back(temp);
      }
      catch (std::exception & e) {
        std::cerr << e.what() << "\n";
        return 2;
      }
    }
    listOfLists.push_back(ullList);
    count++;
    if (std::cin.eof()) {
      break;
    }
    else {
      std::cin.clear();
    }
  }
  iterator * iterators = new iterator[count];
  listsIterator listOfListsIterator = listOfLists.begin();
  for (size_t i = 0; i < count; i++) {
    iterators[i] = (*listOfListsIterator).begin();
    listOfListsIterator++;
  }
  stringIterator strIter = stringList.begin();
  if (strIter != stringList.end()) {
    std::cout << *strIter;
    strIter++;
    while (strIter != stringList.end()) {
      std::cout << " " << *strIter;
      strIter++;
    }
  }
  else {
    delete [] iterators;
    std::cout << 0 << "\n";
    return 0;
  }
  std::cout << '\n';
  numberList sum;
  bool overflow = false;
  while (true) {
    size_t amount = 0;
    size_t temp = 0;
    while (temp != count && iterators[temp] == iterator{}) {
      temp++;
    }
    if (temp == count) {
      break;
    }
    else {
      std::cout << *iterators[temp];
      amount += *iterators[temp];
      iterators[temp]++;
    }
    for (size_t i = temp + 1; i < count; i++) {
      if (iterators[i] != iterator{}) {
        std::cout << " " << *iterators[i];
        if (MAX - amount < *(iterators[i])) {
          overflow = true;
        }
        amount += *(iterators[i]);
        iterators[i]++;
      }
    }
    sum.push_back(amount);
    std::cout << '\n';
  }
  if (overflow) {
    std::cerr << "<OVERFLOW>\n";
    delete [] iterators;
    return 1;
  }
  const_iterator citer = sum.cbegin();
  if (citer != sum.cend()) {
    std::cout << *citer;
    citer++;
    while (citer != sum.cend()) {
      std::cout << " " << *citer;
      citer++;
    }
    std::cout << "\n";
  }
  else {
    std::cout << 0 << '\n';
  }
  delete [] iterators;
}

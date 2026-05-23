#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include <string>
#include "Vector.h"

namespace khairullin {
  inline std::string getToken(std::string & line)
  {
    std::string token = "";
    std::string result = "";
    size_t i = 0;
    for (; i < line.length() && line[i] != ' ' && line[i] != '\n'; i++) {
      token += line[i];
    }
    if (i >= line.length()) {
      line.clear();
      return token;
    }
    i++;
    for (; i < line.length() && line[i] != '\n' && line[i] != '\r'; i++) {
      result += line[i];
    }
    line = result;
    return token;
  }

  template< class T, class U >
  void sortPair(Vector< std::pair< T, U > > & vector)
  {
    for (size_t i = 0; i < vector.getSize() - 1; i++) {
      for (size_t j = 0; j < vector.getSize() - i - 1; j++) {
        if (vector[j].first > vector[j + 1].first) {
          std::swap(vector[j], vector[j + 1]);
        }
      }
    }
  }

  template< class T >
  void sortVector(Vector< T > & vector)
  {
    for (size_t i = 0; i < vector.getSize() - 1; i++) {
      for (size_t j = 0; j < vector.getSize() - i - 1; j++) {
        if (vector[j] > vector[j + 1]) {
          std::swap(vector[j], vector[j + 1]);
        }
      }
    }
  }

  template< class T, class U >
  std::pair< bool, size_t > hasInPair(const Vector< std::pair< T, U > > & vector, const T & value)
  {
    for (size_t i = 0; i < vector.getSize(); i++) {
      if (vector[i].first == value) {
        return std::make_pair(true, i);
      }
    }
    return std::make_pair(false, 0);
  }
}
#endif

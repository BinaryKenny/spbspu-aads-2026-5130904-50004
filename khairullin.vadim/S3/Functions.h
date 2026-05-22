#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include <string>
#include "Vector.h"

namespace khairullin {
  std::string getToken(std::string & line);

  template< class T, class U >
  void sortPair(khairullin::Vector< std::pair< T, U > > & vector);

  template< class T >
  void sortVector(khairullin::Vector< T > & vector);

  template< class T, class U >
  std::pair< bool, size_t > hasInPair(const Vector< std::pair< T, U > > & vector, const T & value);
}
#endif

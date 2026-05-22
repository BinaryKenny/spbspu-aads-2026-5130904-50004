#include "Functions.h"

std::string khairullin::getToken(std::string & line)
{
  std::string token = "";
  std::string result = "";
  size_t new_start = 0;
  for (size_t i = 0; i < line.length() && line[i] != ' '; i++) {
    token += line[i];
    new_start = i;
  }
  if (new_start == line.length() - 1) {
    line = result;
    return token;
  }
  new_start += 2;
  for (size_t i = new_start; i < line.length() && line[i] != '\0'; i++) {
    result += line[i];
  }
  line = result;
  return token;
}

template< class T, class U >
void khairullin::sortPair(Vector< std::pair< T, U > > & vector)
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
void khairullin::sortVector(khairullin::Vector< T > & vector)
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
std::pair< bool, size_t > khairullin::hasInPair(const Vector< std::pair< T, U > > & vector, const T & value)
{
  for (size_t i = 0; i < vector.getSize(); i++) {
    if (vector[i].first == value) {
      return std::make_pair(true, i);
    }
  }
  return std::make_pair(false, 0);
}

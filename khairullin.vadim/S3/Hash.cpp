#include "Hash.h"

size_t khairullin::Hash::operator()(const std::string & key) const {
  return key.length() + key[0];
}

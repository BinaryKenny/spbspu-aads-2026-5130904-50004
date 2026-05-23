#ifndef HASH_H
#define HASH_H
#include <string>

namespace khairullin {
  struct Hash {
    size_t operator()(const std::string & key) const;
  };
}
#endif

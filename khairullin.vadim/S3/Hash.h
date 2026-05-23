#ifndef HASH_H
#define HASH_H
#include <string>
#include <boost/hash2/hmac.hpp>
#include <boost/hash2/sha2.hpp>

namespace khairullin {
  struct Hash {
    size_t operator()(const std::string & key) const;
  };
}
#endif

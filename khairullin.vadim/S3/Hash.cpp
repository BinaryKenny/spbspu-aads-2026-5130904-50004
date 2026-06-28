#include "Hash.h"

size_t khairullin::Hash::operator()(const std::string & key) const {
  static const std::string secret_key = "Sipulka";
  boost::hash2::hmac<boost::hash2::sha2_256> hmac(
  reinterpret_cast<const unsigned char*>(secret_key.data()), secret_key.size());
  const std::string& str = key;
  hmac.update(reinterpret_cast<const unsigned char*>(str.data()), str.size());
  auto result = hmac.result();
  size_t hash = 0;
  for (size_t i = 0; i < sizeof(size_t) && i < result.size(); ++i) {
    hash = (hash << 8) | result[i];
  }
  return hash;
}

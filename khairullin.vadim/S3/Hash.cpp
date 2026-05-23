#include "Hash.h"

size_t khairullin::Hash::operator()(const std::string & key) const {
  static const std::string secret_key = "Sipulka";
  boost::hash2::hmac<boost::hash2::sha2_256> hmac(
      reinterpret_cast<const unsigned char*>(secret_key.data()),
      secret_key.size());
  hmac.update(reinterpret_cast<const unsigned char*>(key.data()), key.size());
  auto result = hmac.result();
  size_t hash = 0;
  // Если result имеет метод data() (Boost ≥1.84)
  const unsigned char* bytes = result.data();
  size_t len = result.size();
  for (size_t i = 0; i < std::min(sizeof(size_t), len); ++i) {
    hash = (hash << 8) | bytes[i];
  }
  return hash;
}

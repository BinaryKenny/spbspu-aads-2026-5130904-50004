#include "Hash.h"

size_t khairullin::Hash::operator()(const std::string & key) const {
  static const std::string secret_key = "Sipulka";
  boost::hash2::hmac<boost::hash2::sha2_256> hmac(
      reinterpret_cast<const unsigned char*>(secret_key.data()),
      secret_key.size());
  hmac.update(reinterpret_cast<const unsigned char*>(key.data()), key.size());
  unsigned char digest[32];
  boost::hash2::get_digest(hmac.result(), digest);
  size_t hash = 0;
  for (int i = 0; i < sizeof(size_t) && i < 32; ++i) {
    hash = (hash << 8) | digest[i];
  }
  return hash;
}

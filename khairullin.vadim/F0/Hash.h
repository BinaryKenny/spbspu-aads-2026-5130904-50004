#ifndef HASH_H
#define HASH_H
namespace khairullin {
  template< class Key >
  struct Hash {
    size_t operator()(Key skey) const;
  };
}

template< class Key >
size_t khairullin::Hash< Key >::operator()(Key skey) const
{
    size_t len = skey.size();
    char * key = new char[len];
    for (size_t i = 0; i < len; i++) {
        key[i] = skey[i];
    }
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned int h = seed ^ len;

    const unsigned char * data = (const unsigned char *)key;
    unsigned int k = 0;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    delete [] key;
    return h;
}
#endif

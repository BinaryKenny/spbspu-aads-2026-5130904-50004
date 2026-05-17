#ifndef COMPARE_H
#define COMPARE_H

namespace khairullin {
  template< class Key >
  struct Compare {
    bool operator()(const Key & a, const Key & b) const
    {
      return a < b;
    }
  };
}
#endif //COMPARE_H

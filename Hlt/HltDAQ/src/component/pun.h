#ifndef PUN_H
#define PUN_H
#include <cstring>

// type punning in way which does not rely on C99 compatiblity...
// (i.e. do _not_ use a union, but use std::memcpy, which does
// a reinterpret_cast<unsigned char*> behind the scenes, and thus
// avoids undefined behaviour -- see eg. the 'notes' section of
// http://en.cppreference.com/w/cpp/string/byte/memcpy)
// [ note: gcc explicitly supports the use of a union for type-punning
// as an extension -- and generates the same code as the std::memcpy version ]

template <typename Out, typename In>
constexpr Out pun_to(In src) {
  static_assert(sizeof(In)==sizeof(Out),"In and Out types must have same size");
  Out dest{0};
  std::memcpy(&dest,&src,sizeof(Out));
  return dest;
}

template<>
constexpr double pun_to<double,unsigned int>(unsigned int in) {
  return pun_to<float>(in);
}

template<>
constexpr unsigned int pun_to<unsigned int,double>(double in) {
  return pun_to<unsigned int>(static_cast<float>(in));
}

#endif

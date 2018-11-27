/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef PUN_H
#define PUN_H
#include <cstring>
#include <type_traits>

// type punning in way which does not rely on C99 compatiblity...
// (i.e. do _not_ use a union, but use std::memcpy, which does
// a reinterpret_cast<unsigned char*> behind the scenes, and thus
// avoids undefined behaviour -- see eg. the 'notes' section of
// http://en.cppreference.com/w/cpp/string/byte/memcpy)
// [ note: gcc explicitly supports the use of a union for type-punning
// as an extension -- and generates the same code as the std::memcpy version ]
//

template <typename Out, typename In>
constexpr Out pun_to(In src) {
  static_assert(sizeof(In)==sizeof(Out),"In and Out types must have same size");
  static_assert(std::is_same<std::decay_t<Out>,unsigned int>::value
                || std::is_same<std::decay_t<In>,unsigned int>::value,
                "Either In or Out must be unsigned int");
  static_assert(!std::is_same<std::decay_t<Out>,std::decay_t<In>>::value,
                "Identy punning not allowed -- just remove the punning!");
  Out dest{0};
  std::memcpy(&dest,&src,sizeof(Out));
  return dest;
}

template<>
constexpr unsigned int pun_to<unsigned int,double>(double in) {
  return pun_to<unsigned int>(static_cast<float>(in));
}

#endif

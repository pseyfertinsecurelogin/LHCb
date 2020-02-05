/*****************************************************************************\
* (c) Copyright 2020 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once
#include <type_traits>

namespace LHCb::Utils {

  // Helper to force the compiler to unroll a loop
  template <int Start, int End, int Step = 1, typename F, typename... Args>
  constexpr inline __attribute__( ( always_inline ) ) void unwind( F const& f, Args&&... args ) {
    static_assert( Step != 0 );
    static_assert( ( Step > 0 && Start <= End ) || ( Step < 0 && Start >= End ) );
    if constexpr ( Start != End ) {
      f( std::integral_constant<int, Start>{}, args... );
      unwind<Start + Step, End, Step>( f, args... );
    }
  }

  // Helper to force the compiler to unroll a sum
  template <int Start, int End, int Step = 1, typename F, typename... Args>
  [[gnu::pure]] constexpr __attribute__( ( always_inline ) ) auto unwind_sum( F const& f, Args&&... args ) {
    auto s = f( std::integral_constant<int, Start>{}, args... );
    unwind<Start + Step, End, Step>( [&]( auto i ) { s = s + f( i, args... ); } );
    return s;
  }

} // namespace LHCb::Utils

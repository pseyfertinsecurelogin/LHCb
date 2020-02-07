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
#include "GaudiKernel/detected.h"

#include <memory>
#include <type_traits>

namespace LHCb {
  namespace details_au {
    template <typename T>
    using has_get_allocator_ = decltype( std::declval<T>().get_allocator() );

    template <typename T>
    inline constexpr bool has_get_allocator_v = Gaudi::cpp17::is_detected_v<has_get_allocator_, T>;
  } // namespace details_au

  /** Construct a new object of type T using the given parameter pack and an allocator obtained from 'old' if possible.
   *
   * The body of this helper could be simplified with C++20's std::make_obj_using_allocator.
   */
  template <typename T, typename Old, typename... Args>
  std::decay_t<T> make_obj_propagating_allocator( Old const& old, Args&&... args ) {
    // Check if old.get_allocator() is valid
    if constexpr ( details_au::has_get_allocator_v<Old> ) {
      // Figure out if the allocator we can get is useful
      using Alloc = std::decay_t<decltype( old.get_allocator() )>;
      if constexpr ( std::uses_allocator_v<std::decay_t<T>, Alloc> ) {
        // We should be able to use it!
        if constexpr ( std::is_constructible_v<std::decay_t<T>, std::allocator_arg_t, Alloc const&, Args...> ) {
          return std::decay_t<T>{std::allocator_arg, old.get_allocator(), std::forward<Args>( args )...};
        } else {
          static_assert( std::is_constructible_v<std::decay_t<T>, Args..., Alloc const&>,
                         "std::uses_allocator_v<T, Alloc> contract violated" );
          return std::decay_t<T>{std::forward<Args>( args )..., old.get_allocator()};
        }
      }
    }
    // Couldn't get a valid allocator, proceed without one
    return std::decay_t<T>{std::forward<Args>( args )...};
  }
} // namespace LHCb
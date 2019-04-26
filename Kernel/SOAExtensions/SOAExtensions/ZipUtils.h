/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

/** @file ZipUtils.h
 *
 * This file contains helper classes and functions for runtime validation of
 * zips.  Their usage is expected to come with a small runtime performance
 * penalty and expected usage is in debug builds only. The errors that can be
 * detected are likely configuration mistakes that cannot be seen by the
 * compiler in most cases.
 *
 * @author Paul Seyfert Paul.Seyfert@cern.ch
 * @date   2019-02-22
 */
#ifndef ZipUtils_H
#define ZipUtils_H 1
#include "SOAContainer/SOAUtils.h"
#include "ZipTraits.h" // IWYU pragma: keep
#include <atomic>
#include <cstdint>
#include <type_traits>

/** @namespace Zipping
 */
namespace Zipping {

  /** @class ZipFamilyNumber ZipUtils.h
   *
   * @brief Provides an identifier for zip'able container "families".
   *
   * At the moment done as uint64_t (could be done as TES location of the first ancestor's TES location or other).
   * All functionalities of uint64_t are disabled except for comparison.
   */
  class ZipFamilyNumber final {
    uint64_t m_value; ///< underlying data

  public:
    /// constructor from underlying type, marked explicit to prevent *accidential* conversion from storage type.
    explicit constexpr ZipFamilyNumber( uint64_t t ) noexcept : m_value( t ) {}

    /// conversion to underlying type, marked explicit to prevent *accidential* conversion to storage type.
    explicit constexpr operator uint64_t() const noexcept { return m_value; }

    /// comparison operator
    friend constexpr bool operator==( ZipFamilyNumber lhs, ZipFamilyNumber rhs ) noexcept {
      return lhs.m_value == rhs.m_value;
    }
    /// comparison operator
    friend constexpr bool operator!=( ZipFamilyNumber lhs, ZipFamilyNumber rhs ) noexcept {
      return lhs.m_value != rhs.m_value;
    }
  };

  namespace details {
    /** @class ZipFamilyNumberGenerator ZipUtils.h
     *
     * @brief Generator for "unique" family numbers across all threads.
     *
     * Uniqueness is only needed within each event. Generation disabled in optimized builds.
     */
    class ZipFamilyNumberGenerator final {
      inline static std::atomic_uint64_t s_generator; ///< static tracker of used ZipFamilyNumbers

    public:
#ifndef NDEBUG
      /// generate a so-far unused ZipFamilyNumber
      [[nodiscard]] static ZipFamilyNumber generate() { return ZipFamilyNumber( s_generator.fetch_add( 1 ) ); }
#else
      /// generate a fake ZipFamilyNumber for optimized builds
      [[nodiscard]] constexpr static ZipFamilyNumber generate() noexcept { return ZipFamilyNumber( 0 ); }
#endif
    };
  } // namespace details

  /**
   * @brief Helper function to obtain the family number from a set of (assumed semantically compatible) zip'able
   * containers.
   *
   * @tparam FIRST type of the first argument, must have a 'ZipFamilyNumber zipIdentifier()' member function
   * @tparam OTHERS types of the remaining, unused arguments
   * @param first an object with a ZipFamilyNumber (ZipContainer or ZipSelection)
   * @param
   *
   * @return the family number of the container family, implementation defined behaviour in case of incompatible
   * containers.
   */
  template <typename FIRST, typename... OTHERS>
  ZipFamilyNumber familyNumber( FIRST& first, OTHERS&... /*unused*/ ) {
    return first.zipIdentifier();
  }

  /**
   * @brief Helper function for runtime validation of zip behaviour
   *
   * Validates if all arguments are from the same family of zip'able containers. Has the same behaviour in optimized and
   * debug build, but the arguments might be constructed such that the optimized build will always true.
   *
   * @tparam FIRST Zipping::ZipContainer<...> type
   * @tparam OTHERS Zipping::ZipContainer<...> types
   * @param first   first Zipping::ZipContainer<...> that should be zipped
   * @param others  remaining Zipping::ZipContainer<...>s that should be zipped
   *
   * @return true if all arguments belong to the same container family
   */
  template <typename FIRST, typename... OTHERS,
            typename = std::enable_if_t<std::conjunction_v<has_semantic_zip<FIRST>, has_semantic_zip<OTHERS>...>>>
  bool areSemanticallyCompatible( FIRST& first, OTHERS&... others ) {
    return ( ... && ( others.zipIdentifier() == first.zipIdentifier() ) );
  }

} // namespace Zipping
#endif

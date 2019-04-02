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
  struct ZipFamilyNumber final {
  private:
    uint64_t m_value; ///< underlying data

  public:
    /// default constructor is disabled to prevent uninitialized creation
    ZipFamilyNumber() = delete;

    /// constructor from underlying type, marked explicit to prevent *accidential* conversion from storage type.
    explicit ZipFamilyNumber( uint64_t t ) : m_value( t ) {}

    /// conversion to underlying type, marked explicit to prevent *accidential* conversion to storage type.
    explicit operator uint64_t() const { return m_value; }

    /// comparison operator
    bool operator==( const ZipFamilyNumber other ) const { return m_value == other.m_value; }
    /// comparison operator
    bool operator!=( const ZipFamilyNumber other ) const { return m_value != other.m_value; }
  };

  namespace details {
    /** @class ZipFamilyNumberGenerator ZipUtils.h
     *
     * @brief Generator for "unique" family numbers across all threads.
     *
     * Uniqueness is only needed within each event. Generation disabled in optimized builds.
     */
    class ZipFamilyNumberGenerator final {
    private:
      inline static std::atomic_uint64_t s_generator; ///< static tracker of used ZipFamilyNumbers

    public:
#ifndef DNDEBUG
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
   * @tparam SECOND Zipping::ZipContainer<...> type
   * @tparam OTHERS Zipping::ZipContainer<...> types
   * @param first   first Zipping::ZipContainer<...> that should be zipped
   * @param second  second Zipping::ZipContainer<...> that should be zipped
   * @param others  remaining Zipping::ZipContainer<...>s that should be zipped
   *
   * @return true if all arguments belong to the same container family
   */
  template <typename FIRST, typename SECOND, typename... OTHERS,
            typename = typename std::enable_if_t<SOA::Utils::ALL( has_semantic_zip_v<FIRST>, has_semantic_zip_v<SECOND>,
                                                                  has_semantic_zip_v<OTHERS>... )>>
  bool areSemanticallyCompatible( FIRST& first, SECOND& second, OTHERS&... others ) {
    return ( first.zipIdentifier() == second.zipIdentifier() ) && ( areSemanticallyCompatible( first, others... ) );
  }

  /** @brief Template specialization as recursion starting point areSemanticallyCompatible
   *
   * @param first: ZipContainer<...> that is intended to be zipped
   * @returns true
   */
  template <typename FIRST, typename = typename std::enable_if_t<has_semantic_zip_v<FIRST>>>
  bool areSemanticallyCompatible( FIRST& /*unused*/ ) {
    return true;
  }
} // namespace Zipping
#endif

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

#ifndef ZipUtils_H
#define ZipUtils_H 1
#include "SOAContainer/SOAUtils.h"
#include "ZipTraits.h" // IWYU pragma: keep
#include <atomic>
#include <type_traits>

// This file contains helper classes and functions for runtime validation of
// zips.  Their usage is expected to come with a small runtime performance
// penalty and expected usage is in debug builds only. The errors that can be
// detected are likely configuration mistakes that cannot be seen by the
// compiler in most cases.

namespace Zipping {
  /** @class ZipFamilyNumber ZipUtils.h
   *
   * Provides an identifier for zip'able container "families". At the moment done as uint64_t (could be done as TES
   * location of the first ancestor's TES location or other).
   */
  struct ZipFamilyNumber final {
  private:
    uint64_t m_value;

  public:
    // prevent uninitialized creation (as data member?)
    ZipFamilyNumber() = delete;

    // disable accidential conversion from storage type to ZipFamilyNumber
    explicit ZipFamilyNumber( uint64_t t ) : m_value( t ) {}

    // assuming most of what can be done with uint64_t doesn't make sense as a
    // family identifier I disable implicit conversion.
    explicit operator uint64_t() const { return m_value; }

    // The only thing ZipFamilyNumbers need to provide is equality comparison. No meaning is attributed to greater or
    // smaller.
    bool operator==( const ZipFamilyNumber other ) { return m_value == other.m_value; }
    bool operator!=( const ZipFamilyNumber other ) { return m_value != other.m_value; }
  };

  namespace details {
    /** @class ZipFamilyNumberGenerator ZipUtils.h
     *
     * Generator for "unique" family numbers across all threads.
     * Uniqueness is only needed within each event.
     */
    class ZipFamilyNumberGenerator final {
    private:
      inline static std::atomic_uint64_t s_generator;

    public:
      static ZipFamilyNumber generate() { return ZipFamilyNumber( s_generator.fetch_add( 1 ) ); }
    };
  } // namespace details

  /** Helper function to obtain the family number from a set of (assumed semantically compatible) zip'able containers.
   *
   * @return the family number of the container family, implementation defined behaviour in case of incompatible
   * containers.
   */
  template <typename FIRST, typename... OTHERS>
  ZipFamilyNumber familyNumber( FIRST& first, OTHERS&... /*unused*/ ) {
    return first.zipIdentifier();
  }

  /** Helper function for runtime validation of zip behaviour
   *
   * Returns if all arguments are from the same family of zip'able containers.
   * @param first, second, others: ZipContainer<...> that are intended to be zipped
   */
  template <typename FIRST, typename SECOND, typename... OTHERS,
            typename = typename std::enable_if_t<SOA::Utils::ALL( has_semantic_zip_v<FIRST>, has_semantic_zip_v<SECOND>,
                                                                  has_semantic_zip_v<OTHERS>... )>>
  bool areSemanticallyCompatible( FIRST& first, SECOND& second, OTHERS&... others ) {
    return ( first.zipIdentifier() == second.zipIdentifier() ) && ( areSemanticallyCompatible( first, others... ) );
  }

  /** Template specialization as recursion starting point for the previous helper or validator for trivial (trivial as
   * in "there is no work to be done") zip.
   *
   * Returns true
   * @param first: ZipContainer<...> that is intended to be zipped
   */
  template <typename FIRST, typename = typename std::enable_if_t<has_semantic_zip_v<FIRST>>>
  bool areSemanticallyCompatible( FIRST& /*unused*/ ) {
    return true;
  }
} // namespace Zipping
#endif

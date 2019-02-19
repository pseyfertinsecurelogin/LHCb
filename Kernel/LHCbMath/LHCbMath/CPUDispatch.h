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

#pragma once

// STL
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <string>

// Gaudi
#include "GaudiKernel/GaudiException.h"

// Vector Class
#include "instrset.h"

namespace LHCb {

  /// Namespace for CPU dispatch helper methods
  namespace CPU {

    /// Enum for CPU capabilities
    enum ID {
      GENERIC    = 0,
      SSE3       = 3,
      SSE4       = 6,
      AVX        = 7,
      AVX2       = 8,
      AVX512     = 9,
      AVX512VL   = 10,
      AVX512BWDQ = 11,
      UNKNOWN    = 999999
    };

    /** @brief Method to set a dispatch function from a list of options based on CPU ID.
     *
     *  Expects to be passed an initializer list of the form of list of std::pairs where the
     *  first element is the CPU ID enum and the second element is the dispatch function
     *  to use for that instruction set.
     *
     *  Entries in the initializer list must be sorted in order of decreasing CPU ID
     *  (i.e. fastest first). A check is applied to enforce this.
     *
     *  Specific SIMD instuction sets can be disabled by simply defining certain environment
     *  variables. For instance, to disable AVX2 implementations define 'LHCBMATH_DISABLE_AVX2'.
     *
     *  @return The selected dispatch function.
     */
    template <typename Vtbl>
    decltype( auto ) dispatch( const Vtbl& vtbl ) {
      // make sure list is sorted in strict decreasing CPU ID
      // needed for the following search to find the right implementation
      {
        ID lastID = UNKNOWN;
        for ( const auto& i : vtbl ) {
          if ( i.first >= lastID ) {
            throw GaudiException( "Dispatch table must be strictly in decreasing ID order", "LHCb::CPU::dispatch",
                                  StatusCode::FAILURE );
          }
          lastID = i.first;
        }
      }

      // Get supported instruction set
      const auto level = instrset_detect();

      // find pointer to the appropriate version
      const auto impl = std::find_if( std::begin( vtbl ), std::end( vtbl ), [&level]( const auto& j ) {
        return ( ( AVX512BWDQ == j.first && getenv( "LHCBMATH_DISABLE_AVX512" ) ) ||
                         ( AVX512VL == j.first && getenv( "LHCBMATH_DISABLE_AVX512" ) ) ||
                         ( AVX512 == j.first && getenv( "LHCBMATH_DISABLE_AVX512" ) ) ||
                         ( AVX2 == j.first && getenv( "LHCBMATH_DISABLE_AVX2" ) ) ||
                         ( AVX == j.first && getenv( "LHCBMATH_DISABLE_AVX" ) ) ||
                         ( SSE4 == j.first && getenv( "LHCBMATH_DISABLE_SSE4" ) ) ||
                         ( SSE3 == j.first && getenv( "LHCBMATH_DISABLE_SSE3" ) )
                     ? false
                     : level >= j.first );
      } );
      if ( impl == std::end( vtbl ) ) {
        throw GaudiException( "No implementation for instruction set level " + std::to_string( level ),
                              "LHCb::CPU::dispatch", StatusCode::FAILURE );
      }

      // return the impl
      return impl->second;
    }

  } // namespace CPU
} // namespace LHCb

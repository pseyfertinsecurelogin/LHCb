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
#include "GaudiKernel/System.h"

// Define flags for each SIMD level
#if defined( __AVX512BW__ ) || defined( __AVX512DQ__ )
#  define AVX512BWDQ_ENABLED true
#else
#  define AVX512BWDQ_ENABLED false
#endif
#if defined( __AVX512VL__ )
#  define AVX512VL_ENABLED true
#else
#  define AVX512VL_ENABLED false
#endif
#if defined( __AVX512F__ )
#  define AVX512F_ENABLED true
#else
#  define AVX512F_ENABLED false
#endif
#if defined( __AVX2__ )
#  define AVX2_ENABLED true
#else
#  define AVX2_ENABLED false
#endif
#if defined( __AVX__ )
#  define AVX_ENABLED true
#else
#  define AVX_ENABLED false
#endif
#if defined( __SSE4_2__ )
#  define SSE4_2_ENABLED true
#else
#  define SSE4_2_ENABLED false
#endif
#if defined( __SSE3__ )
#  define SSE3_ENABLED true
#else
#  define SSE3_ENABLED false
#endif

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

    namespace {
      // Define the SIMD allowed, based on compilation level and runtime flags
      inline const bool allow_avx512bwdq = AVX512BWDQ_ENABLED && !getenv( "LHCBMATH_DISABLE_AVX512" );
      inline const bool allow_avx512vl   = AVX512VL_ENABLED && !getenv( "LHCBMATH_DISABLE_AVX512" );
      inline const bool allow_avx512f    = AVX512F_ENABLED && !getenv( "LHCBMATH_DISABLE_AVX512" );
      inline const bool allow_avx2       = AVX2_ENABLED && !getenv( "LHCBMATH_DISABLE_AVX2" );
      inline const bool allow_avx        = AVX_ENABLED && !getenv( "LHCBMATH_DISABLE_AVX" );
      inline const bool allow_sse4       = SSE4_2_ENABLED && !getenv( "LHCBMATH_DISABLE_SSE4" );
      inline const bool allow_sse3       = SSE3_ENABLED && !getenv( "LHCBMATH_DISABLE_SSE3" );
    } // namespace

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
          if ( UNLIKELY( i.first >= lastID ) ) {
            throw GaudiException( "Dispatch table must be strictly in decreasing ID order", "LHCb::CPU::dispatch",
                                  StatusCode::FAILURE );
          }
          lastID = i.first;
        }
      }

      // Get supported instruction set
      const auto level = System::instructionsetLevel();

      // find pointer to the appropriate version
      const auto impl = std::find_if( std::begin( vtbl ), std::end( vtbl ), [&level]( const auto& j ) {
        // check the runtime flags against the implementation
        return ( ( ( AVX512BWDQ == j.first && !allow_avx512bwdq ) || //
                   ( AVX512VL == j.first && !allow_avx512vl ) ||     //
                   ( AVX512 == j.first && !allow_avx512f ) ||        //
                   ( AVX2 == j.first && !allow_avx2 ) ||             //
                   ( AVX == j.first && !allow_avx ) ||               //
                   ( SSE4 == j.first && !allow_sse4 ) ||             //
                   ( SSE3 == j.first && !allow_sse3 ) )
                     ? false
                     : level >= j.first );
      } );

      // check we found something to use
      if ( UNLIKELY( impl == std::end( vtbl ) ) ) {
        throw GaudiException( "No implementation for instruction set level " + std::to_string( level ),
                              "LHCb::CPU::dispatch", StatusCode::FAILURE );
      }

      // return the impl
      return impl->second;
    }

  } // namespace CPU
} // namespace LHCb

#undef AVX512BWDQ_ENABLED
#undef AVX512VL_ENABLED
#undef AVX512F_ENABLED
#undef AVX2_ENABLED
#undef AVX_ENABLED
#undef SSE4_2_ENABLED
#undef SSE3_ENABLED

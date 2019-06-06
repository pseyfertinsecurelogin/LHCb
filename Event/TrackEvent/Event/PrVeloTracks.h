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

#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"

#include "PrVeloHits.h"

/**
 * Track data for exchanges between VeloTracking and UT
 *
 * @author: Arthur Hennequin
 */

namespace LHCb::Pr::Velo {
  class Tracks {
    constexpr static int max_tracks = align_size( 1024 );
    constexpr static int max_hits   = 26;
    constexpr static int max_states = 2;

  public:
    Tracks() {
      const size_t size = max_tracks * ( max_hits + max_states * 8 + 1 );
      m_data            = static_cast<int*>( std::aligned_alloc( 64, size * sizeof( int ) ) );
    }

    Tracks( const Tracks& ) = delete;

    Tracks( Tracks&& other ) {
      m_data       = other.m_data;
      other.m_data = nullptr;
      m_size       = other.m_size;
    }

    inline int  size() const { return m_size; }
    inline int& size() { return m_size; }

    SOA_ACCESSOR( nHits, m_data )
    SOA_ACCESSOR_VAR( hit, &m_data[( hit + 1 ) * max_tracks], int hit )

    VEC3_SOA_ACCESSOR_VAR( statePos, (float*)&m_data[( max_hits + 1 + state * 8 ) * max_tracks],
                           (float*)&m_data[( max_hits + 1 + state * 8 + 1 ) * max_tracks],
                           (float*)&m_data[( max_hits + 1 + state * 8 + 2 ) * max_tracks], int state )

    VEC3_XY_SOA_ACCESSOR_VAR( stateDir, (float*)&m_data[( max_hits + 1 + state * 8 + 3 ) * max_tracks],
                              (float*)&m_data[( max_hits + 1 + state * 8 + 4 ) * max_tracks], 1.f, int state )

    VEC3_SOA_ACCESSOR_VAR( stateCov, (float*)&m_data[( max_hits + 1 + state * 8 + 5 ) * max_tracks],
                           (float*)&m_data[( max_hits + 1 + state * 8 + 6 ) * max_tracks],
                           (float*)&m_data[( max_hits + 1 + state * 8 + 7 ) * max_tracks], int state )

    /// Retrieve the pseudorapidity at the first state
    template <typename T>
    inline T pseudoRapidity( int t ) const {
      return stateDir<T>( t, 0 ).eta();
    }

    /// Retrieve the phi at the first state
    template <typename T>
    inline T phi( int t ) const {
      return stateDir<T>( t, 0 ).phi();
    }

    template <typename simd, typename maskT>
    inline void copy_back( const Tracks& from, int at, maskT mask ) {
      using intT = typename simd::int_v;
      for ( int i = 0; i < max_hits + max_states * 8 + 1; i++ ) {
        intT( &from.m_data[i * max_tracks + at] ).compressstore( mask, &m_data[i * max_tracks + m_size] );
      }
      m_size += simd::popcount( mask );
    }

    ~Tracks() { std::free( m_data ); }

  private:
    alignas( 64 ) int* m_data;
    int m_size = 0;
  };
} // namespace LHCb::Pr::Velo

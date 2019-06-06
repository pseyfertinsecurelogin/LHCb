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

/**
 * Track data for exchanges between UT and FT
 *
 * @author: Arthur Hennequin
 *
 * Based on https://gitlab.cern.ch/lhcb/Rec/blob/master/Pr/PrKernel/PrKernel/PrVeloUTTrack.h
 * from Michel De Cian
 */

namespace LHCb::Pr::Upstream {
  class Tracks {
    constexpr static int max_tracks = align_size( 1024 );
    constexpr static int max_hits   = 30;

  public:
    Tracks() {
      const size_t size = max_tracks * ( max_hits + 8 );
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

    // Index in TracksVP container of the track's ancestor
    SOA_ACCESSOR( trackVP, m_data )

    // Refined Velo state
    VEC3_SOA_ACCESSOR( statePos, (float*)&m_data[1 * max_tracks], (float*)&m_data[2 * max_tracks],
                       (float*)&m_data[3 * max_tracks] )

    VEC3_XY_SOA_ACCESSOR( stateDir, (float*)&m_data[4 * max_tracks], (float*)&m_data[5 * max_tracks], 1.f )

    // QoP estimate from UT
    SOA_ACCESSOR( stateQoP, (float*)&m_data[6 * max_tracks] )

    // Hits (for now LHCBid) in UT
    // TODO: replace LHCbids by index in UT hit container
    SOA_ACCESSOR( nHits, &m_data[7 * max_tracks] )
    SOA_ACCESSOR_VAR( hit, &m_data[( hit + 8 ) * max_tracks], int hit )

    /// Retrieve the momentum
    template <typename T>
    inline T p( int t ) const {
      T qop = stateQoP<T>( t );
      return 1 / qop;
    }

    /// Retrieve the transverse momentum
    template <typename T>
    inline T pt( int t ) const {
      Vec3<T> dir  = stateDir<T>( t );
      T       qop  = stateQoP<T>( t );
      T       txy2 = dir.x * dir.x + dir.y * dir.y;
      return sqrt( txy2 / ( 1 + txy2 ) ) / abs( qop );
    }

    /// Retrieve the pseudorapidity
    template <typename T>
    inline T pseudoRapidity( int t ) const {
      return stateDir<T>( t ).eta();
    }

    /// Retrieve the phi
    template <typename T>
    inline T phi( int t ) const {
      return stateDir<T>( t ).phi();
    }

    template <typename simd, typename maskT>
    inline void copy_back( const Tracks& from, int at, maskT mask ) {
      using intT = typename simd::int_v;
      for ( int i = 0; i < max_hits + 8; i++ ) {
        intT( &from.m_data[i * max_tracks + at] ).compressstore( mask, &m_data[i * max_tracks + m_size] );
      }
      m_size += simd::popcount( mask );
    }

    ~Tracks() { std::free( m_data ); }

  private:
    alignas( 64 ) int* m_data;
    int m_size = 0;
  };
} // namespace LHCb::Pr::Upstream

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
 * Track data after the Kalman fit
 *
 * @author: Arthur Hennequin
 */

namespace LHCb::Pr::Fitted::Forward {
  class Tracks {
    constexpr static int max_tracks = align_size( 1024 );

  public:
    Tracks() {
      const size_t size = max_tracks * 15;
      m_data            = static_cast<data_t*>( std::aligned_alloc( 64, size * sizeof( int ) ) );
    }

    Tracks( const Tracks& ) = delete;

    Tracks( Tracks&& other ) {
      m_data       = other.m_data;
      other.m_data = nullptr;
      m_size       = other.m_size;
    }

    bool        empty() const { return m_size == 0; }
    inline int  size() const { return m_size; }
    inline int& size() { return m_size; }

    // Index in TracksFT container of the track's ancestor
    SOA_ACCESSOR( trackFT, &( m_data->i ) )

    // QoP estimate from FT
    SOA_ACCESSOR( QoP, &( m_data[max_tracks].f ) )

    // Closest to beam state after after fit
    VEC3_SOA_ACCESSOR( beamStatePos, &( m_data[2 * max_tracks].f ), &( m_data[3 * max_tracks].f ),
                       &( m_data[4 * max_tracks].f ) )

    VEC3_XY_SOA_ACCESSOR( beamStateDir, &( m_data[5 * max_tracks].f ), &( m_data[6 * max_tracks].f ), 1.f )

    // Chi2 from fit
    SOA_ACCESSOR( chi2, &( m_data[7 * max_tracks].f ) )

    // Chi2 degrees of freedom
    SOA_ACCESSOR( chi2nDof, &( m_data[8 * max_tracks].i ) )

    // Covariances matrix for X and Y (can be merged if wx=wy)
    VEC3_SOA_ACCESSOR( covX, &( m_data[9 * max_tracks].f ), &( m_data[10 * max_tracks].f ),
                       &( m_data[11 * max_tracks].f ) )

    VEC3_SOA_ACCESSOR( covY, &( m_data[12 * max_tracks].f ), &( m_data[13 * max_tracks].f ),
                       &( m_data[14 * max_tracks].f ) )

    /// Retrieve the momentum
    template <typename T>
    inline T p( int t ) const {
      T qop = QoP<T>( t );
      return abs( 1 / qop );
    }

    template <typename simd, typename maskT>
    inline void copy_back( const Tracks& from, int at, maskT mask ) {
      using intT = typename simd::int_v;
      for ( int i = 0; i < 15; i++ ) {
        intT( &( from.m_data[i * max_tracks + at].i ) ).compressstore( mask, &( m_data[i * max_tracks + m_size].i ) );
      }
      m_size += simd::popcount( mask );
    }

    ~Tracks() { std::free( m_data ); }

  private:
    using data_t = union {
      float f;
      int   i;
    };
    alignas( 64 ) data_t* m_data;
    int m_size = 0;
  };
} // namespace LHCb::Pr::Fitted::Forward

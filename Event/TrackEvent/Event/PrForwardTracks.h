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
#include "Event/PrUpstreamTracks.h"
#include "Event/PrVeloTracks.h"

#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"
#include <iostream>
/**
 * Track data for exchanges between FT and Fit
 *
 * @author: Arthur Hennequin
 */

namespace LHCb::Pr::Forward {
  class Tracks {
    constexpr static int max_tracks = align_size( 1024 );
    constexpr static int max_hits   = 40;

  public:
    Tracks( Velo::Tracks const* velo_ancestors, Upstream::Tracks const* upstream_ancestors ) {
      const size_t size    = max_tracks * ( max_hits + 9 );
      m_data               = static_cast<data_t*>( std::aligned_alloc( 64, size * sizeof( int ) ) );
      m_velo_ancestors     = velo_ancestors;
      m_upstream_ancestors = upstream_ancestors;
    }

    Tracks( const Tracks& ) = delete;

    Tracks( Tracks&& other )
        : m_data{std::exchange( other.m_data, nullptr )}
        , m_size{other.m_size}
        , m_velo_ancestors{other.m_velo_ancestors}
        , m_upstream_ancestors{other.m_upstream_ancestors} {}

    inline int  size() const { return m_size; }
    inline int& size() { return m_size; }

    // Return pointer to ancestor container
    Velo::Tracks const*     getVeloAncestors() const { return m_velo_ancestors; };
    Upstream::Tracks const* getUpstreamAncestors() const { return m_upstream_ancestors; };

    // Index in TracksVP container of the track's ancestor
    SOA_ACCESSOR( trackVP, &( m_data->i ) )

    // Index in TracksUT container of the track's ancestor
    SOA_ACCESSOR( trackUT, &( m_data[max_tracks].i ) )

    // QoP estimate from FT
    SOA_ACCESSOR( stateQoP, &( m_data[2 * max_tracks].f ) )

    // Hits (for now LHCBid) in FT (or UT)
    // TODO: replace LHCbids by index in FT hit container
    SOA_ACCESSOR( nHits, &( m_data[3 * max_tracks].i ) )
    SOA_ACCESSOR_VAR( hit, &( m_data[( hit + 4 ) * max_tracks].i ), int hit )

    VEC3_SOA_ACCESSOR( statePos, &( m_data[( max_hits + 4 ) * max_tracks].f ),
                       &( m_data[( max_hits + 4 + 1 ) * max_tracks].f ),
                       &( m_data[( max_hits + 4 + 2 ) * max_tracks].f ) )

    VEC3_XY_SOA_ACCESSOR( stateDir, &( m_data[( max_hits + 4 + 3 ) * max_tracks].f ),
                          &( m_data[( max_hits + 4 + 4 ) * max_tracks].f ), 1.f )

    /// Retrieve the momentum
    template <typename T>
    inline T p( int t ) const {
      T qop = stateQoP<T>( t );
      return 1 / qop;
    }

    template <typename simd, typename maskT>
    inline void copy_back( const Tracks& from, int at, maskT mask ) {
      using intT = typename simd::int_v;
      for ( int i = 0; i < ( max_hits + 9 ); i++ ) {
        intT( &( from.m_data[i * max_tracks + at].i ) ).compressstore( mask, &( m_data[i * max_tracks + m_size].i ) );
      }
      m_size += simd::popcount( mask );
    }

    // These can be LHCbIDs from FT and UT. The latter only if the Forward was run with Velo tracks as input.
    // And UT hits were added during the Forward tracking.
    std::vector<LHCb::LHCbID> lhcbIDsFromForward( int t ) const {
      int                       n_hits = nHits<SIMDWrapper::scalar::types::int_v>( t ).cast();
      std::vector<LHCb::LHCbID> ids;
      ids.reserve( n_hits );
      for ( int i = 0; i < n_hits; i++ ) {
        int lhcbid = hit<SIMDWrapper::scalar::types::int_v>( t, i ).cast();
        ids.emplace_back( lhcbid );
      }
      return ids;
    }

    ~Tracks() { std::free( m_data ); }

  private:
    using data_t = union {
      float f;
      int   i;
    };
    alignas( 64 ) data_t* m_data;
    int                     m_size               = 0;
    Velo::Tracks const*     m_velo_ancestors     = nullptr;
    Upstream::Tracks const* m_upstream_ancestors = nullptr;
  };
} // namespace LHCb::Pr::Forward

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
#include "Event/PrVeloTracks.h"
#include "Kernel/EventLocalAllocator.h"
#include "Kernel/LHCbID.h"
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
    constexpr static int         max_tracks = align_size( 1024 );
    constexpr static int         max_hits   = 30;
    constexpr static int         row_size   = max_hits + 11;
    constexpr static std::size_t array_size = max_tracks * row_size;
    using data_t                            = union {
      float f;
      int   i;
    };

  public:
    using allocator_type = LHCb::Allocators::EventLocal<data_t>;
    Tracks( Velo::Tracks const* velo_ancestors, allocator_type alloc = {} )
        : m_alloc{std::move( alloc )}
        , m_data{std::allocator_traits<allocator_type>::allocate( m_alloc, array_size )}
        , m_velo_ancestors{velo_ancestors} {}

    Tracks( const Tracks& ) = delete;

    Tracks( Tracks&& other )
        : m_alloc{std::move( other.m_alloc )}
        , m_data{std::exchange( other.m_data, nullptr )}
        , m_size{other.m_size}
        , m_velo_ancestors{other.m_velo_ancestors} {}

    [[nodiscard]] allocator_type get_allocator() const noexcept { return m_alloc; }
    [[nodiscard]] int            size() const { return m_size; }
    int&                         size() { return m_size; }

    // Return pointer to ancestor container
    [[nodiscard]] Velo::Tracks const* getVeloAncestors() const { return m_velo_ancestors; };

    // Index in TracksVP container of the track's ancestor
    SOA_ACCESSOR( trackVP, &m_data->i )

    // Refined Velo state
    VEC3_SOA_ACCESSOR( statePos, &( m_data[1 * max_tracks].f ), &( m_data[2 * max_tracks].f ),
                       &( m_data[3 * max_tracks].f ) )

    VEC3_XY_SOA_ACCESSOR( stateDir, &( m_data[4 * max_tracks].f ), &( m_data[5 * max_tracks].f ), 1.f )

    VEC3_SOA_ACCESSOR( stateCov, &( m_data[6 * max_tracks].f ), &( m_data[7 * max_tracks].f ),
                       &( m_data[8 * max_tracks].f ) )

    // QoP estimate from UT
    SOA_ACCESSOR( stateQoP, &( m_data[9 * max_tracks].f ) )

    // Hits (for now LHCBid) in UT
    // TODO: replace LHCbids by index in UT hit container
    SOA_ACCESSOR( nHits, &( m_data[10 * max_tracks].i ) )
    SOA_ACCESSOR_VAR( hit, &( m_data[( hit + 11 ) * max_tracks].i ), int hit )

    /// Retrieve the momentum
    template <typename T>
    T p( int t ) const {
      T qop = stateQoP<T>( t );
      return 1 / qop;
    }

    /// Retrieve the transverse momentum
    template <typename T>
    T pt( int t ) const {
      Vec3<T> dir  = stateDir<T>( t );
      T       qop  = stateQoP<T>( t );
      T       txy2 = dir.x * dir.x + dir.y * dir.y;
      return sqrt( txy2 / ( 1 + txy2 ) ) / abs( qop );
    }

    /// Retrieve the pseudorapidity
    template <typename T>
    T pseudoRapidity( int t ) const {
      return stateDir<T>( t ).eta();
    }

    /// Retrieve the phi
    template <typename T>
    T phi( int t ) const {
      return stateDir<T>( t ).phi();
    }

    template <typename simd, typename maskT>
    void copy_back( const Tracks& from, int at, maskT mask ) {
      using intT = typename simd::int_v;
      for ( int i = 0; i < max_hits + 11; i++ ) {
        intT( &( from.m_data[i * max_tracks + at].i ) ).compressstore( mask, &( m_data[i * max_tracks + m_size].i ) );
      }
      m_size += simd::popcount( mask );
    }

    [[nodiscard]] std::vector<LHCb::LHCbID> lhcbIDsFromUT( int t ) const {
      int                       n_hits = nHits<SIMDWrapper::scalar::types::int_v>( t ).cast();
      std::vector<LHCb::LHCbID> ids;
      ids.reserve( n_hits );
      for ( int i = 0; i < n_hits; i++ ) {
        int lhcbid = hit<SIMDWrapper::scalar::types::int_v>( t, i ).cast();
        ids.emplace_back( lhcbid );
      }
      return ids;
    }

    ~Tracks() { std::allocator_traits<allocator_type>::deallocate( m_alloc, m_data, array_size ); }

  private:
    allocator_type      m_alloc;
    data_t*             m_data{nullptr};
    int                 m_size{0};
    Velo::Tracks const* m_velo_ancestors{nullptr};
  };
} // namespace LHCb::Pr::Upstream

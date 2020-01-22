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
#include "Event/PrForwardTracks.h"
#include "Event/PrVeloHits.h"
#include "Kernel/EventLocalAllocator.h"
#include "Kernel/LHCbID.h"
#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"
#include "SOAExtensions/ZipUtils.h"

/**
 * Track data after the Kalman fit
 *
 * @author: Arthur Hennequin
 */

namespace LHCb::Pr::Fitted::Forward {
  class Tracks {
    constexpr static int         max_tracks = align_size( 1024 );
    constexpr static int         row_size   = 15;
    constexpr static std::size_t array_size = max_tracks * row_size;
    using data_t                            = union {
      float f;
      int   i;
    };

  public:
    using allocator_type = LHCb::Allocators::EventLocal<data_t>;
    Tracks( Pr::Forward::Tracks const* forward_ancestors,
            Zipping::ZipFamilyNumber zipIdentifier = Zipping::generateZipIdentifier(), allocator_type alloc = {} )
        : m_alloc{std::move( alloc )}
        , m_data{std::allocator_traits<allocator_type>::allocate( m_alloc, array_size )}
        , m_forward_ancestors{forward_ancestors}
        , m_zipIdentifier{zipIdentifier} {}

    // Special constructor for zipping machinery
    Tracks( Zipping::ZipFamilyNumber zipIdentifier, Tracks const& tracks )
        : Tracks( tracks.getForwardAncestors(), zipIdentifier, tracks.m_alloc ) {}

    Tracks( const Tracks& ) = delete;

    Tracks( Tracks&& other )
        : m_alloc{std::move( other.m_alloc )}
        , m_data{std::exchange( other.m_data, nullptr )}
        , m_size{other.m_size}
        , m_forward_ancestors{other.m_forward_ancestors}
        , m_zipIdentifier{other.m_zipIdentifier} {}

    [[nodiscard]] allocator_type             get_allocator() const noexcept { return m_alloc; }
    [[nodiscard]] bool                       empty() const noexcept { return m_size == 0; }
    [[nodiscard]] int                        size() const noexcept { return m_size; }
    int&                                     size() noexcept { return m_size; }
    [[nodiscard]] Zipping::ZipFamilyNumber   zipIdentifier() const noexcept { return m_zipIdentifier; }
    [[nodiscard]] Pr::Forward::Tracks const* getForwardAncestors() const noexcept { return m_forward_ancestors; };

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

    /** @brief Return a sorted vector of unique LHCbID objects on the t'th track.
     */
    template <typename I = SIMDWrapper::scalar::types::int_v>
    [[nodiscard]] std::vector<LHCbID> lhcbIDs( int t, Velo::Hits const& velo_hits ) const {
      auto const* forward_tracks = getForwardAncestors();
      if ( !forward_tracks ) { return std::vector<LHCbID>{}; }
      auto const* velo_tracks         = forward_tracks->getVeloAncestors();
      auto const* upstream_tracks     = forward_tracks->getUpstreamAncestors();
      auto        forward_track_index = trackFT<I>( t ).cast();
      auto upstream_track_index       = upstream_tracks ? forward_tracks->trackUT<I>( forward_track_index ).cast() : -1;
      auto velo_track_index           = forward_tracks->trackVP<I>( forward_track_index ).cast();

      // Get LHCb IDs
      auto velo_lhcbids =
          velo_tracks ? velo_tracks->lhcbIDs<Velo::Hits>( velo_track_index, velo_hits ) : std::vector<LHCbID>{};
      auto ut_lhcbids =
          upstream_tracks ? upstream_tracks->lhcbIDsFromUT( upstream_track_index ) : std::vector<LHCbID>{};
      auto                ft_lhcbids = forward_tracks->lhcbIDsFromForward( forward_track_index );
      std::vector<LHCbID> lhcbids;
      lhcbids.reserve( velo_lhcbids.size() + ut_lhcbids.size() + ft_lhcbids.size() );
      lhcbids.insert( end( lhcbids ), begin( velo_lhcbids ), end( velo_lhcbids ) );
      lhcbids.insert( end( lhcbids ), begin( ft_lhcbids ), end( ft_lhcbids ) );
      // UT has higher IDs than FT
      lhcbids.insert( end( lhcbids ), begin( ut_lhcbids ), end( ut_lhcbids ) );

      // Sort and remove duplicates
      // This is a temporary workaround for duplicate LHCbIDs made by PrVeloUT,
      // see https://gitlab.cern.ch/lhcb/LHCb/merge_requests/2221 for details.
      std::sort( lhcbids.begin(), lhcbids.end() );
      lhcbids.erase( std::unique( lhcbids.begin(), lhcbids.end() ), lhcbids.end() );

      return lhcbids;
    }

    template <typename I, typename M>
    I nHits( int t, const M mask ) const {
      I           nHits   = 0;
      auto const* forward = getForwardAncestors();
      if ( forward ) {
        auto forward_indices = trackFT<I>( t );
        nHits                = nHits + forward->maskgather_nHits<I>( forward_indices, mask, 0 );
        auto const* velo     = forward->getVeloAncestors();
        if ( velo ) {
          auto velo_indices = forward->maskgather_trackVP<I>( forward_indices, mask, 0 );
          nHits             = nHits + velo->maskgather_nHits<I>( velo_indices, mask, 0 );
        }

        auto const* upstream = forward->getUpstreamAncestors();
        if ( upstream ) {
          auto upstream_indices = forward->maskgather_trackUT<I>( forward_indices, mask, 0 );
          nHits                 = nHits + upstream->maskgather_nHits<I>( upstream_indices, mask, 0 );
        }
      }
      return nHits;
    }

    ~Tracks() { std::allocator_traits<allocator_type>::deallocate( m_alloc, m_data, array_size ); }

  private:
    allocator_type             m_alloc;
    data_t*                    m_data;
    int                        m_size{0};
    Pr::Forward::Tracks const* m_forward_ancestors{nullptr};
    Zipping::ZipFamilyNumber   m_zipIdentifier;
  };
} // namespace LHCb::Pr::Fitted::Forward

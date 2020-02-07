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
#include "Kernel/EventLocalAllocator.h"
#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"
#include "SOAExtensions/ZipUtils.h"

/**
 * Track data for exchanges between FT and Fit
 *
 * @author: Arthur Hennequin
 */

namespace LHCb::Pr::Forward {
  class Tracks {

  public:
    constexpr static int         max_tracks = align_size( 1024 );
    constexpr static int         max_hits   = 40;
    constexpr static int         row_size   = max_hits + 9;
    constexpr static std::size_t array_size = max_tracks * row_size;
    using data_t                            = union {
      float f;
      int   i;
    };
    using allocator_type = LHCb::Allocators::EventLocal<data_t>;
    Tracks( Velo::Tracks const* velo_ancestors, Upstream::Tracks const* upstream_ancestors,
            Zipping::ZipFamilyNumber zipIdentifier = Zipping::generateZipIdentifier(), allocator_type alloc = {} )
        : m_alloc{std::move( alloc )}
        , m_data{std::allocator_traits<allocator_type>::allocate( m_alloc, array_size )}
        , m_velo_ancestors{velo_ancestors}
        , m_upstream_ancestors{upstream_ancestors}
        , m_zipIdentifier{zipIdentifier} {}

    // Special constructor for zipping machinery
    Tracks( Zipping::ZipFamilyNumber zipIdentifier, Tracks const& tracks )
        : Tracks( tracks.getVeloAncestors(), tracks.getUpstreamAncestors(), zipIdentifier, tracks.m_alloc ) {}

    Tracks( const Tracks& ) = delete;

    Tracks( Tracks&& other )
        : m_alloc{std::move( other.m_alloc )}
        , m_data{std::exchange( other.m_data, nullptr )}
        , m_size{other.m_size}
        , m_velo_ancestors{other.m_velo_ancestors}
        , m_upstream_ancestors{other.m_upstream_ancestors}
        , m_zipIdentifier{other.m_zipIdentifier} {}

    [[nodiscard]] allocator_type get_allocator() const noexcept { return m_alloc; }
    [[nodiscard]] int            size() const { return m_size; }
    int&                         size() { return m_size; }

    // Return pointer to ancestor container
    [[nodiscard]] Velo::Tracks const*     getVeloAncestors() const { return m_velo_ancestors; };
    [[nodiscard]] Upstream::Tracks const* getUpstreamAncestors() const { return m_upstream_ancestors; };
    /** Identifier showing which family of containers these columns can be zipped
     *  into.
     */
    [[nodiscard]] Zipping::ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

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
    T p( int t ) const {
      T qop = stateQoP<T>( t );
      return abs( 1 / qop );
    }

    template <typename simd, typename maskT>
    void copy_back( const Tracks& from, int at, maskT mask ) {
      using intT = typename simd::int_v;
      for ( int i = 0; i < ( max_hits + 9 ); i++ ) {
        intT( &( from.m_data[i * max_tracks + at].i ) ).compressstore( mask, &( m_data[i * max_tracks + m_size].i ) );
      }
      m_size += simd::popcount( mask );
    }

    // These can be LHCbIDs from FT and UT. The latter only if the Forward was run with Velo tracks as input.
    // And UT hits were added during the Forward tracking.
    [[nodiscard]] std::vector<LHCbID> lhcbIDsFromForward( int t ) const {
      int                 n_hits = nHits<SIMDWrapper::scalar::types::int_v>( t ).cast();
      std::vector<LHCbID> ids;
      ids.reserve( n_hits );
      for ( int i = 0; i < n_hits; i++ ) {
        int lhcbid = hit<SIMDWrapper::scalar::types::int_v>( t, i ).cast();
        ids.emplace_back( lhcbid );
      }
      return ids;
    }

    ~Tracks() { std::allocator_traits<allocator_type>::deallocate( m_alloc, m_data, array_size ); }

  private:
    allocator_type           m_alloc;
    data_t*                  m_data{nullptr};
    int                      m_size{0};
    Velo::Tracks const*      m_velo_ancestors{nullptr};
    Upstream::Tracks const*  m_upstream_ancestors{nullptr};
    Zipping::ZipFamilyNumber m_zipIdentifier;
  };
} // namespace LHCb::Pr::Forward

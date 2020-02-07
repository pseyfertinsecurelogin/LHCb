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
#include "Event/PrVeloHits.h"
#include "Kernel/EventLocalAllocator.h"
#include "Kernel/LHCbID.h"
#include "Kernel/VPChannelID.h"
#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"
#include "SOAExtensions/ZipUtils.h"

/**
 * Track data for exchanges between VeloTracking and UT
 *
 * @author: Arthur Hennequin
 */

namespace LHCb::Pr::Velo {
  class Tracks {
    constexpr static int         max_tracks       = align_size( 1024 );
    constexpr static int         max_hits         = 26;
    constexpr static int         max_states       = 2;
    constexpr static int         params_per_state = 11;
    constexpr static int         other_params     = 1;
    constexpr static int         row_size         = max_hits + max_states * params_per_state + other_params;
    constexpr static std::size_t array_size       = max_tracks * row_size;
    using data_t                                  = union {
      float f;
      int   i;
    };

  public:
    using allocator_type = LHCb::Allocators::EventLocal<data_t>;
    Tracks( Zipping::ZipFamilyNumber zipIdentifier = Zipping::generateZipIdentifier(), allocator_type alloc = {} )
        : m_alloc{std::move( alloc )}
        , m_data{std::allocator_traits<allocator_type>::allocate( m_alloc, array_size )}
        , m_zipIdentifier{zipIdentifier} {}

    Tracks( const Tracks& ) = delete;

    // Special constructor for zipping machinery
    Tracks( Zipping::ZipFamilyNumber zipIdentifier, Tracks const& old ) : Tracks( zipIdentifier, old.m_alloc ) {}

    Tracks( Tracks&& other )
        : m_alloc{std::move( other.m_alloc )}
        , m_data{std::exchange( other.m_data, nullptr )}
        , m_size{other.m_size}
        , m_zipIdentifier{other.m_zipIdentifier} {}

    [[nodiscard]] allocator_type           get_allocator() const noexcept { return m_alloc; }
    [[nodiscard]] int                      size() const { return m_size; }
    int&                                   size() { return m_size; }
    [[nodiscard]] bool                     empty() const { return m_size == 0; }
    [[nodiscard]] Zipping::ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

    SOA_ACCESSOR( nHits, &( m_data->i ) )
    SOA_ACCESSOR_VAR( hit, &( m_data[( hit + other_params ) * max_tracks].i ), int hit )

    VEC3_SOA_ACCESSOR_VAR( statePos, &( m_data[( max_hits + other_params + state * params_per_state ) * max_tracks].f ),
                           &( m_data[( max_hits + other_params + state * params_per_state + 1 ) * max_tracks].f ),
                           &( m_data[( max_hits + other_params + state * params_per_state + 2 ) * max_tracks].f ),
                           int state )

    VEC3_XY_SOA_ACCESSOR_VAR( stateDir,
                              &( m_data[( max_hits + other_params + state * params_per_state + 3 ) * max_tracks].f ),
                              &( m_data[( max_hits + other_params + state * params_per_state + 4 ) * max_tracks].f ),
                              1.f, int state )

    VEC3_SOA_ACCESSOR_VAR( stateCovX,
                           &( m_data[( max_hits + other_params + state * params_per_state + 5 ) * max_tracks].f ),
                           &( m_data[( max_hits + other_params + state * params_per_state + 6 ) * max_tracks].f ),
                           &( m_data[( max_hits + other_params + state * params_per_state + 7 ) * max_tracks].f ),
                           int state )

    VEC3_SOA_ACCESSOR_VAR( stateCovY,
                           &( m_data[( max_hits + other_params + state * params_per_state + 8 ) * max_tracks].f ),
                           &( m_data[( max_hits + other_params + state * params_per_state + 9 ) * max_tracks].f ),
                           &( m_data[( max_hits + other_params + state * params_per_state + 10 ) * max_tracks].f ),
                           int state )

    /// Retrieve the pseudorapidity at the first state
    template <typename T>
    T pseudoRapidity( int t ) const {
      return stateDir<T>( t, 0 ).eta();
    }

    /// Retrieve the phi at the first state
    template <typename T>
    T phi( int t ) const {
      return stateDir<T>( t, 0 ).phi();
    }

    template <typename simd, typename maskT>
    void copy_back( const Tracks& from, int at, maskT mask ) {
      using intT = typename simd::int_v;
      for ( int i = 0; i < row_size; i++ ) {
        intT( &( from.m_data[i * max_tracks + at].i ) ).compressstore( mask, &( m_data[i * max_tracks + m_size].i ) );
      }
      m_size += simd::popcount( mask );
    }

    /// Retrieve the (sorted) set of LHCbIDs
    // This could be simplified if we had only one type of Velo hits
    template <typename T>
    std::vector<LHCb::LHCbID> lhcbIDs( int t, T const& hits ) const {
      int n_hits = nHits<SIMDWrapper::scalar::types::int_v>( t ).cast();
      // tracks are created by large z to small z, lhcbID ordered with increasing z, reverse iteration on hit buffer.
      std::vector<LHCb::LHCbID> ids;
      ids.reserve( n_hits );
      for ( int i = n_hits - 1; i >= 0; i-- ) {
        int  hit_index = hit<SIMDWrapper::scalar::types::int_v>( t, i ).cast();
        auto lhcbid    = [&]() {
          if constexpr ( std::is_same_v<T, LHCb::Pr::Velo::Hits> ) {
            return LHCb::LHCbID(
                LHCb::VPChannelID( hits.template ChannelId<SIMDWrapper::scalar::types::int_v>( hit_index ).cast() ) );
          } else {
            return LHCb::LHCbID( hits[hit_index].channelID() );
          }
        }();
        ids.push_back( lhcbid );
      }
      return ids;
    }

    ~Tracks() { std::allocator_traits<allocator_type>::deallocate( m_alloc, m_data, array_size ); }

  private:
    allocator_type           m_alloc;
    data_t*                  m_data;
    int                      m_size{0};
    Zipping::ZipFamilyNumber m_zipIdentifier;
  };
} // namespace LHCb::Pr::Velo

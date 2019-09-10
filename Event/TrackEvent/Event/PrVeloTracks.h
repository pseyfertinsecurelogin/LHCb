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

#include "SOAExtensions/ZipUtils.h"

/**
 * Track data for exchanges between VeloTracking and UT
 *
 * @author: Arthur Hennequin
 */

namespace LHCb::Pr::Velo {
  class Tracks {
    constexpr static int     max_tracks       = align_size( 1024 );
    constexpr static int     max_hits         = 26;
    constexpr static int     max_states       = 2;
    constexpr static int     params_per_state = 11;
    constexpr static int     other_params     = 1;
    Zipping::ZipFamilyNumber m_zipIdentifier;

  public:
    Tracks( Zipping::ZipFamilyNumber zipIdentifier = Zipping::generateZipIdentifier() )
        : m_zipIdentifier{zipIdentifier} {
      const size_t size = max_tracks * ( max_hits + max_states * params_per_state + other_params );
      m_data            = static_cast<data_t*>( std::aligned_alloc( 64, size * sizeof( int ) ) );
    }

    Tracks( const Tracks& ) = delete;

    // Special constructor for zipping machinery
    Tracks( Zipping::ZipFamilyNumber zipIdentifier, Tracks const& ) : Tracks( zipIdentifier ) {}

    Tracks( Tracks&& other ) : m_zipIdentifier{other.m_zipIdentifier} {
      m_data       = other.m_data;
      other.m_data = nullptr;
      m_size       = other.m_size;
    }

    inline int               size() const { return m_size; }
    inline int&              size() { return m_size; }
    bool                     empty() const { return m_size == 0; }
    Zipping::ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

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
      for ( int i = 0; i < max_hits + max_states * params_per_state + other_params; i++ ) {
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
} // namespace LHCb::Pr::Velo

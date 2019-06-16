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
 * Hits in VP
 *
 * @author: Arthur Hennequin
 */

namespace LHCb::Pr::Velo {
  class Hits {
    constexpr static int max_hits = align_size( 10000 );

  public:
    Hits() {
      const size_t size = max_hits * 4;
      m_data            = static_cast<data_t*>( std::aligned_alloc( 64, size * sizeof( int ) ) );
    }

    Hits( const Hits& ) = delete;

    Hits( Hits&& other ) {
      m_data       = other.m_data;
      other.m_data = nullptr;
      m_size       = other.m_size;
    }

    inline int  size() const { return m_size; }
    inline int& size() { return m_size; }

    VEC3_SOA_ACCESSOR( pos, &( m_data->f ), &( m_data[max_hits].f ), &( m_data[2 * max_hits].f ) )

    SOA_ACCESSOR( LHCbId, &m_data[3 * max_hits].i )

    ~Hits() { std::free( m_data ); }

  private:
    using data_t = union {
      float f;
      int   i;
    };
    alignas( 64 ) data_t* m_data;
    int m_size = 0;
  };
} // namespace LHCb::Pr::Velo

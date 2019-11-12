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
#ifndef EVENT_PRUTHITS_H
#define EVENT_PRUTHITS_H 1

// Include files
#include "LHCbMath/SIMDWrapper.h"

/** @class PrUTHits PrUTHits.h
 *  SoA Implementation of Upstream tracker hit for pattern recognition
 *  @author Michel De Cian, based on Arthur Hennequin's PrVeloHits
 *  @date   2019-11-07
 */

namespace LHCb::Pr::UT {

  class Hits {
    constexpr static int max_hits = align_size( 10000 );

  public:
    Hits() {
      const size_t size = max_hits * 6;
      m_data            = static_cast<data_t*>( std::aligned_alloc( 64, size * sizeof( int ) ) );
    }

    Hits( const Hits& ) = delete;

    Hits( Hits&& other ) : m_data{std::exchange( other.m_data, nullptr )}, m_size{other.m_size} {}
    int  size() const { return m_size; }
    int& size() { return m_size; }

    SOA_ACCESSOR( channelID, &m_data[0 * max_hits].i )
    SOA_ACCESSOR( weight, &m_data[1 * max_hits].f )
    SOA_ACCESSOR( xAtYEq0, &m_data[2 * max_hits].f )
    SOA_ACCESSOR( yBegin, &m_data[3 * max_hits].f )
    SOA_ACCESSOR( yEnd, &m_data[4 * max_hits].f )
    SOA_ACCESSOR( zAtYEq0, &m_data[5 * max_hits].f )

    ~Hits() { std::free( m_data ); }

  private:
    using data_t = union {
      float f;
      int   i;
    };
    alignas( 64 ) data_t* m_data;
    int m_size = 0;
  };
} // namespace LHCb::Pr::UT

#endif // EVENT_PRUTHITS_H

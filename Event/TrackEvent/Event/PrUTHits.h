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
#include "Kernel/EventLocalAllocator.h"
#include "LHCbMath/SIMDWrapper.h"

/** @class PrUTHits PrUTHits.h
 *  SoA Implementation of Upstream tracker hit for pattern recognition
 *  @author Michel De Cian, based on Arthur Hennequin's PrVeloHits
 *  @date   2019-11-07
 */

namespace LHCb::Pr::UT {
  class Hits {
    constexpr static int         max_hits = align_size( 10000 );
    constexpr static std::size_t capacity = max_hits * 8;
    using data_t                          = union {
      float f;
      int   i;
    };

  public:
    using allocator_type = LHCb::Allocators::EventLocal<data_t>;
    Hits( allocator_type alloc = {} )
        : m_data{std::allocator_traits<allocator_type>::allocate( alloc, capacity )}, m_alloc{std::move( alloc )} {}

    Hits( const Hits& ) = delete;

    Hits( Hits&& other )
        : m_data{std::exchange( other.m_data, nullptr )}
        , m_size{std::exchange( other.m_size, 0 )}
        , m_alloc{std::move( other.m_alloc )} {}

    [[nodiscard]] int size() const { return m_size; }
    int&              size() { return m_size; }

    SOA_ACCESSOR( channelID, &m_data[0 * max_hits].i )
    SOA_ACCESSOR( weight, &m_data[1 * max_hits].f )
    SOA_ACCESSOR( xAtYEq0, &m_data[2 * max_hits].f )
    SOA_ACCESSOR( yBegin, &m_data[3 * max_hits].f )
    SOA_ACCESSOR( yEnd, &m_data[4 * max_hits].f )
    SOA_ACCESSOR( zAtYEq0, &m_data[5 * max_hits].f )
    SOA_ACCESSOR( dxDy, &m_data[6 * max_hits].f )
    SOA_ACCESSOR( cos, &m_data[7 * max_hits].f )

    ~Hits() { std::allocator_traits<allocator_type>::deallocate( m_alloc, m_data, capacity ); }

  private:
    data_t*        m_data{nullptr};
    int            m_size{0};
    allocator_type m_alloc;
  };
} // namespace LHCb::Pr::UT

#endif // EVENT_PRUTHITS_H

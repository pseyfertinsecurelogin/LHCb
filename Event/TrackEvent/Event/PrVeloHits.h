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

#include "Kernel/EventLocalAllocator.h"
#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/Vec3.h"
#include "SOAExtensions/ZipUtils.h"

/**
 * Hits in VP
 *
 * @author: Arthur Hennequin
 */
namespace LHCb::Pr::Velo {
  class Hits {
    constexpr static int         max_hits = align_size( 10000 );
    constexpr static std::size_t capacity = max_hits * 4;
    using data_t                          = union {
      float f;
      int   i;
    };

  public:
    using allocator_type = LHCb::Allocators::EventLocal<data_t>;
    Hits( Zipping::ZipFamilyNumber = Zipping::generateZipIdentifier(), allocator_type alloc = {} )
        : m_alloc{std::move( alloc )}, m_data{std::allocator_traits<allocator_type>::allocate( m_alloc, capacity )} {}

    Hits( const Hits& ) = delete;

    Hits( Hits&& other )
        : m_alloc{std::move( other.m_alloc )}
        , m_data{std::exchange( other.m_data, nullptr )}
        , m_size{std::exchange( other.m_size, 0 )} {}

    [[nodiscard]] int size() const { return m_size; }
    int&              size() { return m_size; }

    VEC3_SOA_ACCESSOR( pos, &( m_data->f ), &( m_data[max_hits].f ), &( m_data[2 * max_hits].f ) )

    SOA_ACCESSOR( ChannelId, &m_data[3 * max_hits].i )

    ~Hits() { std::allocator_traits<allocator_type>::deallocate( m_alloc, m_data, capacity ); }

  private:
    allocator_type m_alloc;
    data_t*        m_data{nullptr};
    int            m_size{0};
  };
} // namespace LHCb::Pr::Velo

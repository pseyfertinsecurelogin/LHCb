/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
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
namespace LHCb::Pr::detail {
  template <class PODTracks_t, typename dType>
  struct PODProxy {
    PODTracks_t const* m_tracks{nullptr};
    int                m_offset{0};
    PODProxy( PODTracks_t const* tracks, int offset ) : m_tracks( tracks ), m_offset( offset ) {}
    auto offset() const { return m_offset; }
    auto loop_mask() const { return dType::loop_mask( m_offset, m_tracks->size() ); }
  };

  template <class PODTracks_t, template <typename> typename PODProxy_t, typename dType>
  class IterablePODTracks {
    PODTracks_t const* m_tracks{nullptr};
    struct Iterator {
      PODTracks_t const* m_tracks{nullptr};
      int                m_offset{0};
      Iterator( PODTracks_t const* tracks, int offset ) : m_tracks( tracks ), m_offset( offset ) {}
      PODProxy_t<dType> operator*() const { return {m_tracks, m_offset}; };
      Iterator          operator++() {
        m_offset += dType::size;
        return *this;
      }
      friend bool operator!=( Iterator const& lhs, Iterator const& rhs ) {
        return lhs.m_tracks != rhs.m_tracks || lhs.m_offset != rhs.m_offset;
      }
    };

  public:
    IterablePODTracks( PODTracks_t const& tracks ) : m_tracks( &tracks ) {}
    Iterator begin() const { return {m_tracks, 0}; }
    Iterator end() const {
      // m_offset is incremented by dType::size each time, so repeatedly
      // incrementing begin() generally misses {m_tracks, m_tracks->size()}
      int num_chunks = ( m_tracks->size() + dType::size - 1 ) / dType::size;
      int max_offset = num_chunks * dType::size;
      return {m_tracks, max_offset};
    }
  };

  template <typename dType>
  struct VeloProxy : public detail::PODProxy<LHCb::Pr::Velo::Tracks, dType> {
    using FType = typename dType::float_v;
    using detail::PODProxy<LHCb::Pr::Velo::Tracks, dType>::PODProxy;
    decltype( auto ) closestToBeamStatePos() const {
      return this->m_tracks->template statePos<FType>( this->m_offset, 0 );
    }
    decltype( auto ) closestToBeamStateDir() const {
      return this->m_tracks->template stateDir<FType>( this->m_offset, 0 );
    }
    decltype( auto ) pseudoRapidity() const { return this->closestToBeamStateDir().eta(); }
  };
} // namespace LHCb::Pr::detail

namespace LHCb::Pr::Velo {
  /** Shorthand for an iterable wrapper around an LHCb::Pr::Velo::Tracks object.
   */
  template <typename dType>
  using IterableTracks =
      LHCb::Pr::detail::IterablePODTracks<LHCb::Pr::Velo::Tracks, LHCb::Pr::detail::VeloProxy, dType>;
} // namespace LHCb::Pr::Velo

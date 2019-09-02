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

namespace LHCb::Pr::Velo {
  namespace detail {
    template <typename VecType>
    struct State {
      VecType m_position, m_slopes;
      using F = typename VecType::value_type;
      State( VecType position, VecType slopes )
          : m_position( std::move( position ) ), m_slopes( std::move( slopes ) ) {}
      VecType const& position() const { return m_position; }
      VecType const& slopes() const { return m_slopes; }
      auto           x() const { return position().x; }
      auto           y() const { return position().y; }
      auto           z() const { return position().z; }
      auto           tx() const { return slopes().x; }
      auto           ty() const { return slopes().y; }
    };
  } // namespace detail

  /** Proxy for iterating over LHCb::Pr::Velo::Tracks objects. */
  template <typename MergedProxy, typename dType>
  struct Proxy {
    // TODO these next four lines could/should be macro'd
    Tracks const* m_tracks{nullptr};
    Proxy( Tracks const* tracks ) : m_tracks{tracks} {}
    auto offset() const { return static_cast<MergedProxy const&>( *this ).offset(); }
    auto size() const { return m_tracks->size(); }

    using FType = typename dType::float_v;

    decltype( auto ) closestToBeamStatePos() const {
      return this->m_tracks->template statePos<FType>( this->offset(), 0 );
    }
    decltype( auto ) closestToBeamStateDir() const {
      return this->m_tracks->template stateDir<FType>( this->offset(), 0 );
    }
    auto             closestToBeamState() const { return State{closestToBeamStatePos(), closestToBeamStateDir()}; }
    decltype( auto ) pseudoRapidity() const { return this->closestToBeamStateDir().eta(); }
  };
} // namespace LHCb::Pr::Velo

// Allow the proxy type to be found from the track container type
template <>
struct LHCb::Pr::Proxy<LHCb::Pr::Velo::Tracks> {
  template <typename MergedProxy, typename dType, bool>
  using type = LHCb::Pr::Velo::Proxy<MergedProxy, dType>;
};
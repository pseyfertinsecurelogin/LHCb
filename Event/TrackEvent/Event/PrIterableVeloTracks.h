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
#include "Event/PrZip.h"
#include "Kernel/HeaderMapping.h"

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
  DECLARE_PROXY( Proxy ) {
    PROXY_METHODS( dType, unwrap, Tracks, m_tracks );
    using FType = typename dType::float_v;
    using IType = typename dType::int_v;

    decltype( auto ) closestToBeamStatePos() const {
      return this->m_tracks->template statePos<FType>( this->offset(), 0 );
    }
    decltype( auto ) closestToBeamStateDir() const {
      return this->m_tracks->template stateDir<FType>( this->offset(), 0 );
    }
    auto closestToBeamState() const { return detail::State{closestToBeamStatePos(), closestToBeamStateDir()}; }
    auto nHits() const { return this->m_tracks->template nHits<IType>( this->offset() ); }
    decltype( auto ) pseudoRapidity() const { return this->closestToBeamStateDir().eta(); }
    decltype( auto ) phi() const { return this->closestToBeamStateDir().phi(); }
  };
} // namespace LHCb::Pr::Velo

// Allow the proxy type to be found from the track container type
REGISTER_PROXY( LHCb::Pr::Velo::Tracks, LHCb::Pr::Velo::Proxy );

template <>
struct LHCb::header_map<LHCb::Pr::Velo::Tracks> {
  constexpr static string_array value{"Event/PrIterableVeloTracks.h"};
};

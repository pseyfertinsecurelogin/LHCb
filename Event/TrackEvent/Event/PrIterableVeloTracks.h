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
#include "Event/PrProxyHelpers.h"
#include "Event/PrVeloTracks.h"
#include "Event/PrZip.h"
#include "Kernel/HeaderMapping.h"

namespace LHCb::Pr::Velo {
  /** Proxy for iterating over LHCb::Pr::Velo::Tracks objects. */
  DECLARE_PROXY( Proxy ) {
    PROXY_METHODS( dType, unwrap, Tracks, m_tracks );
    using FType = typename dType::float_v;
    using IType = typename dType::int_v;

    auto closestToBeamStatePos() const {
      return LHCb::Pr::detail::castToPoint<unwrap>( m_tracks->template statePos<FType>( this->offset(), 0 ) );
    }
    auto closestToBeamStateDir() const {
      return LHCb::Pr::detail::castToVector<unwrap>( m_tracks->template stateDir<FType>( this->offset(), 0 ) );
    }
    auto closestToBeamState() const {
      return LHCb::Pr::detail::VeloState{closestToBeamStatePos(), closestToBeamStateDir()};
    }
    auto nHits() const {
      return LHCb::Pr::detail::cast<unwrap>( this->m_tracks->template nHits<IType>( this->offset() ) );
    }
    auto pseudoRapidity() const { return this->closestToBeamStateDir().eta(); }
    auto phi() const { return this->closestToBeamStateDir().phi(); }
  };
} // namespace LHCb::Pr::Velo

// Allow the proxy type to be found from the track container type
REGISTER_PROXY( LHCb::Pr::Velo::Tracks, LHCb::Pr::Velo::Proxy );

namespace LHCb::Pr::Iterable::Scalar::Velo {
  using Tracks = LHCb::Pr::unwrapped_zip_t<LHCb::Pr::Velo::Tracks>;
} // namespace LHCb::Pr::Iterable::Scalar::Velo

REGISTER_HEADER( LHCb::Pr::Velo::Tracks, "Event/PrIterableVeloTracks.h" );
REGISTER_HEADER( LHCb::Pr::Iterable::Scalar::Velo::Tracks, "Event/PrIterableVeloTracks.h" );
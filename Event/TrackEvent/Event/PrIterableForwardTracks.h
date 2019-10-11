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
#include "Event/PrForwardTracks.h"
#include "Event/PrProxyHelpers.h"
#include "Event/PrZip.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "Kernel/HeaderMapping.h"

namespace LHCb::Pr::Forward {
  namespace detail {
    /** Helper type for fitted track proxies */
    template <typename TrackProxy>
    struct ScifiState {
      TrackProxy const& m_proxy;
      ScifiState( TrackProxy const& proxy ) : m_proxy{proxy} {}
      decltype( auto ) qOverP() const { return m_proxy.qOverP(); }
      decltype( auto ) slopes() const { return m_proxy.endScifiStateDir(); }
      decltype( auto ) position() const { return m_proxy.endScifiStatePos(); }
      // TODO maybe stick these in a base class via CRTP if it gets a bit cumbersome?
      decltype( auto ) x() const { return position().X(); }
      decltype( auto ) y() const { return position().Y(); }
      decltype( auto ) z() const { return position().Z(); }
      decltype( auto ) tx() const { return slopes().X(); }
      decltype( auto ) ty() const { return slopes().Y(); }
    };
  } // namespace detail

  /** Proxy for iterating over LHCb::Pr::Forward::Tracks objects. */
  DECLARE_PROXY( Proxy ) {
    PROXY_METHODS( dType, unwrap, Tracks, m_tracks );
    using IType = typename dType::int_v;
    using FType = typename dType::float_v;

    auto closestToBeamStatePos() const {
      // Fail if we're asked to do this in vector mode...
      static_assert( std::is_same_v<dType, SIMDWrapper::scalar::types>,
                     "Forward->Velo links only supported in scalar mode." );
      return LHCb::Pr::detail::castToPoint<unwrap>( m_tracks->getVeloAncestors()->template statePos<FType>(
          m_tracks->template trackVP<IType>( this->offset() ).cast(), 0 ) );
    }

    auto closestToBeamStateDir() const {
      // Fail if we're asked to do this in vector mode...
      static_assert( std::is_same_v<dType, SIMDWrapper::scalar::types>,
                     "Forward->Velo links only supported in scalar mode." );
      return LHCb::Pr::detail::castToVector<unwrap>( m_tracks->getVeloAncestors()->template stateDir<FType>(
          m_tracks->template trackVP<IType>( this->offset() ).cast(), 0 ) );
    }

    auto closestToBeamState() const {
      return LHCb::Pr::detail::VeloState{closestToBeamStatePos(), closestToBeamStateDir()};
    }

    auto phi() const { return closestToBeamState().slopes().phi(); }
    auto pseudoRapidity() const { return closestToBeamState().slopes().eta(); }

    auto endScifiState() const { return detail::ScifiState{*this}; }

    auto p() const { return LHCb::Pr::detail::cast<unwrap>( m_tracks->template p<FType>( this->offset() ) ); }
    auto pt() const {
      auto const mom = p();
      auto const dir = closestToBeamStateDir();
      auto const pt2 = mom * mom * ( dir.X() * dir.X() + dir.Y() * dir.Y() ) / dir.mag2();
      using std::sqrt;
      return sqrt( pt2 );
    }

    auto qOverP() const {
      return LHCb::Pr::detail::cast<unwrap>( m_tracks->template stateQoP<FType>( this->offset() ) );
    }

    auto endScifiStatePos() const {
      return LHCb::Pr::detail::castToPoint<unwrap>( m_tracks->template statePos<FType>( this->offset() ) );
    }

    auto endScifiStateDir() const {
      return LHCb::Pr::detail::castToVector<unwrap>( m_tracks->template stateDir<FType>( this->offset() ) );
    }
  };
} // namespace LHCb::Pr::Forward

// Allow the proxy type to be found from the track container type
REGISTER_PROXY( LHCb::Pr::Forward::Tracks, LHCb::Pr::Forward::Proxy );

namespace LHCb::Pr::Iterable::Scalar::Forward {
  using Tracks = LHCb::Pr::unwrapped_zip_t<LHCb::Pr::Forward::Tracks>;
} // namespace LHCb::Pr::Iterable::Scalar::Forward

REGISTER_HEADER( LHCb::Pr::Forward::Tracks, "Event/PrIterableForwardTracks.h" );
REGISTER_HEADER( LHCb::Pr::Iterable::Scalar::Forward::Tracks, "Event/PrIterableForwardTracks.h" );
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
  template <typename MergedProxy, typename dType, bool unwrap_tparam>
  struct Proxy {
    // TODO these next four lines could/should be macro'd
    Tracks const* m_tracks{nullptr};
    Proxy( Tracks const* tracks ) : m_tracks{tracks} {}
    auto offset() const { return static_cast<MergedProxy const&>( *this ).offset(); }
    auto size() const { return m_tracks->size(); }

    static constexpr bool unwrap = unwrap_tparam;
    using FType                  = typename dType::float_v;

    template <typename T>
    static constexpr auto cast( T x ) {
      if constexpr ( unwrap ) {
        return x.cast();
      } else {
        return x;
      }
    }

    auto endScifiState() const { return detail::ScifiState{*this}; }

    auto p() const { return cast( this->m_tracks->template p<FType>( this->offset() ) ); }

    decltype( auto ) endScifiStatePos() const {
      auto pos = this->m_tracks->template statePos<FType>( this->offset() );
      if constexpr ( unwrap )
        return Gaudi::XYZPointF{cast( pos.x ), cast( pos.y ), cast( pos.z )};
      else
        return pos;
    }

    decltype( auto ) endScifiStateDir() const {
      auto dir = this->m_tracks->template stateDir<FType>( this->offset() );
      if constexpr ( unwrap )
        return Gaudi::XYZVectorF{cast( dir.x ), cast( dir.y ), cast( dir.z )};
      else
        return dir;
    }
  };
} // namespace LHCb::Pr::Forward

// Allow the proxy type to be found from the track container type
template <>
struct LHCb::Pr::Proxy<LHCb::Pr::Forward::Tracks> {
  template <typename MergedProxy, typename dType, bool unwrap>
  using type = LHCb::Pr::Forward::Proxy<MergedProxy, dType, unwrap>;
};
template <>
struct LHCb::header_map<LHCb::Pr::Forward::Tracks> {
  constexpr static string_array value{"Event/PrIterableForwardTracks.h"};
};

namespace LHCb::Pr::Iterable::Forward {
  using Tracks = LHCb::Pr::zip_t<LHCb::Pr::Forward::Tracks>;
} // namespace LHCb::Pr::Iterable::Forward

namespace LHCb::Pr::Iterable::Scalar::Forward {
  using Tracks = LHCb::Pr::unwrapped_zip_t<LHCb::Pr::Forward::Tracks>;
} // namespace LHCb::Pr::Iterable::Scalar::Forward

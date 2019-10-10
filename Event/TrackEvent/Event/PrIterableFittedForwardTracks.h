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
#include "Event/PrFittedForwardTracks.h"
#include "Event/PrZip.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include <Math/SMatrix.h>
namespace LHCb::Pr::Fitted::Forward {
  namespace detail {
    /** Helper type for fitted track proxies */
    template <typename TrackProxy>
    struct FittedState {
      TrackProxy const& m_proxy;
      FittedState( TrackProxy const& proxy ) : m_proxy{proxy} {}
      decltype( auto ) qOverP() const { return m_proxy.qOverP(); }
      decltype( auto ) slopes() const { return m_proxy.closestToBeamStateDir(); }
      decltype( auto ) position() const { return m_proxy.closestToBeamStatePos(); }
      decltype( auto ) covariance() const { return m_proxy.closestToBeamStateCovariance(); }
      // TODO maybe stick these in a base class via CRTP if it gets a bit cumbersome?
      decltype( auto ) x() const { return position().X(); }
      decltype( auto ) y() const { return position().Y(); }
      decltype( auto ) z() const { return position().Z(); }
      decltype( auto ) tx() const { return slopes().X(); }
      decltype( auto ) ty() const { return slopes().Y(); }
    };
  } // namespace detail

  /** Proxy type for iterating over LHCb::Pr::Fitted::Forward::Tracks objects. */
  template <typename MergedProxy, typename dType, bool unwrap_tparam>
  struct Proxy {
    // TODO these next four lines could/should be macro'd
    Tracks const* m_tracks{nullptr};
    Proxy( Tracks const* tracks ) : m_tracks{tracks} {}
    auto offset() const { return static_cast<MergedProxy const&>( *this ).offset(); }
    auto size() const { return m_tracks->size(); }

    static constexpr bool unwrap = unwrap_tparam;
    using IType                  = typename dType::int_v;
    using FType                  = typename dType::float_v;
    // FIXME make this configurable or at least float. For now double is assumed elsewhere.
    using FTypeOut     = std::conditional_t<unwrap, double, FType>;
    using MatrixSym5x5 = ROOT::Math::SMatrix<FTypeOut, 5, 5, ROOT::Math::MatRepSym<FTypeOut, 5>>;

    template <typename T>
    static constexpr auto cast( T x ) {
      if constexpr ( unwrap ) {
        return x.cast();
      } else {
        return x;
      }
    }

    decltype( auto ) closestToBeamStateDir() const {
      auto dir = this->m_tracks->template beamStateDir<FType>( this->offset() );
      if constexpr ( unwrap )
        return Gaudi::XYZVectorF{cast( dir.x ), cast( dir.y ), cast( dir.z )};
      else
        return dir;
    }

    decltype( auto ) closestToBeamStatePos() const {
      auto pos = this->m_tracks->template beamStatePos<FType>( this->offset() );
      if constexpr ( unwrap )
        return Gaudi::XYZPointF{cast( pos.x ), cast( pos.y ), cast( pos.z )};
      else
        return pos;
    }

    decltype( auto ) qOverP() const { return cast( this->m_tracks->template QoP<FType>( this->offset() ) ); }

    auto closestToBeamStateCovariance() const {
      MatrixSym5x5 cov{ROOT::Math::SMatrixNoInit{}};
      auto const   qop  = qOverP();
      auto const   covX = this->m_tracks->template covX<FType>( this->offset() );
      auto const   covY = this->m_tracks->template covY<FType>( this->offset() );
      cov( 0, 0 )       = cast( covX.x ); // x error
      cov( 0, 2 )       = cast( covX.y ); // x-tx covariance
      cov( 2, 2 )       = cast( covX.z ); // tx error
      cov( 1, 1 )       = cast( covY.x ); // y error
      cov( 1, 3 )       = cast( covY.y ); // y-ty covariance
      cov( 3, 3 )       = cast( covY.z ); // ty error
      cov( 0, 1 )       = 0.f;            // x-y covariance
      cov( 0, 3 )       = 0.f;            // x-ty covariance
      cov( 0, 4 )       = 0.f;            // x-qop covariance
      cov( 1, 2 )       = 0.f;            // y-tx covariance
      cov( 1, 4 )       = 0.f;            // y-qop covariance
      cov( 2, 3 )       = 0.f;            // tx-ty covariance
      cov( 2, 4 )       = 0.f;            // tx-qop covariance
      cov( 3, 4 )       = 0.f;            // ty-qop covariance
      // FIXME this hack should not be hardcoded
      cov( 4, 4 ) = .1f * qop * qop; // qop erro
      return cov;
    }

    auto closestToBeamState() const { return detail::FittedState{*this}; }

    auto p() const { return cast( this->m_tracks->template p<FType>( this->offset() ) ); }

    auto pt() const {
      auto const mom = p();
      auto const dir = closestToBeamStateDir();
      auto const pt2 = mom * mom * ( dir.X() * dir.X() + dir.Y() * dir.Y() ) / dir.mag2();
      using std::sqrt;
      return sqrt( pt2 );
    }

    auto pseudoRapidity() const { return closestToBeamStateDir().eta(); }
    auto phi() const { return closestToBeamStateDir().phi(); }

    auto momentum() const {
      auto const dir = closestToBeamStateDir();
      using std::sqrt;
      auto const scale = p() / sqrt( dir.mag2() );
      if constexpr ( unwrap ) {
        return Gaudi::XYZVectorF{scale * dir.X(), scale * dir.Y(), scale * dir.Z()};
      } else {
        return Vec3<FType>{scale * dir.x, scale * dir.y, scale * dir.z};
      }
    }

    auto chi2PerDoF() const { return cast( this->m_tracks->template chi2<FType>( this->offset() ) ); }

    auto lhcbIDs( LHCb::Pr::Velo::Hits const& velo_hits ) const {
      return this->m_tracks->template lhcbIDs<IType>( this->offset(), velo_hits );
    }
  };
} // namespace LHCb::Pr::Fitted::Forward

template <>
struct LHCb::Pr::Proxy<LHCb::Pr::Fitted::Forward::Tracks> {
  template <typename MergedProxy, typename dType, bool unwrap>
  using type = LHCb::Pr::Fitted::Forward::Proxy<MergedProxy, dType, unwrap>;
};

namespace LHCb::Pr::Iterable::Fitted::Forward {
  using Tracks = LHCb::Pr::zip_t<LHCb::Pr::Fitted::Forward::Tracks>;
} // namespace LHCb::Pr::Iterable::Fitted::Forward

namespace LHCb::Pr::Iterable::Scalar::Fitted::Forward {
  using Tracks = LHCb::Pr::unwrapped_zip_t<LHCb::Pr::Fitted::Forward::Tracks>;
} // namespace LHCb::Pr::Iterable::Scalar::Fitted::Forward

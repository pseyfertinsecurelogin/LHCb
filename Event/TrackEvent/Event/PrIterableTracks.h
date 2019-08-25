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
#include "Event/PrVeloTracks.h"
#include "GaudiKernel/Vector3DTypes.h"
#include <Math/SMatrix.h>
namespace LHCb::Pr::detail {
  /** Base class for the various wrappers that make LHCb::Pr::*::Tracks
   *  containers iterable */
  template <class PODTracks_t, typename dType>
  struct PODProxy {
    static constexpr bool is_proxy{true};
    PODTracks_t const*    m_tracks{nullptr};
    int                   m_offset{0};
    PODProxy( PODTracks_t const* tracks, int offset ) : m_tracks( tracks ), m_offset( offset ) {}
    auto offset() const { return m_offset; }
    auto loop_mask() const { return dType::loop_mask( m_offset, m_tracks->size() ); }
  };

  template <typename VecType>
  struct State {
    VecType m_position, m_slopes;
    using F = typename VecType::value_type;
    State( VecType position, VecType slopes ) : m_position( std::move( position ) ), m_slopes( std::move( slopes ) ) {}
    VecType const& position() const { return m_position; }
    VecType const& slopes() const { return m_slopes; }
    auto           x() const { return position().x; }
    auto           y() const { return position().y; }
    auto           z() const { return position().z; }
    auto           tx() const { return slopes().x; }
    auto           ty() const { return slopes().y; }
  };

  /** Proxy for iterating over LHCb::Pr::Velo::Tracks objects. */
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
    auto             closestToBeamState() const { return State{closestToBeamStatePos(), closestToBeamStateDir()}; }
    decltype( auto ) pseudoRapidity() const { return this->closestToBeamStateDir().eta(); }
  };

  /** Helper type for fitted track proxies */
  template <typename TrackProxy>
  struct FittedState {
    /** Helper to convert scalar::float_v -> float if requested. */
    template <typename T>
    static constexpr auto cast( T x ) {
      if constexpr ( TrackProxy::unwrap ) {
        return x.cast();
      } else {
        return x;
      }
    }
    TrackProxy const& m_proxy;
    FittedState( TrackProxy const& proxy ) : m_proxy{proxy} {}
    decltype( auto ) qOverP() const { return m_proxy.qOverP(); }
    decltype( auto ) slopes() const { return m_proxy.closestToBeamStateDir(); }
    decltype( auto ) position() const { return m_proxy.closestToBeamStatePos(); }
    decltype( auto ) covariance() const { return m_proxy.closestToBeamStateCovariance(); }
    // TODO maybe stick these in a base class via CRTP if it gets a bit cumbersome?
    decltype( auto ) x() const { return cast( position().x ); }
    decltype( auto ) y() const { return cast( position().y ); }
    decltype( auto ) z() const { return cast( position().z ); }
    decltype( auto ) tx() const { return cast( slopes().x ); }
    decltype( auto ) ty() const { return cast( slopes().y ); }
  };

  /** Proxy type for iterating over LHCb::Pr::Fitted::Forward::Tracks objects. */
  template <typename dType, bool unwrap_tparam>
  struct FittedForwardProxy : public detail::PODProxy<LHCb::Pr::Fitted::Forward::Tracks, dType> {
    static constexpr bool unwrap = unwrap_tparam;
    using IType                  = typename dType::int_v;
    using FType                  = typename dType::float_v;
    // FIXME make this configurable or at least float. For now double is assumed elsewhere.
    using FTypeOut     = std::conditional_t<unwrap, double, FType>;
    using MatrixSym5x5 = ROOT::Math::SMatrix<FTypeOut, 5, 5, ROOT::Math::MatRepSym<FTypeOut, 5>>;
    using detail::PODProxy<LHCb::Pr::Fitted::Forward::Tracks, dType>::PODProxy;
    template <typename T>
    static constexpr auto cast( T x ) {
      if constexpr ( unwrap ) {
        return x.cast();
      } else {
        return x;
      }
    }

    decltype( auto ) closestToBeamStateDir() const {
      return this->m_tracks->template beamStateDir<FType>( this->m_offset );
    }

    decltype( auto ) closestToBeamStatePos() const {
      return this->m_tracks->template beamStatePos<FType>( this->m_offset );
    }

    decltype( auto ) qOverP() const { return cast( this->m_tracks->template QoP<FType>( this->m_offset ) ); }

    auto closestToBeamStateCovariance() const {
      MatrixSym5x5 cov{ROOT::Math::SMatrixNoInit{}};
      auto const   qop  = qOverP();
      auto const   covX = this->m_tracks->template covX<FType>( this->m_offset );
      auto const   covY = this->m_tracks->template covY<FType>( this->m_offset );
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

    auto closestToBeamState() const { return FittedState{*this}; }

    auto p() const { return cast( this->m_tracks->template p<FType>( this->m_offset ) ); }

    auto pt() const {
      auto const mom = p();
      auto const dir = closestToBeamStateDir();
      auto const pt2 = mom * mom * ( dir.x * dir.x + dir.y * dir.y ) / dir.mag2();
      return cast( sqrt( pt2 ) );
    }

    auto pseudoRapidity() const { return closestToBeamStateDir().eta(); }

    auto momentum() const {
      auto const dir   = closestToBeamStateDir();
      auto const scale = p() / dir.mag();
      if constexpr ( unwrap ) {
        return Gaudi::XYZVector{cast( scale * dir.x ), cast( scale * dir.y ), cast( scale * dir.z )};
      } else {
        return Vec3<FType>{scale * dir.x, scale * dir.y, scale * dir.z};
      }
    }

    auto chi2PerDoF() const { return cast( this->m_tracks->template chi2<FType>( this->m_offset ) ); }
  };

  /** Helpers for getting from container type -> proxy type. */
  template <typename>
  struct Proxy {};

  template <>
  struct Proxy<LHCb::Pr::Velo::Tracks> {
    template <typename T, bool>
    using type = VeloProxy<T>;
  };

  template <>
  struct Proxy<LHCb::Pr::Fitted::Forward::Tracks> {
    template <typename T, bool unwrap>
    using type = FittedForwardProxy<T, unwrap>;
  };

  /** This is the type returned by LHCb::Pr::IterableTracks */
  template <class PODTracks_t, typename dType, bool unwrap = false>
  class IterablePODTracks {
  public:
    using proxy_type = typename Proxy<PODTracks_t>::template type<dType, unwrap>;

  private:
    PODTracks_t const* m_tracks{nullptr};
    struct Iterator {
      using value_type        = proxy_type;
      using pointer           = value_type const*;
      using reference         = value_type const&;
      using difference_type   = int;
      using iterator_category = std::random_access_iterator_tag;
      PODTracks_t const* m_tracks{nullptr};
      int                m_offset{0};
      Iterator( PODTracks_t const* tracks, int offset ) : m_tracks( tracks ), m_offset( offset ) {}
      value_type operator*() const { return {m_tracks, m_offset}; };
      Iterator   operator++() {
        m_offset += dType::size;
        return *this;
      }
      Iterator operator--() {
        m_offset -= dType::size;
        return *this;
      }
      Iterator& operator+=( difference_type n ) {
        m_offset += n * dType::size;
        return *this;
      }
      friend bool operator!=( Iterator const& lhs, Iterator const& rhs ) {
        return lhs.m_tracks != rhs.m_tracks || lhs.m_offset != rhs.m_offset;
      }
      friend difference_type operator-( Iterator const& lhs, Iterator const& rhs ) {
        return ( lhs.m_offset - rhs.m_offset );
      }
    };

  public:
    using value_type = proxy_type;
    IterablePODTracks( PODTracks_t const& tracks ) : m_tracks( &tracks ) {}
    Iterator begin() const { return {m_tracks, 0}; }
    Iterator end() const {
      // m_offset is incremented by dType::size each time, so repeatedly
      // incrementing begin() generally misses {m_tracks, m_tracks->size()}
      int num_chunks = ( m_tracks->size() + dType::size - 1 ) / dType::size;
      int max_offset = num_chunks * dType::size;
      return {m_tracks, max_offset};
    }
    // FIXME you probably don't want to use this with a vector dType
    value_type     operator[]( int index ) const { return {m_tracks, index}; }
    unsigned short family() const { return 24; }
  };

  template <typename PrTracks>
  using UnwrappedScalarIterableTracks = IterablePODTracks<PrTracks, SIMDWrapper::scalar::types, true>;
} // namespace LHCb::Pr::detail

namespace LHCb::Pr {
  /** Construct an appropriate iterable wrapper from the given container
   *
   *  e.g. transform LHCb::Pr::Velo::Tracks to an iterable wrapper around that,
   *  this is just a helper function that saves writing out so many explicit
   *  types...
   */
  template <typename dType, bool unwrap = false, typename PrTracks>
  auto IterableTracks( PrTracks const& tracks ) {
    return detail::IterablePODTracks<PrTracks, dType, unwrap>{tracks};
  }

  namespace Iterable::Fitted::Forward {
    using Tracks = LHCb::Pr::detail::UnwrappedScalarIterableTracks<LHCb::Pr::Fitted::Forward::Tracks>;
  }
} // namespace LHCb::Pr
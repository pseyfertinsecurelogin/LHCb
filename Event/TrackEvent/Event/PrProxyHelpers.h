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
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
namespace LHCb::Pr::detail {
  template <bool unwrap, typename T>
  constexpr auto cast( T&& x ) {
    if constexpr ( unwrap ) {
      return x.cast();
    } else {
      return x;
    }
  }

  template <bool unwrap, typename T>
  constexpr auto castToVector( T&& vec ) {
    if constexpr ( unwrap ) {
      return Gaudi::XYZVectorF{cast<unwrap>( vec.x ), cast<unwrap>( vec.y ), cast<unwrap>( vec.z )};
    } else {
      return vec;
    }
  }

  template <bool unwrap, typename T>
  constexpr auto castToPoint( T&& pos ) {
    if constexpr ( unwrap ) {
      return Gaudi::XYZPointF{cast<unwrap>( pos.x ), cast<unwrap>( pos.y ), cast<unwrap>( pos.z )};
    } else {
      return pos;
    }
  }

  template <typename PosType, typename SlopeType>
  struct VeloState {
    PosType   m_position;
    SlopeType m_slopes;
    VeloState( PosType position, SlopeType slopes )
        : m_position{std::move( position )}, m_slopes{std::move( slopes )} {}
    PosType const&   position() const { return m_position; }
    SlopeType const& slopes() const { return m_slopes; }
    auto             x() const { return position().X(); }
    auto             y() const { return position().Y(); }
    auto             z() const { return position().Z(); }
    auto             tx() const { return slopes().X(); }
    auto             ty() const { return slopes().Y(); }
  };
} // namespace LHCb::Pr::detail
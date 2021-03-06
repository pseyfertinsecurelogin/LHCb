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
#include "Event/PrimaryVertexTrack.h"
#include "Event/PrimaryVertex.h"
#include "Event/Track.h"

namespace LHCb {
  PrimaryVertexTrack::PrimaryVertexTrack( const LHCb::Track& track, double weight, double refz ) {
    // compute the id
    m_id = PrimaryVertex::uniqueVeloSegmentID( track );

    // get the state
    const LHCb::State* closestToBeam = track.stateAt( LHCb::State::Location::ClosestToBeam );
    LHCb::State        state         = closestToBeam ? *closestToBeam : track.firstState();
    state.linearTransportTo( refz );

    // fill from state
    m_state  = state.stateVector().Sub<Vector4>( 0 );
    m_weight = weight;
    m_invcov = state.covariance().Sub<Gaudi::SymMatrix2x2>( 0, 0 );
    m_invcov.Invert();

    // initialize the non-persisted stuff
    initCache();
  }

  void PrimaryVertexTrack::initCache() {
    // The following can all be written out, omitting the zeros, once
    // we know that it works.
    ROOT::Math::SMatrix<double, 3, 2> H;
    H( 0, 0 ) = H( 1, 1 ) = 1;
    H( 2, 0 )             = -m_state( 2 );
    H( 2, 1 )             = -m_state( 3 );
    auto res              = m_state.Sub<Vector2>( 0 );
    m_halfD2Chi2DX2       = ROOT::Math::Similarity( H, m_invcov );
    m_halfDChi2DX         = ( H * m_invcov ) * res;
    m_chi2                = ROOT::Math::Similarity( res, m_invcov );
  }
} // namespace LHCb

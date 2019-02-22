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

// Event
#include "Event/MCParticle.h"

// STL
#include <algorithm>

//-----------------------------------------------------------------------------
// Implementation file for class : MCParticle
//
// 2012-11-20 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::MCParticle::fillStream( std::ostream& s ) const {
  s << "{ Momentum = " << momentum() << " ParticleID = " << particleID() << " OriginVertex = " << originVertex()
    << " EndVertices : #=" << endVertices().size() << " [";
  for ( const auto& v : endVertices() ) { s << " " << v.target(); }
  s << " ] }";
  return s;
}

bool LHCb::MCParticle::hasOscillated() const {
  return std::any_of( endVertices().begin(), endVertices().end(),
                      []( const auto& v ) { return v->type() == LHCb::MCVertex::MCVertexType::OscillatedAndDecay; } );
}

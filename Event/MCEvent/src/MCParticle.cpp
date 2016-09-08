
// Event
#include "Event/MCParticle.h"

// STL
#include <algorithm>

//-----------------------------------------------------------------------------
// Implementation file for class : MCParticle
//
// 2012-11-20 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::MCParticle::fillStream(std::ostream& s) const
{
  s << "{ Momentum = "     << momentum()
    << " ParticleID = "    << particleID()
    << " OriginVertex = "  << originVertex()
    << " EndVertices : #=" << endVertices().size() << " [";
  for ( const auto & v : endVertices() ) { s << " " << v.target(); }
  s << " ] }";
  return s;
}

bool LHCb::MCParticle::hasOscillated() const
{
  return std::any_of( endVertices().begin(), endVertices().end(),
                      []( const auto & v )
                      { return v->type() == LHCb::MCVertex::OscillatedAndDecay; } );
}

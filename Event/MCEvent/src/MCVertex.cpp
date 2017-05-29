
// Event
#include "Event/MCVertex.h"

#include <Math/GenVector/PositionVector3D.h>

//-----------------------------------------------------------------------------
// Implementation file for class : MCVertex
//
// 2012-11-20 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::MCVertex::fillStream(std::ostream& s) const
{
  s << "{ Position = "  << position()
    << " Time = "       << time()
    << " Type = "       << type()
    << " Mother = "     << mother()
    << " Products : #=" << products().size() << " [";
  for ( const auto & p : products() ) { s << " " << p.target(); }
  s << " ] }";
  return s;
}

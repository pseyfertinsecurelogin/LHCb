
//-----------------------------------------------------------------------------
/** @file RichSide.cpp
 *
 *  Implementation file for enumeration : Rich::Side
 *
 *  CVS Log :-
 *  $Id: RichSide.cpp,v 1.6 2010-02-16 19:27:20 jonrob Exp $
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2002-06-19
 */
//-----------------------------------------------------------------------------

// local
#include "Kernel/RichSide.h"

// boost
#include "boost/assign/list_of.hpp"

// Text conversion for Rich::Side enumeration
std::string Rich::text( const Rich::Side side )
{
  switch( side )
  {
  case Rich::top:           return "RICH1 top or RICH2 ASide-left"; // Note also covers  Rich::left
  case Rich::bottom:        return "RICH1 bottom or RICH2 CSide-right"; // Note also covers Rich::right
  case Rich::InvalidSide:   return "Invalid side";
  default:                  return "SHOULD NEVER SEE THIS";
  }
}

// Text conversion for Rich::DetectorType and Rich::Side enumeration
std::string Rich::text( const Rich::DetectorType rich, const Rich::Side side )
{
  if ( Rich::Rich1 == rich )
  {
    switch( side )
    {
    case Rich::top:           return "top";
    case Rich::bottom:        return "bottom";
    case Rich::InvalidSide:   return "Invalid side";
    default:                  return "SHOULD NEVER SEE THIS";
    }
  }
  else if ( Rich::Rich2 == rich )
  {
    switch( side )
    {
    case Rich::left:          return "ASide-left";
    case Rich::right:         return "CSide-right";
    case Rich::InvalidSide:   return "Invalid side";
    default:                  return "SHOULD NEVER SEE THIS";
    }
  }
  else
  {
    return Rich::text(side);
  }
}

const Rich::Sides & Rich::sides()
{
  static Rich::Sides s = boost::assign::list_of(Rich::top)(Rich::bottom);
  return s;
}


//-----------------------------------------------------------------------------
/** @file MCRichTrack.cpp
 *
 *  Implementation file for class : MCRichTrack
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    2004-01-22
 */
//-----------------------------------------------------------------------------

// local
#include "Event/MCRichTrack.h"
#include "Event/MCRichSegment.h"

const LHCb::MCRichSegment * 
LHCb::MCRichTrack::segmentInRad( const Rich::RadiatorType rad ) const
{
  const LHCb::MCRichSegment * segment = nullptr;
  for ( const auto & seg : mcSegments() )
  {
    if ( seg.target() && rad == seg->radiator() ) 
    {
      if ( segment ) 
      { std::cout << "WARNING : MCRichTrack has multiple segments for the same radiator !!" << std::endl; }
      segment = seg;
    }
  }
  return segment;
}

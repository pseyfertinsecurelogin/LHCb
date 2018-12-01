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

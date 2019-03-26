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
// Include files

// Event Model
#include "Event/Track.h"

// local
#include "DumpTracks.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DumpTracks
//
// 2004-07-14 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DumpTracks )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DumpTracks::DumpTracks( const std::string& name, ISvcLocator* pSvcLocator ) : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "NumberOfObjectsToPrint", m_numObjects = 5 );
  declareProperty( "TracksLocation", m_tracksLocation = LHCb::TrackLocation::Default );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode DumpTracks::execute() {
  LHCb::Tracks* tracksCont = get<LHCb::Tracks>( m_tracksLocation );

  info() << "There are " << tracksCont->size() << " tracks in " << m_tracksLocation << endmsg;

  counter( "#Tracks" ) += tracksCont->size();

  if ( msgLevel( MSG::DEBUG ) ) {
    unsigned int numPrinted = 0;

    for ( const LHCb::Track* track : *tracksCont ) {
      if ( !msgLevel( MSG::VERBOSE ) && ++numPrinted > m_numObjects ) break;
      debug() << *track << endmsg;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================

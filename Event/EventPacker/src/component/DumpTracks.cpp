// Include files

#include <functional>

// local
#include "DumpTracks.h"
// Event Model
#include "Event/Track.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/StatEntity.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
// Implementation file for class : DumpTracks
//
// 2004-07-14 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY(DumpTracks)

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DumpTracks::DumpTracks( const std::string& name,
                        ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "NumberOfObjectsToPrint", m_numObjects = 5 );
  declareProperty( "TracksLocation", m_tracksLocation = LHCb::TrackLocation::Default );
}


//=============================================================================
// Main execution
//=============================================================================
StatusCode DumpTracks::execute() 
{
  LHCb::Tracks* tracksCont = get<LHCb::Tracks>( m_tracksLocation );

  info() << "There are " << tracksCont->size() << " tracks in "
         << m_tracksLocation << endmsg;

  counter("#Tracks") += tracksCont->size();

  if ( msgLevel( MSG::DEBUG ) ) 
  {
    unsigned int numPrinted = 0;

    for ( const LHCb::Track * track : *tracksCont )
    {
      if ( !msgLevel(MSG::VERBOSE) && ++numPrinted > m_numObjects ) break;
      debug() << *track << endmsg;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================

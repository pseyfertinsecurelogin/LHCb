// $Id: UnpackTrack.cpp,v 1.15 2009-12-12 15:37:20 jonrob Exp $
// Include files

// from Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "Kernel/StandardPacker.h"

// local
#include "UnpackTrack.h"

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackTrack
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( UnpackTrack )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  UnpackTrack::UnpackTrack( const std::string& name,
                            ISvcLocator* pSvcLocator)
    : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName" , m_inputName  = LHCb::PackedTrackLocation::Default );
  declareProperty( "OutputName", m_outputName = LHCb::TrackLocation::Default );
  declareProperty( "AlwaysCreateOutput",      m_alwaysOutput = false     );
  declareProperty( "AncestorFor",             m_ancestorFor    = LHCb::PackedTrackLocation::Muon );
  declareProperty( "AncestorSource",          m_ancestorSource = LHCb::TrackLocation::Default );
}

//=============================================================================
// Destructor
//=============================================================================
UnpackTrack::~UnpackTrack() {}

//=============================================================================
// Main execution
//=============================================================================
StatusCode UnpackTrack::execute()
{

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput && 
       !exist<LHCb::PackedTracks>(m_inputName) ) return StatusCode::SUCCESS;

  // Get the packed tracks
  const LHCb::PackedTracks* dst = 
    getOrCreate<LHCb::PackedTracks,LHCb::PackedTracks>( m_inputName );
  if ( msgLevel(MSG::DEBUG) )
  {
    debug() << "Size of PackedTracks = " << dst->tracks().size() << endmsg;
    if ( dst->ids().size()    > 65535 ) 
      debug() << "Large container of LHCbIDs: " << dst->ids().size()    << endmsg;
    if ( dst->states().size() > 65535 ) 
      debug() << "Large container of States: "  << dst->states().size() << endmsg;
    if ( dst->extras().size() > 65535 ) 
      debug() << "Large container of Extras: "  << dst->extras().size() << endmsg;
  }

  // Make new unpacked tracks
  LHCb::Tracks* newTracks = new LHCb::Tracks();
  put( newTracks, m_outputName );

  /// Track Packer
  const LHCb::TrackPacker packer(*this);

  // Unpack the tracks
  packer.unpack( *dst, *newTracks );

  //== If one needs to build ancestors, get the location of the ancestor's container
  const LHCb::Tracks* targetTracks = ( m_inputName != m_ancestorFor ? NULL :
                                       get<LHCb::Tracks>( m_ancestorSource ) );
  if ( targetTracks )
  {
    for ( LHCb::Tracks::iterator iTk = newTracks->begin(); 
          iTk != newTracks->end(); ++iTk )
    {
      LHCb::Track * track = *iTk;
      //== Create an ancestor if needed
      const LHCb::Track* ancest = targetTracks->object( track->key() );
      if ( ancest )
      {
        track->addToAncestors( ancest );
        if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
          debug() << "Add ancestor for track " << track->key() << endmsg;
      }
    }
  }

  counter("# Unpacked Tracks") += newTracks->size();

  return StatusCode::SUCCESS;
}

//=============================================================================

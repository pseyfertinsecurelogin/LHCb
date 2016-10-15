
// from Gaudi
#include "Event/StandardPacker.h"

// local
#include "UnpackTrackFunctional.h"

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackTrackFunctional
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( UnpackTrackFunctional )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
UnpackTrackFunctional::UnpackTrackFunctional( const std::string& name,
                                              ISvcLocator* pSvcLocator)
: Transformer ( name , pSvcLocator ,
                KeyValue { "InputName" , LHCb::PackedTrackLocation::Default },
                KeyValue { "OutputName", LHCb::TrackLocation::Default } )
{
  //setProperty( "OutputLevel", 1 );
}

//=============================================================================
// Main execution
//=============================================================================
LHCb::Tracks UnpackTrackFunctional::operator()( const LHCb::PackedTracks& dst ) const
{
  // Make new unpacked tracks
  LHCb::Tracks newTracks;

  // Track Packer
  const LHCb::TrackPacker packer(this);

  // Unpack the tracks
  packer.unpack( dst, newTracks );

  counter("# Unpacked Tracks") += newTracks.size();

  return newTracks;
}

//=============================================================================

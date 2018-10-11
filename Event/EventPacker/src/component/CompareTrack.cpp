/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files 

// local
#include "CompareTrack.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareTrack
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CompareTrack )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CompareTrack::CompareTrack( const std::string& name,
                            ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName" , m_inputName  = LHCb::TrackLocation::Default );
  declareProperty( "TestName"  , m_testName   = LHCb::TrackLocation::Default+"Test" );
}


//=============================================================================
// Main execution
//=============================================================================
StatusCode CompareTrack::execute()
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  LHCb::Tracks* old  = get<LHCb::Tracks>( m_inputName  );
  LHCb::Tracks* test = get<LHCb::Tracks>( m_testName  );

  // Track Packer
  const LHCb::TrackPacker packer(this);

  // compare
  return packer.check( *old, *test );
}

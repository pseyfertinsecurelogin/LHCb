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

// local
#include "CompareProtoParticle.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareProtoParticle
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CompareProtoParticle )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CompareProtoParticle::CompareProtoParticle( const std::string& name,
                                            ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName", m_inputName = LHCb::ProtoParticleLocation::Charged );
  declareProperty( "TestName" , m_testName  = LHCb::ProtoParticleLocation::Charged+"Test" );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CompareProtoParticle::execute() 
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;
 
  LHCb::ProtoParticles* old  = get<LHCb::ProtoParticles>( m_inputName  );
  LHCb::ProtoParticles* test = get<LHCb::ProtoParticles>( m_testName  );

  // check and return
  const LHCb::ProtoParticlePacker packer(this);
  return packer.check( *old, *test );
}

//=============================================================================

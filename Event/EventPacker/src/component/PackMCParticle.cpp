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
// STL
#include <algorithm>

#include "Event/MCParticle.h"
#include "Event/StandardPacker.h"
#include "Event/PackedMCParticle.h"
// local
#include "PackMCParticle.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PackMCParticle
//
// 2005-03-18 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( PackMCParticle )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  PackMCParticle::PackMCParticle( const std::string& name,
                                  ISvcLocator* pSvcLocator)
    : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName" , m_inputName  = LHCb::MCParticleLocation::Default );
  declareProperty( "OutputName", m_outputName = LHCb::PackedMCParticleLocation::Default );
  declareProperty( "AlwaysCreateOutput",         m_alwaysOutput = false     );
  declareProperty( "DeleteInput",                m_deleteInput  = false     );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PackMCParticle::execute() {

  // Check to see if the output data already exists.
  if( exist<LHCb::PackedMCParticles>(m_outputName) ) {
    if (msgLevel(MSG::DEBUG) )
      debug() << "Output already exists at '" << m_outputName << "'" << endmsg;
    return StatusCode::SUCCESS;
  }

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput && !exist<LHCb::MCParticles>(m_inputName) ) {
    if (msgLevel(MSG::DEBUG) )
      debug() << "Input does not exist at '" << m_inputName << "'" << endmsg;
    return StatusCode::SUCCESS;
  }

  LHCb::MCParticles* parts = getOrCreate<LHCb::MCParticles,LHCb::MCParticles>( m_inputName );

  if ( msgLevel(MSG::DEBUG) )
    debug() << m_inputName << " contains " << parts->size()
            << " MCParticles to convert." << endmsg;

  StandardPacker pack(this);

  LHCb::PackedMCParticles* out = new LHCb::PackedMCParticles();
  put( out, m_outputName );

  // Packing version
  const char pVer = LHCb::PackedMCParticles::defaultPackingVersion();
  out->setPackingVersion(pVer);

  out->mcParts().reserve( parts->size() );
  for ( const LHCb::MCParticle * part : *parts )
  {
    out->mcParts().emplace_back( LHCb::PackedMCParticle() );
    auto & newPart = out->mcParts().back();

    newPart.key   = part->key();
    newPart.px    = pack.energy( part->momentum().px() );
    newPart.py    = pack.energy( part->momentum().py() );
    newPart.pz    = pack.energy( part->momentum().pz() );
    newPart.mass  = (float)part->virtualMass();
    newPart.PID   = part->particleID().pid();
    newPart.flags = part->flags();
    newPart.originVertex = ( 0==pVer ? 
                             pack.reference32( out,
                                               part->originVertex()->parent(),
                                               part->originVertex()->key() ) :
                             pack.reference64( out,
                                               part->originVertex()->parent(),
                                               part->originVertex()->key() ) );
    for ( const auto & V : part->endVertices() )
    {
      newPart.endVertices.push_back( 0==pVer ? 
                                     pack.reference32( out, V->parent(), V->key() ) :
                                     pack.reference64( out, V->parent(), V->key() ) );
    }

  }

  // If requested, remove the input data from the TES and delete
  if ( UNLIKELY(m_deleteInput) )
  {
    const StatusCode sc = evtSvc()->unregisterObject( parts );
    if ( sc.isSuccess() )
    {
      delete parts;
      parts = nullptr;
    }
    else
    {
      return Error("Failed to delete input data as requested", sc );
    }
  }
  else
  {
    // Clear the registry address of the unpacked container, to prevent reloading
    parts->registry()->setAddress( 0 );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================

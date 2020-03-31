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
/** @file ChargedProtoParticleAddRichInfo.cpp
 *
 * Implementation file for algorithm ChargedProtoParticleAddRichInfo
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 28/08/2009
 */
//-----------------------------------------------------------------------------

// local
#include "ChargedProtoParticleAddRichInfo.h"

// namespaces
using namespace LHCb;

//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ChargedProtoParticleAddRichInfo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ChargedProtoParticleAddRichInfo::ChargedProtoParticleAddRichInfo( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {}

//=============================================================================
// Main execution
//=============================================================================
StatusCode ChargedProtoParticleAddRichInfo::execute() {
  // ProtoParticle container
  LHCb::ProtoParticles* protos = m_protos.get();

  // Load the RichPIDs
  auto rich_data = getRichData();

  // Loop over proto particles and add RICH info
  for ( auto* proto : *protos ) { updateRICH( proto, rich_data ); }

  // return
  return StatusCode::SUCCESS;
}

//=============================================================================
// Add RICH info to the protoparticle
//=============================================================================
void ChargedProtoParticleAddRichInfo::updateRICH( ProtoParticle* proto, TrackToRichPID const& richMap ) const {
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Trying ProtoParticle " << proto->key() << endmsg;

  // Erase current RichPID information
  proto->removeRichInfo();

  // Does this track have a RICH PID result ?
  TrackToRichPID::const_iterator iR = richMap.find( proto->track() );
  if ( richMap.end() == iR ) {
    if ( msgLevel( MSG::VERBOSE ) ) verbose() << " -> NO associated RichPID object found" << endmsg;
    return;
  }

  // RichPID for this track is found, so save data
  const LHCb::RichPID* richPID = ( *iR ).second;

  // RichPID for this track is found, so save data
  if ( msgLevel( MSG::VERBOSE ) )
    verbose() << " -> Found RichPID data : DLLs = " << richPID->particleLLValues() << endmsg;

  // reference to RichPID object
  proto->setRichPID( richPID );

  // Store the raw RICH PID info
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::RichDLLe, richPID->particleDeltaLL( Rich::Electron ) );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::RichDLLmu, richPID->particleDeltaLL( Rich::Muon ) );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::RichDLLpi, richPID->particleDeltaLL( Rich::Pion ) );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::RichDLLk, richPID->particleDeltaLL( Rich::Kaon ) );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::RichDLLp, richPID->particleDeltaLL( Rich::Proton ) );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::RichDLLbt, richPID->particleDeltaLL( Rich::BelowThreshold ) );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::RichDLLd, richPID->particleDeltaLL( Rich::Deuteron ) );

  // Store History
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::RichPIDStatus, richPID->pidResultCode() );
}

//=============================================================================
// Loads the RICH data
//=============================================================================
ChargedProtoParticleAddRichInfo::TrackToRichPID ChargedProtoParticleAddRichInfo::getRichData() const {
  // empty the map
  TrackToRichPID richMap;

  // Do we have any RichPID results
  auto const* richpids = m_richpids.get();
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Successfully loaded " << richpids->size() << " RichPIDs from " << m_richpids.objKey() << " Version "
            << (unsigned int)richpids->version() << endmsg;

  // refresh the reverse mapping
  for ( const LHCb::RichPID* pid : *richpids ) {
    if ( pid->track() ) {
      richMap[pid->track()] = pid;
      if ( msgLevel( MSG::VERBOSE ) )
        verbose() << "RichPID key=" << pid->key() << " has Track key=" << pid->track()->key() << " " << pid->track()
                  << endmsg;
    } else {
      std::ostringstream mess;
      mess << "RichPID key=" << pid->key() << " has NULL Track pointer";
      Warning( mess.str() ).ignore();
    }
  }

  return richMap;
}

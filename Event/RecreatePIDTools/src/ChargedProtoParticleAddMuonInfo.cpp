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
/** @file ChargedProtoParticleAddMuonInfo.cpp
 *
 * Implementation file for algorithm ChargedProtoParticleAddMuonInfo
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 28/08/2009
 */
//-----------------------------------------------------------------------------

// local
#include "ChargedProtoParticleAddMuonInfo.h"

//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ChargedProtoParticleAddMuonInfo )

//=============================================================================
// Main execution
//=============================================================================
StatusCode ChargedProtoParticleAddMuonInfo::execute() {
  // ProtoParticle container
  LHCb::ProtoParticles* protos = m_protos.get();

  // Load the MuonPIDs
  auto muonMap = getMuonData();

  // Loop over proto particles and update muon info
  for ( auto* proto : *protos ) { updateMuon( proto, muonMap ); }

  // return
  return StatusCode::SUCCESS;
}

//=============================================================================
// Replace MUON info to the protoparticle
//=============================================================================
void ChargedProtoParticleAddMuonInfo::updateMuon( LHCb::ProtoParticle* proto, TrackToMuonPID const& muonMap ) const {
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Trying ProtoParticle " << proto->key() << endmsg;

  // Erase current MuonPID information
  proto->removeMuonInfo();

  // Does this track have a MUON PID result ?
  TrackToMuonPID::const_iterator iM = muonMap.find( proto->track() );
  if ( muonMap.end() == iM ) {
    if ( msgLevel( MSG::VERBOSE ) ) verbose() << " -> NO associated MuonPID object found" << endmsg;
    return;
  }

  // MuonPID object is found
  const LHCb::MuonPID* muonPID = ( *iM ).second;

  // MuonPID for this track is found, so save data
  if ( msgLevel( MSG::VERBOSE ) )
    verbose() << " -> Found MuonPID data : MuLL=" << muonPID->MuonLLMu() << " BkLL=" << muonPID->MuonLLBg()
              << " nSharedHits=" << muonPID->nShared() << " isMuonLoose=" << muonPID->IsMuonLoose()
              << " isMuon=" << muonPID->IsMuon() << endmsg;

  // Store History
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::MuonPIDStatus, muonPID->Status() );

  // store acceptance flag for those in acceptance (lack of flag signifies
  // track was outside acceptance)
  if ( muonPID->InAcceptance() ) { proto->addInfo( LHCb::ProtoParticle::additionalInfo::InAccMuon, true ); }

  // reference to MuonPID object
  proto->setMuonPID( muonPID );

  // check Loose IsMuonLoose flag - Reject non-muons
  if ( !muonPID->IsMuonLoose() ) {
    if ( msgLevel( MSG::VERBOSE ) ) { verbose() << " -> Rejected by IsMuonLoose cut" << endmsg; }
    return;
  }

  // Store the PID info
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::MuonMuLL, muonPID->MuonLLMu() );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::MuonBkgLL, muonPID->MuonLLBg() );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::MuonNShared, muonPID->nShared() );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::MuonChi2Corr, muonPID->chi2Corr() );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::MuonMVA1, muonPID->muonMVA1() );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::MuonMVA2, muonPID->muonMVA2() );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::MuonMVA3, muonPID->muonMVA3() );
  proto->addInfo( LHCb::ProtoParticle::additionalInfo::MuonMVA4, muonPID->muonMVA4() );

  // print full ProtoParticle content
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << *proto << endmsg;
}

//=============================================================================
// Loads the Muon data
//=============================================================================
ChargedProtoParticleAddMuonInfo::TrackToMuonPID ChargedProtoParticleAddMuonInfo::getMuonData() const {
  TrackToMuonPID muonMap{};
  // Do we have any MuonPID results
  const LHCb::MuonPIDs* muonpids = m_muonPath.get();

  // refresh the reverse mapping
  for ( const auto* M : *muonpids ) {
    if ( M->idTrack() ) {
      muonMap[M->idTrack()] = M;
      if ( msgLevel( MSG::VERBOSE ) )
        verbose() << "MuonPID key=" << M->key() << " has Track key=" << M->idTrack()->key() << " " << M->idTrack()
                  << endmsg;
    }
  }

  return muonMap;
}

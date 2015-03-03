
//-----------------------------------------------------------------------------
/** @file ChargedProtoParticleAddMuonInfo.cpp
 *
 * Implementation file for algorithm ChargedProtoParticleAddMuonInfo
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 28/08/2009
 */
//-----------------------------------------------------------------------------

// from Gaudi
#include "GaudiKernel/AlgFactory.h"

// local
#include "ChargedProtoParticleAddMuonInfo.h"

//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( ChargedProtoParticleAddMuonInfo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  ChargedProtoParticleAddMuonInfo::
ChargedProtoParticleAddMuonInfo( const std::string& name,
                                 ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{

  // context specific locations
  if ( context() == "HLT" || context() == "Hlt" )
  {
    m_muonPath   = LHCb::MuonPIDLocation::Hlt;
    m_protoPath  = LHCb::ProtoParticleLocation::HltCharged;
  }
  else
  {
    m_muonPath   = LHCb::MuonPIDLocation::Offline;
    m_protoPath  = LHCb::ProtoParticleLocation::Charged;
  }

  // Muon data
  declareProperty( "InputMuonPIDLocation", m_muonPath );

  // ProtoParticles
  declareProperty( "ProtoParticleLocation", m_protoPath );

}

//=============================================================================
// Destructor
//=============================================================================
ChargedProtoParticleAddMuonInfo::~ChargedProtoParticleAddMuonInfo() { }

//=============================================================================
// Main execution
//=============================================================================
StatusCode ChargedProtoParticleAddMuonInfo::execute()
{
  // ProtoParticle container
  if ( !exist<LHCb::ProtoParticles>(m_protoPath) )
  {
    return Warning( "No existing ProtoParticle container at " +  m_protoPath + " thus do nothing.",
                    StatusCode::SUCCESS );
  }
  LHCb::ProtoParticles * protos = get<LHCb::ProtoParticles>(m_protoPath);

  // Load the MuonPIDs
  const bool muonSc = getMuonData();
  if ( !muonSc ) { return StatusCode::SUCCESS; }

  // Loop over proto particles
  for ( LHCb::ProtoParticles::iterator iProto = protos->begin();
        iProto != protos->end(); ++iProto )
  {
    // replace the muon information
    updateMuon(*iProto);
  }
  counter(m_muonPath + " ==> " + m_protoPath )+= protos->size();

  return StatusCode::SUCCESS;
}

//=============================================================================
// Replace MUON info to the protoparticle
//=============================================================================
void ChargedProtoParticleAddMuonInfo::updateMuon( LHCb::ProtoParticle * proto ) const
{
  if ( msgLevel(MSG::VERBOSE) )
    verbose() << "Trying ProtoParticle " << proto->key() << endmsg;

  // Erase current MuonPID information
  proto->removeMuonInfo();

  // Does this track have a MUON PID result ?
  TrackToMuonPID::const_iterator iM = m_muonMap.find( proto->track() );
  if ( m_muonMap.end() == iM )
  {
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << " -> NO associated MuonPID object found" << endmsg;
    return;
  }

  // MuonPID object is found
  const LHCb::MuonPID * muonPID = (*iM).second;

  // MuonPID for this track is found, so save data
  if ( msgLevel(MSG::VERBOSE) )
    verbose() << " -> Found MuonPID data : MuLL=" <<  muonPID->MuonLLMu()
              << " BkLL=" <<  muonPID->MuonLLBg()
              << " nSharedHits=" << muonPID->nShared()
              << " isMuonLoose=" << muonPID->IsMuonLoose()
              << " isMuon=" << muonPID->IsMuon()
              << endmsg;

  // Store History
  proto->addInfo( LHCb::ProtoParticle::MuonPIDStatus, muonPID->Status() );

  // store acceptance flag for those in acceptance (lack of flag signifies
  // track was outside acceptance)
  if ( muonPID->InAcceptance() )
  {
    proto->addInfo( LHCb::ProtoParticle::InAccMuon, true );
  }

  // reference to MuonPID object
  proto->setMuonPID( muonPID );

  // check Loose IsMuonLoose flag - Reject non-muons
  if ( !muonPID->IsMuonLoose() )
  {
    if ( msgLevel(MSG::VERBOSE) )
    {
      verbose() << " -> Rejected by IsMuonLoose cut" << endmsg;
    }
    return;
  }

  // Store the PID info
  proto->addInfo( LHCb::ProtoParticle::MuonMuLL,      muonPID->MuonLLMu() );
  proto->addInfo( LHCb::ProtoParticle::MuonBkgLL,     muonPID->MuonLLBg() );
  proto->addInfo( LHCb::ProtoParticle::MuonNShared,   muonPID->nShared()  );

  // print full ProtoParticle content
  if ( msgLevel(MSG::VERBOSE) ) verbose() << *proto << endmsg;

}

//=============================================================================
// Loads the Muon data
//=============================================================================
bool ChargedProtoParticleAddMuonInfo::getMuonData()
{
  // empty the map
  m_muonMap.clear();

  // Do we have any MuonPID results
  if ( !exist<LHCb::MuonPIDs>(m_muonPath) )
  {
    Warning( "No MuonPIDs at '" + m_muonPath +
             "' -> ProtoParticles will not be changed.", StatusCode::SUCCESS, 1 ).ignore();
    return false;
  }

  // yes, so load them
  const LHCb::MuonPIDs * muonpids = get<LHCb::MuonPIDs>( m_muonPath );
  if ( msgLevel(MSG::DEBUG) )
    debug() << "Successfully loaded " << muonpids->size()
            << " MuonPIDs from " << m_muonPath << endmsg;

  // Pointer for bug work around below, if needed.
  const LHCb::ProtoParticles * protos = NULL;

  // refresh the reverse mapping
  for ( LHCb::MuonPIDs::const_iterator iM = muonpids->begin();
        iM != muonpids->end(); ++iM )
  {
    if ( (*iM)->idTrack() )
    {
      m_muonMap[ (*iM)->idTrack() ] = *iM;
      if ( msgLevel(MSG::VERBOSE) )
        verbose() << "MuonPID key=" << (*iM)->key()
                  << " has Track key=" << (*iM)->idTrack()->key()
                  << " " << (*iM)->idTrack() << endmsg;
    }
    else
    {
      std::ostringstream mess;
      mess << "MuonPID key=" << (*iM)->key() << " has NULL Track pointer. "
           << "Will try and work around using track keys ...";
      Warning( mess.str() ).ignore();

      // Bug in old (u)DSTs. Try and work around using track keys ...
      // Eventually to be removed
      if ( !protos ) protos = get<LHCb::ProtoParticles>(m_protoPath);
      for ( LHCb::ProtoParticles::const_iterator iProto = protos->begin();
            iProto != protos->end(); ++iProto )
      {
        if ( (*iProto)->track() &&
             (*iProto)->track()->key() == (*iM)->key() )
        {
          m_muonMap[ (*iProto)->track() ] = *iM;
          if ( msgLevel(MSG::VERBOSE) )
            verbose() << "MuonPID key=" << (*iM)->key()
                      << " has Track key=" << (*iProto)->track()->key()
                      << " " << (*iProto)->track() << endmsg;
          break;
        }
      }

    }
  }

  return true;
}

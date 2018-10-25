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
//-----------------------------------------------------------------------------
/** @file ChargedProtoCombineDLLsAlg.cpp
 *
 * Implementation file for algorithm ChargedProtoCombineDLLsAlg
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 15/11/2006
 */
//-----------------------------------------------------------------------------

// local
#include "ChargedProtoCombineDLLsAlg.h"

//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ChargedProtoCombineDLLsAlg )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ChargedProtoCombineDLLsAlg::ChargedProtoCombineDLLsAlg( const std::string& name,
                                                        ISvcLocator* pSvcLocator )
  : GaudiAlgorithm ( name , pSvcLocator )
  , m_elCombDll(0xFFFF)
  , m_muCombDll(0xFFFF)
  , m_prCombDll(0xFFFF)
  , m_piCombDll(0xFFFF)
  , m_kaCombDll(0xFFFF)
  , m_deCombDll(0xFFFF)
{

  // context specific locations
  if ( context() == "HLT" || context() == "Hlt" )
  {
    m_protoPath = LHCb::ProtoParticleLocation::HltCharged;
  }
  else
  {
    m_protoPath = LHCb::ProtoParticleLocation::Charged;
  }

  // Job Options
  declareProperty( "ProtoParticleLocation", m_protoPath );

  declareProperty("ElectronDllDisable"  , m_elDisable);
  declareProperty("MuonDllDisable"      , m_muDisable);
  declareProperty("KaonDllDisable"      , m_kaDisable);
  declareProperty("ProtonDllDisable"    , m_piDisable);
  declareProperty("PionllDisable"       , m_prDisable);
  declareProperty("DeuteronDllDisable"  , m_deDisable);

  m_maskTechnique["RICH"] = 0x1;
  m_maskTechnique["MUON"] = 0x2;
  m_maskTechnique["ECAL"] = 0x4;
  m_maskTechnique["HCAL"] = 0x8;
  m_maskTechnique["PRS"]  = 0x10;
  m_maskTechnique["SPD"]  = 0x20;
  m_maskTechnique["BREM"] = 0x40;
  m_maskTechnique["CALO"] = 0x7C;
}

//=============================================================================
// Destructor
//=============================================================================
ChargedProtoCombineDLLsAlg::~ChargedProtoCombineDLLsAlg() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode ChargedProtoCombineDLLsAlg::initialize()
{
  StatusCode scc = GaudiAlgorithm::initialize();
  if ( scc.isFailure() ) return scc;

  for ( const auto& itech : m_elDisable )
  {
    if ( 0 == m_maskTechnique[to_upper(itech)] )
    {
      error() << "Electron PID technique " << itech << " unknown"<< endmsg;
      scc = StatusCode::FAILURE;
    }
    m_elCombDll &= ~m_maskTechnique[to_upper(itech)] ;
  }
  for ( const auto& itech : m_muDisable )
  {
    if ( 0 == m_maskTechnique[to_upper(itech)] )
    {
      error() << "Muon PID technique " << itech << " unknown"<< endmsg;
      scc = StatusCode::FAILURE;
    }
    m_muCombDll &= ~m_maskTechnique[to_upper(itech)] ;
  }
  for ( const auto& itech : m_prDisable )
  {
    if ( 0 == m_maskTechnique[to_upper(itech)] )
    {
      error() << "Proton PID technique " << itech << " unknown"<< endmsg;
      scc = StatusCode::FAILURE;
    }
    m_prCombDll &= ~m_maskTechnique[to_upper(itech)] ;
  }
  for ( const auto& itech : m_piDisable )
  {
    if ( 0 == m_maskTechnique[to_upper(itech)] )
    {
      error() << "Pion PID technique " << itech << " unknown"<< endmsg;
      scc = StatusCode::FAILURE;
    }
    m_piCombDll &= ~m_maskTechnique[to_upper(itech)] ;
  }
  for ( const auto& itech : m_kaDisable )
  {
    if ( 0 == m_maskTechnique[to_upper(itech)] )
    {
      error() << "Kaon PID technique " << itech << " unknown"<< endmsg;
      scc = StatusCode::FAILURE;
    }
    m_kaCombDll &= ~m_maskTechnique[to_upper(itech)] ;
  }

  info() << "Using retuned RICH el and mu DLL values in combined DLLs" << endmsg;
  if ( 0 == m_elCombDll )
    Warning( "Not creating Combined DLL for electron hypothesis", StatusCode::SUCCESS ).ignore();
  if ( 0 == m_muCombDll )
    Warning( "Not creating Combined DLL for muon hypothesis", StatusCode::SUCCESS ).ignore();
  if ( 0 == m_piCombDll )
    Warning( "Not creating Combined DLL for pion hypothesis", StatusCode::SUCCESS ).ignore();
  if ( 0 == m_kaCombDll )
    Warning( "Not creating Combined DLL for kaon hypothesis", StatusCode::SUCCESS ).ignore();
  if ( 0 == m_prCombDll )
    Warning( "Not creating Combined DLL for proton hypothesis", StatusCode::SUCCESS ).ignore();
  if ( 0 == m_deCombDll )
    Warning( "Not creating Combined DLL for deuteron hypothesis", StatusCode::SUCCESS ).ignore();

  return scc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode ChargedProtoCombineDLLsAlg::execute()
{

  // Load the charged ProtoParticles
  LHCb::ProtoParticles * protos = getIfExists<LHCb::ProtoParticles>( m_protoPath );

  // If ProtoParticles do not exist, exit cleanly with a warning
  if ( !protos )
  {
    return Warning( "ProtoParticles do not exist at '" + m_protoPath + "'",
                    StatusCode::SUCCESS );
  }

  // Loop over the protos
  for ( auto * proto : *protos )
  {
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << "Creating Combined DLLs for ProtoParticle " << proto->key()
                << endmsg;

    // Remove any current Combined DLL information
    proto->removeCombinedInfo();

    bool hasPIDinfo(false);

    // Combined DLL data object for this proto
    CombinedLL combDLL(0);

    // Add any RICH info
    hasPIDinfo |= addRich(proto,combDLL);

    // Add any MUON info
    hasPIDinfo |= addMuon(proto,combDLL);

    // Add any CALO info
    hasPIDinfo |= addCalo(proto,combDLL);

    if ( hasPIDinfo )
    {
      // Store the final combined DLL values into the ProtoParticle
      proto->addInfo( LHCb::ProtoParticle::additionalInfo::CombDLLe,  combDLL.elDLL-combDLL.piDLL );
      proto->addInfo( LHCb::ProtoParticle::additionalInfo::CombDLLmu, combDLL.muDLL-combDLL.piDLL );
      proto->addInfo( LHCb::ProtoParticle::additionalInfo::CombDLLpi, 0 ); // by definition
      proto->addInfo( LHCb::ProtoParticle::additionalInfo::CombDLLk,  combDLL.kaDLL-combDLL.piDLL );
      proto->addInfo( LHCb::ProtoParticle::additionalInfo::CombDLLp,  combDLL.prDLL-combDLL.piDLL );
      proto->addInfo( LHCb::ProtoParticle::additionalInfo::CombDLLd,  combDLL.deDLL-combDLL.piDLL );
    }
    else
    {
      proto->addInfo( LHCb::ProtoParticle::additionalInfo::NoPID, 1 );
    }

  } // loop over protos
  counter("CombineDLL ==> " + m_protoPath ) += protos->size();

  return StatusCode::SUCCESS;
}

bool ChargedProtoCombineDLLsAlg::addRich( LHCb::ProtoParticle * proto,
                                          CombinedLL & combDLL )
{
  // Add RICH Dll information.
  bool ok = false;
  if ( proto->hasInfo(LHCb::ProtoParticle::additionalInfo::RichPIDStatus) )
  {
    ok = true;
    // Apply renormalisation of RICH el and mu DLL values
    // Eventually, should make these tunable job options ....
    const int rTechnique = m_maskTechnique["RICH"];
    if( 0 != (m_elCombDll & rTechnique) )
      combDLL.elDLL += 7.0*tanh( proto->info(LHCb::ProtoParticle::additionalInfo::RichDLLe,0)/40.0  );
    if( 0 != (m_muCombDll & rTechnique) )
      combDLL.muDLL += 7.0*tanh( proto->info(LHCb::ProtoParticle::additionalInfo::RichDLLmu,0)/5.0 );
    if( 0 != (m_piCombDll & rTechnique) )
      combDLL.piDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::RichDLLpi , 0 );
    if( 0 != (m_kaCombDll & rTechnique) )
      combDLL.kaDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::RichDLLk  , 0 );
    if( 0 != (m_prCombDll & rTechnique) )
      combDLL.prDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::RichDLLp  , 0 );
    if( 0 != (m_deCombDll & rTechnique) )
      combDLL.deDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::RichDLLd  , 0 );
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << " -> Adding RICH info " << combDLL << endmsg;
  }
  return ok;
}

bool ChargedProtoCombineDLLsAlg::addMuon( LHCb::ProtoParticle * proto,
                                          CombinedLL & combDLL )
{

  bool ok = false;
  if (  proto->hasInfo(LHCb::ProtoParticle::additionalInfo::MuonPIDStatus) )
  {
    ok = true;
    const int mTechnique = m_maskTechnique["MUON"];
    if( 0 != (m_elCombDll  & mTechnique))
      combDLL.elDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::MuonBkgLL,0 );
    if( 0 != (m_muCombDll  & mTechnique))
      combDLL.muDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::MuonMuLL, 0 );
    if( 0 != (m_piCombDll  & mTechnique))
      combDLL.piDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::MuonBkgLL,0 );
    if( 0 != (m_kaCombDll  & mTechnique))
      combDLL.kaDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::MuonBkgLL,0 );
    if( 0 != (m_prCombDll  & mTechnique))
      combDLL.prDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::MuonBkgLL,0 );
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << " -> Adding MUON info " << combDLL << endmsg;
  }
  return ok;
}

bool ChargedProtoCombineDLLsAlg::addCalo( LHCb::ProtoParticle * proto,
                                          CombinedLL & combDLL )
{
  bool ok = false;
  if ( proto->hasInfo(LHCb::ProtoParticle::additionalInfo::EcalPIDe) ||
       proto->hasInfo(LHCb::ProtoParticle::additionalInfo::EcalPIDmu) )
  {
    const int eTechnique = m_maskTechnique["ECAL"];
    if( 0 != (m_elCombDll & eTechnique))
      combDLL.elDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::EcalPIDe, 0  );
    if( 0 != (m_muCombDll & eTechnique))
      combDLL.muDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::EcalPIDmu, 0 );
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << " -> Adding ECAL info " << combDLL << endmsg;
    ok = true;
  }

  if ( proto->hasInfo(LHCb::ProtoParticle::additionalInfo::HcalPIDe) ||
       proto->hasInfo(LHCb::ProtoParticle::additionalInfo::HcalPIDmu) )
  {
    const int hTechnique =  m_maskTechnique["HCAL"];
    if( 0 != (m_elCombDll & hTechnique))
      combDLL.elDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::HcalPIDe, 0  );
    if( 0 != (m_muCombDll & hTechnique))
      combDLL.muDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::HcalPIDmu, 0 );
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << " -> Adding HCAL info " << combDLL << endmsg;
    ok = true;
  }

  if ( proto->hasInfo(LHCb::ProtoParticle::additionalInfo::PrsPIDe) )
  {
    const int pTechnique = m_maskTechnique["PRS"];
    if( 0 != (m_elCombDll & pTechnique))
      combDLL.elDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::PrsPIDe , 0  );
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << " -> Adding PRS  info " << combDLL << endmsg;
    ok = true;
  }

  if ( proto->hasInfo(LHCb::ProtoParticle::additionalInfo::BremPIDe) )
  {
    const int bTechnique = m_maskTechnique["BREM"] ;
    if( 0 != (m_elCombDll & bTechnique) )
      combDLL.elDLL += proto->info ( LHCb::ProtoParticle::additionalInfo::BremPIDe, 0  );
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << " -> Adding BREM info " << combDLL << endmsg;
    ok = true;
  }
  return ok;
}

//=============================================================================

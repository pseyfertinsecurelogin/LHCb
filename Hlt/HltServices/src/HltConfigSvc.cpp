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

#include <algorithm>

// from Gaudi
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/System.h"

// Event
#include "Event/HltDecReports.h"
#include "Event/ODIN.h"

#include "Kernel/PlatformInfo.h"

// Detector
#include <DetDesc/Condition.h>

// local
#include "HltConfigSvc.h"

// TODO: check for IEEE 1003.1 compliance instead of Linux...
#include <netdb.h>
#include <unistd.h>

static const ConfigTreeNodeAlias::alias_type TCK_{std::string( "TCK/" )};

//-----------------------------------------------------------------------------
// Implementation file for class : HltConfigSvc
//
// 2007-10-24 : Gerhard Raven
//-----------------------------------------------------------------------------

// Declaration of the Service Factory
DECLARE_COMPONENT( HltConfigSvc )

//=============================================================================
// Finalization
//=============================================================================
StatusCode HltConfigSvc::finalize() {
  m_evtSvc.reset();
  m_incidentSvc.reset();
  return PropertyConfigSvc::finalize();
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode HltConfigSvc::initialize() {
  StatusCode status = PropertyConfigSvc::initialize();
  if ( !status.isSuccess() ) return status;
  m_evtSvc = service( "EventDataSvc" );
  if ( !m_evtSvc ) return StatusCode::FAILURE;
  if ( !m_decodeOdin.retrieve().isSuccess() ) return StatusCode::FAILURE;

  if ( !m_checkOdin && !m_hlt2Mode ) {
    warning() << "Running with CheckOdin off and not in Hlt2 mode. "
              << "The HLT will blindly assume that the initial TCK is correct." << endmsg;
  }

  m_incidentSvc = service( "IncidentSvc" );
  if ( !m_incidentSvc ) return StatusCode::FAILURE;
  // add listener to be triggered by first BeginEvent with low priority
  // so it gets called first
  bool rethrow = false;
  bool oneShot = false;
  m_incidentSvc->addListener( this, IncidentType::BeginEvent, std::numeric_limits<long>::min(), rethrow, oneShot );

  // If in HLT2 only mode, register the condition we use to get the TCK.q
  if ( m_hlt2Mode ) {
    SmartIF<IUpdateManagerSvc> updateSvc = service<IUpdateManagerSvc>( "UpdateManagerSvc" );
    if ( !updateSvc ) return StatusCode::FAILURE;
    updateSvc->registerCondition<HltConfigSvc>( this, m_tckConditionPath, &HltConfigSvc::updateTCK, m_tckCondition );
  }

  // load the complete TCK -> ID mapping... (brute force, but OK for now...)
  for ( const auto& tck : cas()->configTreeNodeAliases( TCK_ ) ) tck2id( TCK{tck.alias().str().substr( 4 )} );

  // find the ID of the initial TCK
  auto initialID = tck2id( m_initialTCK );

  // configure everyone from the a-priori specified TCK
  status = configure( initialID, false );
  if ( status.isSuccess() ) m_configuredTCK = m_initialTCK;
  return status;
}

//=============================================================================
StatusCode HltConfigSvc::start() {
  StatusCode sc = PropertyConfigSvc::start();
  if ( !sc.isSuccess() ) return sc;

  // make sure PlatformInfo instances report the right instructions set level
  // (it may not be the same as during application start if we use checkpointing)
  LHCb::PlatformInfo::s_hostInstrSetLevel = static_cast<std::uint16_t>( System::instructionsetLevel() );

  LHCb::PlatformInfo platform;
  info() << "binaryId=" << platform.binaryId() << " binaryTag=" << platform.binaryTag() << endmsg;
  info() << "hostInstrSetLevel=" << platform.hostInstrSetLevel() << " hostInstrSet=" << platform.hostInstrSet()
         << endmsg;
  m_id = ( platform.binaryId() & 0xffff ) | ( platform.hostInstrSetLevel() << 16 );

  return sc;
}

//=============================================================================
// Perform mapping from TCK to onfiguration ID
//=============================================================================
ConfigTreeNode::digest_type HltConfigSvc::tck2id( TCK tck ) const {
  tck.normalize();
  if ( m_maskL0TCK ) {
    tck.maskL0();
    debug() << " masked L0 part of TCK -- now have " << tck << endmsg;
  }
  auto id = ConfigTreeNode::digest_type::createInvalid();
  auto i  = m_tck2config.find( tck );
  if ( i != m_tck2config.end() ) {
    id = ConfigTreeNode::digest_type::createFromStringRep( i->second );
    warning() << " TCK " << tck << " mapped (by explicit option) to " << id << endmsg;
    return id;
  }

  // NOTE: we need to access to the IConfigAccessSvc of the parent to make
  // sure we are consistent...

  i = m_tck2configCache.find( tck );
  if ( i != m_tck2configCache.end() ) {
    id = ConfigTreeNode::digest_type::createFromStringRep( i->second );
  } else {
    ConfigTreeNodeAlias::alias_type alias{std::string( "TCK/" ) + tck.str()};
    auto                            n = cas()->readConfigTreeNodeAlias( alias );
    if ( !n ) {
      error() << "Could not resolve TCK " << tck << " : no alias '" << alias << "' found " << endmsg;
      return id;
    }
    id = n->digest(); // need a digest, not an object itself...
    // add to cache...
    m_tck2configCache.emplace( tck, id.str() );
  }
  debug() << "mapping TCK" << tck << " to configuration ID" << id << endmsg;
  return id;
}

//=============================================================================
StatusCode HltConfigSvc::updateTCK() {
  unsigned int tck = 0;
  if ( m_hlt2Mode ) {
    // Check condition
    tck = m_tckCondition->paramAsInt( "TCK" );
  } else {
    // Check ODIN
    SmartDataPtr<LHCb::ODIN> odin( m_evtSvc, LHCb::ODINLocation::Default );
    if ( !odin ) m_decodeOdin->execute();
    if ( !odin ) {
      error() << " Could not locate ODIN... " << endmsg;
      m_incidentSvc->fireIncident( Incident( name(), IncidentType::AbortEvent ) );
      throw GaudiException( "No ODIN present??", "", StatusCode::FAILURE );
    }
    tck = odin->triggerConfigurationKey();
  }

  if ( m_configuredTCK != tck ) {
    TCK TCKr{tck};
    info() << "requesting configuration update from TCK " << m_configuredTCK << " to TCK " << TCKr << endmsg;
    StatusCode sc = reconfigure( tck2id( TCKr ) );
    if ( sc.isSuccess() ) {
      m_configuredTCK = TCKr;
    } else {
      error() << "\n\n\n\n\n"
              << "            ****************************************\n"
              << "            ****************************************\n"
              << "            ****************************************\n"
              << "            ********                        ********\n"
              << "            ********   RECONFIGURE FAILED   ********\n"
              << "            ********                        ********\n"
              << "            ****************************************\n"
              << "            ****************************************\n"
              << "            ****************************************\n"
              << "\n\n\n\n\n"
              << endmsg;
      m_incidentSvc->fireIncident( Incident( name(), IncidentType::AbortEvent ) );
      throw GaudiException( "Reconfigure failed", "", sc );
    }
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
void HltConfigSvc::createHltDecReports() {
  for ( const auto& location : m_outputContainerName ) {
    auto hdr = std::make_unique<LHCb::HltDecReports>();
    hdr->setConfiguredTCK( m_configuredTCK.uint() );
    hdr->setTaskID( m_id );
    m_evtSvc->registerObject( location, hdr.release() );
  }
}

//=============================================================================
void HltConfigSvc::handle( const Incident& /*incident*/ ) {
  if ( m_checkOdin ) updateTCK().ignore();
  createHltDecReports();
}

// ============================================================================
// Gaudi
// ============================================================================
#include "GaudiKernel/ParsersFactory.h"
// ============================================================================
namespace Gaudi::Parsers {

  StatusCode parse( std::map<TCK, std::string>& result, const std::string& input ) {
    result.clear();
    std::map<std::string, std::string> m;
    auto                               sc = parse( m, input );
    if ( sc ) {
      std::transform( std::begin( m ), std::end( m ), std::inserter( result, std::end( result ) ),
                      []( const std::pair<std::string, std::string>& i ) {
                        return std::pair{TCK{i.first}, i.second};
                      } );
    }
    return sc;
  }
} // namespace Gaudi::Parsers

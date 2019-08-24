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
// event model
#include "Event/CrossSectionsFSR.h"
#include "Event/GenCountersFSR.h"
#include "Event/GenFSR.h"

// local
#include "FSRNavigator.h"
#include "GenFSRMerge.h"

//-----------------------------------------------------------------------------
// Implementation file for class : GenFSRMerge
//
// 2015-06-25 : Davide Fazzini
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( GenFSRMerge )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================

GenFSRMerge::GenFSRMerge( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator )
    , m_fileRecordSvc( nullptr )
    , m_navigatorTool( nullptr )
    , m_evtType( 0 )
    , m_genName( "" )
    , m_method( "" ) {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode GenFSRMerge::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first

  if ( sc.isFailure() ) return sc; // error printed already by GaudiAlgorithm
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // get the File Records service
  m_fileRecordSvc = Gaudi::svcLocator()->service( "FileRecordDataSvc" );

  // prepare navigator tool
  m_navigatorTool = tool<IFSRNavigator>( "FSRNavigator", "FSRNavigator" );

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode GenFSRMerge::execute() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  return StatusCode::SUCCESS;
}
//=============================================================================

//=============================================================================
//  Finalize
//=============================================================================
StatusCode GenFSRMerge::finalize() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;
  info() << "========== Merging GenFSR ==========" << endmsg;

  // integrate all FSRs in one go
  const StatusCode mergeSC = merge();

  m_fileRecordSvc.reset();

  return StatusCode{mergeSC && GaudiAlgorithm::finalize()}; // must be called after all other actions
}

//=============================================================================
//  merge the FSR data
//=============================================================================
StatusCode GenFSRMerge::merge() {
  // make an inventory of the FileRecord store

  std::vector<std::string> slots     = {"/PrevPrev", "/Prev", "/Next", "/NextNext", ""};
  std::vector<std::string> addresses = m_navigatorTool->navigate( m_fileRecordName, m_FSRName );

  for ( const auto& slot : slots ) {

    std::string   nameFSR = slot + m_FSRName.value();
    LHCb::GenFSR* genFSR;
    int           nFSRs = 0;

    for ( const auto& genRecordAddress : addresses ) {
      if ( msgLevel( MSG::DEBUG ) ) debug() << "address: " << genRecordAddress << endmsg;

      if ( genRecordAddress.find( nameFSR ) != std::string::npos ) {
        genFSR = getIfExists<LHCb::GenFSR>( m_fileRecordSvc, genRecordAddress );
        nFSRs++;

        if ( m_evtType == 0 )
          m_evtType = genFSR->getSimulationInfo( "evtType", 0 );
        else if ( m_evtType != genFSR->getSimulationInfo( "evtType", 0 ) )
          throw GaudiException( "Found a different Event Type! Merging is not allowed!", "GenMergeFSR::finalize",
                                StatusCode::FAILURE );

        if ( m_genName == "" )
          m_genName = genFSR->getSimulationInfo( "hardGenerator", "" );
        else if ( m_genName != genFSR->getSimulationInfo( "hardGenerator", "" ) )
          throw GaudiException( "Found a different Hard Generator Name! Merging is not allowed!",
                                "GenMergeFSR::finalize", StatusCode::FAILURE );

        if ( m_method == "" )
          m_method = genFSR->getSimulationInfo( "generationMethod", "" );
        else if ( m_method != genFSR->getSimulationInfo( "generationMethod", "" ) )
          if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Found a different generation method" << endmsg;
      }
    }

    if ( nFSRs == 0 ) continue;

    LHCb::GenFSR* genFSRMerged = new LHCb::GenFSR();
    put( m_fileRecordSvc, genFSRMerged, m_fileRecordName.value() + nameFSR );

    genFSRMerged->initializeInfos();

    for ( const auto& genRecordAddress : addresses ) {
      if ( genRecordAddress.find( nameFSR ) != std::string::npos ) {
        genFSR = getIfExists<LHCb::GenFSR>( m_fileRecordSvc, genRecordAddress );

        if ( genFSR == nullptr ) {
          if ( msgLevel( MSG::VERBOSE ) ) error() << genRecordAddress << " not found" << endmsg;
        } else {
          if ( msgLevel( MSG::VERBOSE ) ) verbose() << genRecordAddress << " found" << endmsg;

          // Fill the informations of the new FSR
          if ( genFSRMerged->getSimulationInfo( "hardGenerator", "" ) == "" ) {
            genFSRMerged->setStringInfos( genFSR->stringInfos() );
            genFSRMerged->setIntInfos( genFSR->intInfos() );
          }

          // Update the FSR information and append to TS
          *genFSRMerged += *genFSR;
        }
      }
    }

    // read back genFSR from TS
    LHCb::GenFSR* topGenFSR = getIfExists<LHCb::GenFSR>( m_fileRecordSvc, m_fileRecordName.value() + nameFSR );
    if ( msgLevel( MSG::DEBUG ) ) debug() << *topGenFSR << endmsg;

    // clean up original FSRs
    for ( auto it = addresses.begin(); it != addresses.end(); ) {
      // get FSR as keyed object and cleanup the original ones - this only cleans genFSRs
      if ( msgLevel( MSG::DEBUG ) ) debug() << "address in list: " << *it << endmsg;

      if ( ( *it ).find( nameFSR ) != std::string::npos ) {
        genFSR = getIfExists<LHCb::GenFSR>( m_fileRecordSvc, *it );

        if ( !genFSR ) {
          if ( msgLevel( MSG::ERROR ) ) error() << *it << " not found" << endmsg;
        } else {
          if ( msgLevel( MSG::VERBOSE ) ) verbose() << *it << " found" << endmsg;

          m_fileRecordSvc->unlinkObject( *it ).ignore(); // get the FSR's directory out of TS
          it = addresses.erase( it );
        }
      } else
        ++it;
    }

    // Reset checking conditions
    m_evtType = 0;
    m_genName = "";
    m_method  = "";
    nFSRs     = 0;
  }

  // make a new inventory of the FileRecord store
  addresses = m_navigatorTool->navigate( m_fileRecordName, m_FSRName );
  if ( msgLevel( MSG::DEBUG ) )
    for ( const auto& addr : addresses ) debug() << "address: " << addr << endmsg;

  return StatusCode::SUCCESS;
}

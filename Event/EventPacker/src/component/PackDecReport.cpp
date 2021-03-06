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

// from Gaudi
#include "GaudiKernel/LinkManager.h"

#include "Event/HltDecReports.h"
#include "Event/PackedDecReport.h"
#include "Event/StandardPacker.h"

// local
#include "PackDecReport.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PackDecReport
//
// 2012-01-20 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( PackDecReport )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PackDecReport::PackDecReport( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "InputName", m_inputName = "Strip/Phys/DecReports" );
  declareProperty( "OutputName", m_outputName = LHCb::PackedDecReportLocation::Default );
  declareProperty( "AlwaysCreateOutput", m_alwaysOutput = false );
  declareProperty( "Filter", m_filter = true );
  declareProperty( "DeleteInput", m_deleteInput = false );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PackDecReport::execute() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput && !exist<LHCb::HltDecReports>( m_inputName ) ) return StatusCode::SUCCESS;

  // Create the output output
  if ( exist<LHCb::PackedDecReport>( m_outputName ) ) {
    // Need for use case of uDST writing from DSTS...
    return Warning( "Packed DecReports already exist at '" + m_outputName + "' -> Packing aborted",
                    StatusCode::SUCCESS );
  }
  LHCb::PackedDecReport* out = new LHCb::PackedDecReport();
  put( out, m_outputName );

  // Get the input
  LHCb::HltDecReports* reports = getOrCreate<LHCb::HltDecReports, LHCb::HltDecReports>( m_inputName );

  out->setConfiguredTCK( reports->configuredTCK() );
  if ( msgLevel( MSG::DEBUG ) ) { debug() << "TCK = " << out->configuredTCK() << endmsg; }

  // loop and pack
  for ( const auto& tR : reports->decReports() ) {
    // Get the report
    LHCb::HltDecReport tmp = tR.second.decReport();

    // If configured to do so, filter out null entries
    if ( m_filter && tmp.decision() == 0 ) continue;

    // store the result
    LinkManager::Link* myLink = out->linkMgr()->link( tR.first );
    if ( !myLink ) {
      out->linkMgr()->addLink( tR.first, nullptr );
      myLink = out->linkMgr()->link( tR.first );
    }
    tmp.setIntDecisionID( myLink->ID() + 1 ); // Store numbers starting at 1 as HltDecReport dislike 0!
    out->reports().emplace_back( tmp.decReport() );
    if ( msgLevel( MSG::DEBUG ) ) {
      debug() << format( "Stored report %8.8x  link ID %3d", tmp.decReport(), myLink->ID() ) << " name " << tR.first
              << endmsg;
    }
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "from " << reports->size() << " reports, stored " << out->reports().size() << " entries." << endmsg;
  }

  // If requested, remove the input data from the TES and delete
  if ( UNLIKELY( m_deleteInput ) ) {
    StatusCode sc = evtSvc()->unregisterObject( reports );
    if ( sc.isSuccess() ) {
      delete reports;
      reports = nullptr;
    } else
      return Error( "Failed to delete input data as requested", sc );
  } else {
    // Clear the registry address of the unpacked container, to prevent reloading
    auto* pReg = reports->registry();
    if ( pReg ) pReg->setAddress( nullptr );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================

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
// local
#include "L0TCKfilter.h"
#include "Event/L0DUReport.h"

//-----------------------------------------------------------------------------
// Implementation file for class : L0TCKfilter
//
// 2008-03-25 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( L0TCKfilter )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0TCKfilter::L0TCKfilter( const std::string& name, ISvcLocator* pSvcLocator ) : L0AlgBase( name, pSvcLocator ) {
  declareProperty( "TCKList", m_list );
  declareProperty( "ReportLocation", m_reportLocation = LHCb::L0DUReportLocation::Default );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode L0TCKfilter::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
  setFilterPassed( false );

  // get tck from raw
  const LHCb::L0DUReport* report = getIfExists<LHCb::L0DUReport>( m_reportLocation );
  if ( !report ) {
    counter( "Report not found L0TCKFilter reject" ) += 1;
    return Error( "L0DUReport not found at location " + m_reportLocation + " - event rejected", StatusCode::FAILURE );
  }
  unsigned int      tck = report->tck();
  std::stringstream ttck;
  ttck << format( "0x%04X", tck );
  // filter
  for ( auto it = m_list.begin(); it != m_list.end(); it++ ) {
    if ( "0x" != ( *it ).substr( 0, 2 ) ) {
      Error( "SKIP the requested TCK value " + *it + " (MUST be in hexadecimal format '0x" + *it + "')" ).ignore();
      continue;
    }
    if ( *it == ttck.str() ) {
      setFilterPassed( true );
      if ( msgLevel( MSG::DEBUG ) )
        debug() << "accepted TCK = " << ttck.str() << " -> set FilterPassed to true" << endmsg;
      counter( "L0TCKFilter accept" )++;
      return StatusCode::SUCCESS;
    }
  }
  if ( msgLevel( MSG::DEBUG ) ) debug() << "rejected TCK = " << ttck.str() << " -> set FilterPassed to false" << endmsg;
  counter( "L0TCKFilter reject" )++;

  return StatusCode::SUCCESS;
}

//=============================================================================

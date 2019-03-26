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
#include <fstream>

// Event/L0Event
#include "Event/ODIN.h"
#include "Event/RawEvent.h"

// local
#include "DumpL0CaloBanks.h"

DECLARE_COMPONENT( DumpL0CaloBanks )

//-----------------------------------------------------------------------------
// Implementation file for class : DumpL0CaloBanks
//
// 27/05/2008 : Patrick Robbe
//-----------------------------------------------------------------------------

// Standard creator
DumpL0CaloBanks::DumpL0CaloBanks( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ), m_event( 0 ), m_file( NULL ) {
  declareProperty( "FileName", m_fileName = "l0calobanks.txt" );
  declareProperty( "NZSBanks", m_nzsBanks = false );
  declareProperty( "ErrorBanks", m_errorBanks = false );
}

//=============================================================================
// Standard destructor
//=============================================================================
DumpL0CaloBanks::~DumpL0CaloBanks() {}

//=============================================================================
// Finalize.
//=============================================================================
StatusCode DumpL0CaloBanks::finalize() {
  m_file->close();
  delete m_file;
  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode DumpL0CaloBanks::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  m_file = new std::ofstream( m_fileName.c_str() );

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode DumpL0CaloBanks::execute() {
  // ODIN first
  ( *m_file ) << "---------------------------------------------" << ++m_event << std::endl;

  LHCb::ODIN* odin = getIfExists<LHCb::ODIN>( LHCb::ODINLocation::Default );
  if ( odin ) { ( *m_file ) << "event = " << odin->eventNumber() << " BCId = " << odin->bunchId() << '\n'; }

  LHCb::RawBank::BankType bankType = LHCb::RawBank::L0Calo;
  if ( m_nzsBanks )
    bankType = LHCb::RawBank::L0CaloFull;
  else if ( m_errorBanks )
    bankType = LHCb::RawBank::L0CaloError;

  LHCb::RawEvent* rawEvt = getIfExists<LHCb::RawEvent>( LHCb::RawEventLocation::Default );
  if ( rawEvt ) {
    const auto& banks = rawEvt->banks( bankType );

    if ( banks.empty() ) return StatusCode::SUCCESS;

    for ( const LHCb::RawBank* b : banks ) {
      const unsigned int* body = b->data();
      for ( int k = 0; b->size() / 4 > k; ++k ) { ( *m_file ) << std::hex << ( *body++ ) << '\n'; }
    }
  }

  //  return sc ;
  return StatusCode::SUCCESS;
}
//============================================================================

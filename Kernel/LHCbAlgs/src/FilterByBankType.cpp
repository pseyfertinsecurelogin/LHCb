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

// From DAQEvent
#include "Event/RawBank.h"
#include "Event/RawEvent.h"

// For regular expressions
#include <regex>

// local
#include "FilterByBankType.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : FilterByBankType
//
// 2009-03-06 : Jeroen van Tilburg
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FilterByBankType )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FilterByBankType::FilterByBankType( const std::string& name, ISvcLocator* pSvcLocator )
    : FilterPredicate( name, pSvcLocator, KeyValue{"InputLocation", RawEventLocation::Default} ) {}

//=============================================================================
// Main execution
//=============================================================================
bool FilterByBankType::operator()( const LHCb::RawEvent& raw ) const {
  // Initialize the select event flag
  bool selectEvent = false;

  // Loop over the bank types
  auto iBankType = m_bankTypes.begin();
  while ( !selectEvent && iBankType < m_bankTypes.end() ) {

    // Get the bank in the RawEvent
    const auto& bank = raw.banks( *iBankType );

    // If bank exist mark the event
    if ( !bank.empty() ) {
      selectEvent = true;

      // Make some printout if bank is found
      if ( msgLevel( MSG::DEBUG ) ) {
        debug() << "Found " << bank.size() << " bank(s) of type " << *iBankType << " (" << bank[0]->typeName() << ")."
                << endmsg;
      }
    }

    // Loop over bank types
    ++iBankType;

  } // end of loop over bank types

  return selectEvent ? m_passSelect.value() : !m_passSelect.value();
}
//=============================================================================

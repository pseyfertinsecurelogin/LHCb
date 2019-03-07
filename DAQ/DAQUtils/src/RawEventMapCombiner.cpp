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

// local
#include "RawEventMapCombiner.h"
#include "Event/RawEvent.h"

//-----------------------------------------------------------------------------
// Implementation file for class : RawEventMapCombiner
//
// 2009-06-22 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( RawEventMapCombiner )

using namespace LHCb;
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RawEventMapCombiner::RawEventMapCombiner( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "RawBanksToCopy", m_banksToCopy );
  declareProperty( "OutputRawEventLocation", m_outputLocation = RawEventLocation::Default );
}

StatusCode RawEventMapCombiner::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) return sc;

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // convert bankNames to bankTypes
  m_bankTypes.clear();
  // const std::vector<std::string>& theNameVector = m_banksToCopy.value();

  // selective banks
  for ( std::map<std::string, std::string>::const_iterator aBank = m_banksToCopy.begin(); aBank != m_banksToCopy.end();
        ++aBank ) {
    bool found = false;
    for ( int i = 0; i != (int)RawBank::LastType; i++ ) {
      const std::string name = RawBank::typeName( (RawBank::BankType)i );
      if ( name == aBank->first ) {
        found                                 = true;
        m_bankTypes[( (RawBank::BankType)i )] = aBank->second;
        break;
      }
    }
    if ( !found ) warning() << "Requested bank '" << aBank->first << "' is not a valid name" << endmsg;
  }
  if ( msgLevel( MSG::VERBOSE ) ) {
    verbose() << " RawBank types to be copied= ";
    for ( const auto& ib : m_bankTypes ) {
      verbose() << RawBank::typeName( ib.first ) << " from " << ib.second << " , ";
    }
    verbose() << endmsg;
  }

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode RawEventMapCombiner::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  std::map<LHCb::RawBank::BankType, RawEvent*> foundRawEvents; // translation of m_banksToCopy

  // find all the raw events
  for ( std::map<LHCb::RawBank::BankType, std::string>::const_iterator ib = m_bankTypes.begin();
        ib != m_bankTypes.end(); ++ib ) {
    RawEvent* rawEvent = getIfExists<RawEvent>( ib->second ); // try with RootInTes

    if ( !rawEvent ) { // try without RootInTes
      rawEvent = getIfExists<RawEvent>( ib->second, false );
    }

    if ( !rawEvent ) { // if still not found it's a problem
      return Error( " No RawEvent at " + ( ib->second ), StatusCode::SUCCESS, 20 );
    }
    foundRawEvents[ib->first] = rawEvent;
  }

  // create empty output RawEvent
  RawEvent* rawEventCopy = new RawEvent();

  // copy selected raw banks
  for ( std::map<LHCb::RawBank::BankType, RawEvent*>::const_iterator ib = foundRawEvents.begin();
        ib != foundRawEvents.end(); ++ib ) {

    for ( const RawBank* bank : ( ib->second )->banks( ib->first ) ) {
      if ( !bank ) continue;
      rawEventCopy->adoptBank(
          rawEventCopy->createBank( bank->sourceID(), ib->first, bank->version(), bank->size(), bank->data() ), true );
      if ( msgLevel( MSG::VERBOSE ) ) {
        verbose() << " Copied RawBank type=" << RawBank::typeName( ib->first ) << " version= " << bank->version()
                  << " sourceID= " << bank->sourceID() << " size (bytes) = " << bank->size() << endmsg;
      }
    }
  }

  // put output RawEvent into its location
  if ( msgLevel( MSG::VERBOSE ) ) { verbose() << " Saving Copied RawEvent into new locations " << endmsg; }
  put( rawEventCopy, m_outputLocation );
  if ( msgLevel( MSG::VERBOSE ) ) { verbose() << " Saved Copied RawEvent into new locations " << endmsg; }

  return StatusCode::SUCCESS;
}

//=============================================================================

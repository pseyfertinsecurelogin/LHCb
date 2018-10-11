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
// Include files
#include <algorithm>
// local
#include "Event/RawEvent.h"
#include "RawEventSelectiveCopy.h"

//-----------------------------------------------------------------------------
// Implementation file for class : RawEventSelectiveCopy
//
// 2009-06-22 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( RawEventSelectiveCopy )

using namespace LHCb;

StatusCode RawEventSelectiveCopy::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if (sc.isFailure()) return sc;

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  const std::vector<std::string>& copyVector   = m_banksToCopy.value();
  const std::vector<std::string>& removeVector = m_banksToRemove.value();

  if( copyVector.size() > 0 && removeVector.size() > 0 ) {
    return Error( "Inconsistent properties, set only one of RawBanksToCopy and RawBanksToRemove" );
  }

  m_bankTypes.clear();

  if( copyVector.size() > 0 ) {
    // all banks?
    if( copyVector.size()==1 ){
      if( copyVector[0] == "all" ||
          copyVector[0] == "All" ||
          copyVector[0] == "ALL" ){
        for(int i = 0 ; i != (int) RawBank::LastType; i++){
          m_bankTypes.push_back( (RawBank::BankType) i );
        }
        if ( msgLevel(MSG::VERBOSE) )
          verbose() << " All RawBank types will be copied from input to output RawEvent " << endmsg;
        return sc;
      }
    }
    // convert bankNames to bankTypes
    for( std::vector<std::string>::const_iterator bankName = copyVector.begin();
         bankName != copyVector.end(); ++bankName ){
      bool found = false;
      for(int i = 0 ; i != (int) RawBank::LastType; i++){
        const std::string name = RawBank::typeName( (RawBank::BankType) i );
        if( name == *bankName){
          found = true;
          m_bankTypes.push_back( (RawBank::BankType) i );
          break;
        }
      }
      if( !found) {
        error() << "Requested bank '" << *bankName << "' is not a valid name" << endmsg;
        return Error( "Invalid bank name requested" );
      }
    }
  }

  else if( removeVector.size() > 0 ) {
    // all banks?
    if( removeVector.size()==1 ){
      if( removeVector[0] == "all" ||
          removeVector[0] == "All" ||
          removeVector[0] == "ALL" ){
        return Error("Requested to remove ALL banks from copied event!" );
      }
    }

    // Check inputs
    for( auto bankName = removeVector.begin(); bankName != removeVector.end(); ++bankName ){
      bool found = false;
      for(int i = 0 ; i != (int) RawBank::LastType; i++){
        const std::string name = RawBank::typeName( (RawBank::BankType) i );
        if( name == *bankName){
          found = true;
          break;
        }
      }
      if( !found) {
        error() << "Requested bank '" << *bankName << "' is not a valid name" << endmsg;
        return Error( "Invalid bank name requested" );
      }
    }

    // convert bankNames to bankTypes
    for(int i = 0 ; i != (int) RawBank::LastType; i++){
      bool found = std::any_of( removeVector.begin(), removeVector.end(),
                                [name = RawBank::typeName((RawBank::BankType)i)]
                                (const std::string& s)
                                { return s == name; } );
      if( !found) m_bankTypes.push_back( (RawBank::BankType) i );
    }
  }

  else {
    return Warning( "Neither RawBanksToCopy nor RawBanksToRemove are set, algorithm has no effect", sc );
  }



  if( msgLevel(MSG::DEBUG) ){
    debug() << " RawBank types to be copied = ";
    for( const auto& b : m_bankTypes) {
      debug() << RawBank::typeName( b ) << " ";
    }
    debug() << endmsg;
  }

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode RawEventSelectiveCopy::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // do nothing of output location already exists (e.g. reprocessing from (S)DST)
  if( exist<RawEvent>(m_outputLocation.value()) ){
    return Warning(" Output location " + m_outputLocation.value() + " already exists, do nothing"
                   ,StatusCode::SUCCESS, 20 );
  }

  // get input RawEvent
  RawEvent* rawEvent = getIfExists<RawEvent>(m_inputLocation.value());
  if( !rawEvent ){
    return Error(" No RawEvent at " + m_inputLocation.value(),StatusCode::SUCCESS, 20 );
  }


  // create empty output RawEvent
  auto rawEventCopy = std::make_unique<RawEvent>();

  // copy selected banks
  for( auto ib = m_bankTypes.begin(); ib!=m_bankTypes.end();++ib){

    const auto& banks= rawEvent->banks( *ib );
    if( !banks.empty() ) {
      for( const RawBank* b : banks ) {
        rawEventCopy->adoptBank( rawEventCopy->createBank( b->sourceID(), *ib, b->version(), b->size(),b->data() ),
                                 true );
        if( msgLevel(MSG::VERBOSE) ){
          verbose() << " Copied RawBank type= " << RawBank::typeName( *ib )
                    << " version= " << b->version()
                    << " sourceID= " << b->sourceID()
                    << " size (bytes) = " << b->size()
                    << endmsg;
        }
      }
    }
    else
      if( msgLevel(MSG::VERBOSE) ){
        verbose() << " No banks found of type= " << RawBank::typeName( *ib ) << endmsg;
      }
  }


  // put output RawEvent into its location
  if( msgLevel(MSG::DEBUG) ){ debug() << " Saving Copied RawEvent into new locations " << endmsg;  }
  put( rawEventCopy.release(), m_outputLocation.value() );

  return StatusCode::SUCCESS;
}

//=============================================================================

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
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/ISvcLocator.h"
#include <map>
#include <string>

//-----------------------------------------------------------------------------
// Implementation file for class : RawEventMapCombiner
//
// 2009-06-22 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

/** @class RawEventMapCombiner RawEventMapCombiner.h
 *  Combines disparate raw events into one new location
 *  Based on RawEventSelectiveCopy from Tomasz
 *  Use if the split locations possibly hold duplicates of certain raw banks
 *
 *  @author Rob Lambert
 *  @date   2013-04-03
 */
class RawEventMapCombiner : public GaudiAlgorithm {
public:
  /// Standard constructor
  RawEventMapCombiner( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  Gaudi::Property<std::map<std::string, std::string>> m_banksToCopy{
      this, "RawBanksToCopy", {}}; // which banks to recombine, from where, (bank, location)
  Gaudi::Property<std::string> m_outputLocation{
      this, "OutputRawEventLocation", LHCb::RawEventLocation::Default}; // where to put them, DEFAULT /Event/RawEvent

  std::map<LHCb::RawBank::BankType, std::string> m_bankTypes; // translation of m_banksToCopy
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( RawEventMapCombiner )

using namespace LHCb;
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RawEventMapCombiner::RawEventMapCombiner( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {}

StatusCode RawEventMapCombiner::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) return sc;

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // convert bankNames to bankTypes
  m_bankTypes.clear();
  // const std::vector<std::string>& theNameVector = m_banksToCopy.value();

  // selective banks
  for ( auto aBank = m_banksToCopy.begin(); aBank != m_banksToCopy.end(); ++aBank ) {
    bool found = false;
    for ( int i = 0; i != (int)RawBank::LastType; i++ ) {
      if ( RawBank::typeName( (RawBank::BankType)i ) == aBank->first ) {
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
  for ( const auto& b : m_bankTypes ) {
    RawEvent* rawEvent = getIfExists<RawEvent>( b.second ); // try with RootInTes

    if ( !rawEvent ) { // try without RootInTes
      rawEvent = getIfExists<RawEvent>( b.second, false );
    }

    if ( !rawEvent ) { // if still not found it's a problem
      return Error( " No RawEvent at " + b.second, StatusCode::SUCCESS, 20 );
    }
    foundRawEvents[b.first] = rawEvent;
  }

  // create empty output RawEvent
  RawEvent* rawEventCopy = new RawEvent();

  // copy selected raw banks
  for ( const auto& ib : foundRawEvents ) {

    for ( const RawBank* bank : ib.second->banks( ib.first ) ) {
      if ( !bank ) continue;
      rawEventCopy->adoptBank(
          rawEventCopy->createBank( bank->sourceID(), ib.first, bank->version(), bank->size(), bank->data() ), true );
      if ( msgLevel( MSG::VERBOSE ) ) {
        verbose() << " Copied RawBank type=" << RawBank::typeName( ib.first ) << " version= " << bank->version()
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

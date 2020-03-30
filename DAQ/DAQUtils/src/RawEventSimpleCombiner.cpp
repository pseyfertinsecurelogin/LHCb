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
#include <string>
#include <vector>

//-----------------------------------------------------------------------------
// Implementation file for class : RawEventSimpleCombiner
//
// 2009-06-22 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

/** @class RawEventSimpleCombiner RawEventSimpleCombiner.h
 *  Combines disparate raw events into one new location
 *  Based on RawEventSelectiveCopy from Tomasz
 *  Use if the split locations hold no raw event bank duplicates
 *
 *  @author Rob Lambert
 *  @date   2013-04-03
 *
 *  2018-07-06 Added functionality that allows to add raw events to an existing one @rcenci
 */
class RawEventSimpleCombiner : public GaudiAlgorithm {
public:
  /// Standard constructor
  RawEventSimpleCombiner( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  Gaudi::Property<std::vector<std::string>> m_banksToCopy{this, "RawBanksToCopy", {"ALL"}}; // which banks to recombine,
                                                                                            // default ALL
  Gaudi::Property<std::vector<std::string>> m_inputLocations{this, "InputRawEventLocations", {}}; // get the banks from
                                                                                                  // where
  Gaudi::Property<std::string> m_outputLocation{
      this, "OutputRawEventLocation", LHCb::RawEventLocation::Default}; // where to put them, DEFAULT /Event/RawEvent

  std::vector<LHCb::RawBank::BankType> m_bankTypes;

  Gaudi::Property<bool> m_enableIncrementalMode{
      this, "EnableIncrementalMode", false,
      "Enable incremental mode, where raw events are added to output raw event (already created)"};
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( RawEventSimpleCombiner )

using namespace LHCb;
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RawEventSimpleCombiner::RawEventSimpleCombiner( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {}

StatusCode RawEventSimpleCombiner::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) return sc;

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // convert bankNames to bankTypes
  m_bankTypes.clear();
  // const std::vector<std::string>& theNameVector = m_banksToCopy.value();

  // all banks?
  if ( m_banksToCopy.size() == 1 ) {
    if ( m_banksToCopy[0] == "all" || m_banksToCopy[0] == "All" || m_banksToCopy[0] == "ALL" ) {
      for ( int i = 0; i != (int)RawBank::LastType; i++ ) { m_bankTypes.push_back( (RawBank::BankType)i ); }
      if ( msgLevel( MSG::VERBOSE ) )
        verbose() << " All RawBank types will be copied from input to output RawEvent " << endmsg;
      return sc;
    }
  }

  // selective banks
  for ( const auto& bankName : m_banksToCopy ) {
    bool found = false;
    for ( int i = 0; i != (int)RawBank::LastType; i++ ) {
      const std::string name = RawBank::typeName( (RawBank::BankType)i );
      if ( name == bankName ) {
        found = true;
        m_bankTypes.push_back( (RawBank::BankType)i );
        break;
      }
    }
    if ( !found ) warning() << "Requested bank '" << bankName << "' is not a valid name" << endmsg;
  }
  if ( msgLevel( MSG::VERBOSE ) ) {
    verbose() << " RawBank types to be copied= ";
    for ( const auto& ib : m_bankTypes ) verbose() << RawBank::typeName( ib ) << " ";
    verbose() << endmsg;
  }

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode RawEventSimpleCombiner::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  std::vector<const RawEvent*> foundRawEvents;

  // get input RawEvents
  for ( auto il = m_inputLocations.begin(); il != m_inputLocations.end(); il++ ) {
    const RawEvent* rawEvent = getIfExists<RawEvent>( *il ); // try with RootInTes

    if ( !rawEvent ) // try without RootInTes
    {
      rawEvent = getIfExists<RawEvent>( *il, false );
    }

    if ( !rawEvent ) { return Error( " No RawEvent at " + ( *il ), StatusCode::SUCCESS, 20 ); }
    foundRawEvents.push_back( rawEvent );
  }

  RawEvent* rawEventCopy;
  if ( m_enableIncrementalMode ) {
    // retrieve output location
    rawEventCopy = getIfExists<RawEvent>( m_outputLocation );
    if ( rawEventCopy == NULL ) return Error( " No RawEvent at " + m_outputLocation, StatusCode::SUCCESS, 20 );
  } else {
    // create empty output RawEvent
    rawEventCopy = new RawEvent();
  }

  // Loop over locations I've found
  for ( auto rawEvent = foundRawEvents.begin(); rawEvent != foundRawEvents.end(); rawEvent++ ) {
    // copy selected raw banks
    for ( auto ib = m_bankTypes.begin(); ib != m_bankTypes.end(); ++ib ) {
      for ( const RawBank* b : ( *rawEvent )->banks( *ib ) ) {
        if ( !b ) continue;

        // const RawBank & bank = *b;
        rawEventCopy->adoptBank( rawEventCopy->createBank( b->sourceID(), *ib, b->version(), b->size(), b->data() ),
                                 true );
        if ( msgLevel( MSG::VERBOSE ) ) {
          verbose() << " Copied RawBank type=" << RawBank::typeName( *ib ) << " version= " << b->version()
                    << " sourceID= " << b->sourceID() << " size (bytes) = " << b->size() << endmsg;
        }
      }
    }
  }

  // if not incremental mode, put output RawEvent into its location
  if ( !m_enableIncrementalMode ) {
    if ( msgLevel( MSG::VERBOSE ) ) { verbose() << " Saving Copied RawEvent into new locations " << endmsg; }
    put( rawEventCopy, m_outputLocation );
    if ( msgLevel( MSG::VERBOSE ) ) { verbose() << " Saved Copied RawEvent into new locations " << endmsg; }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================

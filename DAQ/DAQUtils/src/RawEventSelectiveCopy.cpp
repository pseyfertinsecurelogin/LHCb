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
#include "GaudiAlg/Transformer.h"
#include <algorithm>
#include <string>
#include <vector>

//-----------------------------------------------------------------------------
// Implementation file for class : RawEventSelectiveCopy
//
// 2009-06-22 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

/** @class RawEventSelectiveCopy RawEventSelectiveCopy.h
 *  Copies selected RawBanks to RawEvent at a new TES location.
 *
 *  @author Tomasz Skwarnicki
 *  @date   2009-06-22
 */
class RawEventSelectiveCopy : public Gaudi::Functional::Transformer<LHCb::RawEvent( const LHCb::RawEvent& )> {
public:
  /// Standard constructor
  RawEventSelectiveCopy( const std::string& name, ISvcLocator* pSvcLocator )
      : Transformer{name,
                    pSvcLocator,
                    {"InputRawEventLocation", LHCb::RawEventLocation::Default},
                    {"OutputRawEventLocation", LHCb::RawEventLocation::Copied}} {}

  StatusCode     initialize() override;                              ///< Algorithm initialization
  LHCb::RawEvent operator()( const LHCb::RawEvent& ) const override; ///< Algorithm execution

private:
  Gaudi::Property<std::vector<std::string>> m_banksToCopy{
      this, "RawBanksToCopy", {}, "Create a new RawEvent copying only these banks"};
  Gaudi::Property<std::vector<std::string>> m_banksToRemove{
      this, "RawBanksToRemove", {}, "Create a RawEvent copy, with these banks removed"};

  std::vector<LHCb::RawBank::BankType> m_bankTypes;
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( RawEventSelectiveCopy )

using namespace LHCb;

StatusCode RawEventSelectiveCopy::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) return sc;

  const std::vector<std::string>& copyVector   = m_banksToCopy.value();
  const std::vector<std::string>& removeVector = m_banksToRemove.value();

  if ( copyVector.size() > 0 && removeVector.size() > 0 ) {
    return Error( "Inconsistent properties, set only one of RawBanksToCopy and RawBanksToRemove" );
  }

  m_bankTypes.clear();

  if ( copyVector.size() > 0 ) {
    // all banks?
    if ( copyVector.size() == 1 ) {
      if ( copyVector[0] == "all" || copyVector[0] == "All" || copyVector[0] == "ALL" ) {
        for ( int i = 0; i != (int)RawBank::LastType; i++ ) { m_bankTypes.push_back( (RawBank::BankType)i ); }
        if ( msgLevel( MSG::VERBOSE ) )
          verbose() << " All RawBank types will be copied from input to output RawEvent " << endmsg;
        return sc;
      }
    }
    // convert bankNames to bankTypes
    for ( auto bankName = copyVector.begin(); bankName != copyVector.end(); ++bankName ) {
      bool found = false;
      for ( int i = 0; i != (int)RawBank::LastType; i++ ) {
        const std::string name = RawBank::typeName( (RawBank::BankType)i );
        if ( name == *bankName ) {
          found = true;
          m_bankTypes.push_back( (RawBank::BankType)i );
          break;
        }
      }
      if ( !found ) {
        error() << "Requested bank '" << *bankName << "' is not a valid name" << endmsg;
        return Error( "Invalid bank name requested" );
      }
    }
  }

  else if ( removeVector.size() > 0 ) {
    // all banks?
    if ( removeVector.size() == 1 ) {
      if ( removeVector[0] == "all" || removeVector[0] == "All" || removeVector[0] == "ALL" ) {
        return Error( "Requested to remove ALL banks from copied event!" );
      }
    }

    // Check inputs
    for ( auto bankName = removeVector.begin(); bankName != removeVector.end(); ++bankName ) {
      bool found = false;
      for ( int i = 0; i != (int)RawBank::LastType; i++ ) {
        const std::string name = RawBank::typeName( (RawBank::BankType)i );
        if ( name == *bankName ) {
          found = true;
          break;
        }
      }
      if ( !found ) {
        error() << "Requested bank '" << *bankName << "' is not a valid name" << endmsg;
        return Error( "Invalid bank name requested" );
      }
    }

    // convert bankNames to bankTypes
    for ( int i = 0; i != (int)RawBank::LastType; i++ ) {
      bool found = std::any_of(
          removeVector.begin(), removeVector.end(),
          [name = RawBank::typeName( (RawBank::BankType)i )]( const std::string& s ) { return s == name; } );
      if ( !found ) m_bankTypes.push_back( (RawBank::BankType)i );
    }
  }

  else {
    return Warning( "Neither RawBanksToCopy nor RawBanksToRemove are set, algorithm has no effect", sc );
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << " RawBank types to be copied = ";
    for ( const auto& b : m_bankTypes ) { debug() << RawBank::typeName( b ) << " "; }
    debug() << endmsg;
  }

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
LHCb::RawEvent RawEventSelectiveCopy::operator()( const LHCb::RawEvent& rawEvent ) const {
  // create empty output RawEvent
  RawEvent rawEventCopy;
  // copy selected banks
  for ( const auto& ib : m_bankTypes ) {
    const auto& banks = rawEvent.banks( ib );
    for ( const RawBank* b : banks ) {
      rawEventCopy.adoptBank( rawEventCopy.createBank( b->sourceID(), ib, b->version(), b->size(), b->data() ), true );
      if ( msgLevel( MSG::VERBOSE ) ) {
        verbose() << " Copied RawBank type= " << RawBank::typeName( ib ) << " version= " << b->version()
                  << " sourceID= " << b->sourceID() << " size (bytes) = " << b->size() << endmsg;
      }
    }
    if ( banks.empty() && msgLevel( MSG::VERBOSE ) ) {
      verbose() << " No banks found of type= " << RawBank::typeName( ib ) << endmsg;
    }
  }
  return rawEventCopy;
}

//=============================================================================

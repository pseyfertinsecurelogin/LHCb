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
#include "DAQKernel/DecoderAlgBase.h"
#include "Event/RawEvent.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// ============================================================================
/** @file bankKiller.cpp
 *
 *    @author: Olivier Deschamps
 *    @date:   O6 September 2006
 */
// ============================================================================

namespace {
  struct bankType_t {
    LHCb::RawBank::BankType type;

    friend bool operator==( const bankType_t& lhs, const LHCb::RawBank::BankType& rhs ) { return lhs.type == rhs; }

    friend StatusCode parse( bankType_t& bt, const std::string& in ) {
      for ( int ibank = 0; ibank < (int)LHCb::RawBank::LastType; ++ibank ) {
        if ( in == LHCb::RawBank::typeName( static_cast<LHCb::RawBank::BankType>( ibank ) ) ) {
          bt.type = static_cast<LHCb::RawBank::BankType>( ibank );
          return StatusCode::SUCCESS;
        }
      }
      return StatusCode::FAILURE;
    }

    friend std::string toString( const bankType_t& t ) { return LHCb::RawBank::typeName( t.type ); }

    friend std::ostream& toStream( const bankType_t& t, std::ostream& os ) {
      return os << std::quoted( toString( t ), '\'' );
    }
  };

  template <typename InnerType>
  StatusCode parse( std::vector<InnerType>& v, const std::string& in ) {
    v.clear();
    using Gaudi::Parsers::parse;
    std::vector<std::string> vs;
    auto                     sc = parse( vs, in );
    if ( !sc ) return sc;
    v.reserve( vs.size() );
    try {
      std::transform( vs.begin(), vs.end(), std::back_inserter( v ), []( const std::string& s ) {
        InnerType t{};
        auto      sc = parse( t, s );
        if ( !sc ) throw GaudiException( "Bad Parse", "", sc );
        return t;
      } );
    } catch ( const GaudiException& ge ) { return ge.code(); }
    return StatusCode::SUCCESS;
  }
} // namespace

/** @class bankKiller bankKiller.h
 *
 *   Killer of raw event banks.
 *
 *   Input:
 *    - BankTypes : list of bank names to be killed or saved
 *    - DefaultIsKill : Main behaviour switch. If false (default), kill only given banks. If true, kill all BUT given
 * banks.
 *
 *    @author: Olivier Deschamps
 *    @date:   O6 Septembre 2006
 */

class bankKiller : public Decoder::AlgBase {
public:
  bankKiller( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;
  StatusCode execute() override;

private:
  ///. kill all banks of given type
  void killBankType( LHCb::RawEvent*, LHCb::RawBank::BankType bankType, bool warningmsg ) const;

  ///. kill all banks with the current m_rawEvent
  StatusCode killFromRawEvent( LHCb::RawEvent* ) const;

  Gaudi::Property<std::vector<bankType_t>> m_bankTypes{this, "BankTypes", {}, "List of bank names"}; ///< List of bank
                                                                                                     ///< names
  /// Main behaviour switch. If false (default), kill only given banks. If true, kill all BUT given banks.
  Gaudi::Property<bool> m_defaultIsKill{
      this, "DefaultIsKill", false,
      "Main behaviour switch. If false (default), kill only given banks. If true, kill all BUT given banks."};
  // kill from all raw event locations, or just from the first one?
  Gaudi::Property<bool> m_killFromAll{
      this, "KillFromAll", false,
      "Main behaviour switch. If false (default), kill only banks in the first location found in the search string. If "
      "false, kill *all* banks found in the search string."};
  Gaudi::Property<int> m_sourceKill{this, "KillSourceID", 0,
                                    "Kill bank type with specific source IDs, see KillSourceIDMask."};
  Gaudi::Property<int> m_sourceKillMask{this, "KillSourceIDMask", 0,
                                        "AND mask for sourceID before comparison with KillSourceID."};
};

DECLARE_COMPONENT( bankKiller )

//=============================================================================
// Standard creator, initializes variables
//=============================================================================
bankKiller::bankKiller( const std::string& name, ISvcLocator* pSvcLocator ) : Decoder::AlgBase( name, pSvcLocator ) {
  m_rawEventLocations = {LHCb::RawEventLocation::Default};
  initRawEventSearch();
}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode bankKiller::initialize() {

  StatusCode sc = Decoder::AlgBase::initialize();
  if ( sc.isFailure() ) return sc;

  if ( m_bankTypes.empty() )
    warning() << ( m_defaultIsKill ? "bankKiller : nothing specified to be kept - all will be removed "
                                   : "bankKiller : nothing to be removed " )
              << endmsg;

  for ( const auto& bt : m_bankTypes ) {
    always() << "bankKiller : all banks of type '" << toString( bt ) << "' will be "
             << ( m_defaultIsKill ? "kept." : "removed." ) << endmsg;
  }
  if ( m_rawEventLocations.empty() )
    return Error( "You didn't give me a list of RawEventLocations to look through", StatusCode::FAILURE );

  if ( m_killFromAll ) {
    info() << "Killing banks from everything in '" << m_rawEventLocations << "'" << endmsg;
  } else if ( m_rawEventLocations.size() != 1 || m_rawEventLocations[0] != LHCb::RawEventLocation::Default ) {
    info() << "Killing banks from first raw event in '" << m_rawEventLocations << "'" << endmsg;
  }

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode bankKiller::execute() {

  if ( !m_killFromAll ) {
    auto rawEvt = findFirstRawEvent();
    if ( !rawEvt ) return Error( "Failed to find raw data" );
    auto sc = killFromRawEvent( rawEvt );
    return sc;
  }

  for ( const auto& loc : m_rawEventLocations ) {
    auto rawEvt = tryEventAt( loc );
    if ( rawEvt ) {
      auto sc = killFromRawEvent( rawEvt );
      if ( sc.isFailure() ) return sc;
    }
  }
  return StatusCode::SUCCESS;
}

StatusCode bankKiller::killFromRawEvent( LHCb::RawEvent* rawEvent ) const {

  if ( m_defaultIsKill ) {
    for ( unsigned int ibank = 0; ibank < (unsigned int)LHCb::RawBank::LastType; ++ibank ) {
      LHCb::RawBank::BankType bankType = (LHCb::RawBank::BankType)ibank;
      bool keep = std::any_of( m_bankTypes.begin(), m_bankTypes.end(), [&]( const auto& i ) { return i == bankType; } );
      if ( !keep ) { // kill al banks of this type
        killBankType( rawEvent, bankType, false );
      }
    }
  } else {
    for ( const auto& bt : m_bankTypes ) { // kill all banks of this type
      killBankType( rawEvent, bt.type, true );
    }
  }
  return StatusCode::SUCCESS;
}

//=============================================================================

void bankKiller::killBankType( LHCb::RawEvent* rawEvent, LHCb::RawBank::BankType bankType, bool warningmsg ) const {
  const std::string bankTypeName = LHCb::RawBank::typeName( bankType );
  // look for all banks of this type and remove them
  const auto& banks = rawEvent->banks( bankType );
  if ( banks.empty() ) {
    if ( warningmsg ) {
      std::stringstream s;
      s << " No bank of type '" << bankTypeName << "' has been found - nothing to be removed";
      Warning( s.str(), StatusCode::SUCCESS, 0 ).ignore();
    }
    return;
  }

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "All banks of type '" << bankTypeName << "'  are to be removed - banks size =  " << banks.size()
            << endmsg;

  // note that we need to _copy_ the list of banks, as the original list will be modified
  // by 'removeBank' while we're looping
  for ( auto& bank : LHCb::RawBank::ConstVector{banks.begin(), banks.end()} ) {
    if ( m_sourceKillMask != 0 && m_sourceKill != ( bank->sourceID() & m_sourceKillMask ) ) { continue; }
    bool success = rawEvent->removeBank( bank );
    if ( !success && warningmsg ) {
      if ( msgLevel( MSG::DEBUG ) )
        debug() << "The bank " << bank << " of type '" << bankTypeName << "' has not been found to be removed "
                << endmsg;
      Warning( "The bank of type '" + bankTypeName + "' has not been found", StatusCode::SUCCESS, 0 ).ignore();
    }
  }
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "All banks of type '" << bankTypeName << "' have been removed  - "
            << " banks size is now : " << rawEvent->banks( bankType ).size() << endmsg;
}

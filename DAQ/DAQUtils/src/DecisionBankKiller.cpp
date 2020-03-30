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
#include "Event/HltDecReports.h"
#include "Event/RawEvent.h"
#include "GaudiAlg/Consumer.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Kernel/ILinePersistenceSvc.h"
#include "LoKi/IHDRFilterTool.h"
#include <algorithm>
#include <map>
#include <regex>
#include <string>
#include <vector>

/** @class DecisionBankKiller DecisionBankKiller.h
 *
 *   Decision Bank Killer class to keep only banks requested by lines
 *
 *   Input:
 *    - InputHltDecReportsLocation : HltDecReports location
 *    - KillFromAll : Main behaviour switch. If false (default), kill only banks
 *      in the first location found in the search string. If true, kill *all*
 *      banks found in the search string.
 *    - LineFilter : An HDRFilterTool configured with an HDR filter selecting the
 *      subset of lines the decision and request of which will be considered.
 *    - AlwaysKeepBanks: Set of bank types to always keep regardless of decisions.
 *
 *    @author: Konstantin Gizdov, Rosen Matev
 *    @date:   25 August 2016, 23 June 2017
 */

class DecisionBankKiller
    : public Gaudi::Functional::Consumer<void( const LHCb::HltDecReports& ),
                                         Gaudi::Functional::Traits::BaseClass_t<Decoder::AlgBase>> {

public:
  DecisionBankKiller( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;
  void       operator()( const LHCb::HltDecReports& decReports ) const override;

protected:
  using ReqBanksBitset = ILinePersistenceSvc::RawBanks;

  /// Find the line decision names that could pass the filter in m_lineFilterTool
  ILinePersistenceSvc::LineDecNames filteredLines( const LHCb::HltDecReports& hdr ) const;

  /// kill all banks of given type in a given raw event
  void killBankType( LHCb::RawEvent* rawEvent, LHCb::RawBank::BankType bankType, bool warningmsg ) const;

  /// kill all non-requested banks in a given raw event
  void killFromRawEvent( LHCb::RawEvent* rawEvent, const ReqBanksBitset& reqBanks ) const;

private:
  /// Set of banks to be always kept
  ReqBanksBitset                            m_alwaysKeepBanksSet{};
  Gaudi::Property<std::vector<std::string>> m_alwaysKeepBanks{
      this,
      "AlwaysKeepBanks",
      {"ODIN", "HltRoutingBits", "DAQ"},
      [=]( auto& ) {
        for ( unsigned i = 0; i < LHCb::RawBank::LastType; ++i ) {
          auto type = LHCb::RawBank::typeName( static_cast<LHCb::RawBank::BankType>( i ) );
          if ( std::find( std::begin( m_alwaysKeepBanks ), std::end( m_alwaysKeepBanks ), type ) !=
               std::end( m_alwaysKeepBanks ) ) {
            m_alwaysKeepBanksSet[i] = 1;
          }
        }
      },
      Gaudi::Details::Property::ImmediatelyInvokeHandler{true},
      "Set of Banks to always keep regardless of line conditions."};
  Gaudi::Property<bool> m_killFromAll{this, "KillFromAll", false,
                                      "Whether to kill from all raw event locations, or just from the first one"};
  /// Implementation of ILinePersistenceSvc used to get the requested raw banks
  Gaudi::Property<std::string> m_linePersistenceSvcName{this, "ILinePersistenceSvc", ""};
  /// LoKi Tool to execute line filter
  ToolHandle<LoKi::IHDRFilterTool> m_lineFilterTool{this, "LineFilter", "LoKi::HDRFilterTool",
                                                    "LoKi Tool to execute line filter"};

  SmartIF<ILinePersistenceSvc>                                      m_linePersistenceSvc;
  mutable std::map<unsigned int, ILinePersistenceSvc::LineDecNames> m_filteredLinesCache;
};

DECLARE_COMPONENT( DecisionBankKiller )

//=============================================================================
// Standard creator, initializes variables
//=============================================================================
DecisionBankKiller::DecisionBankKiller( const std::string& name, ISvcLocator* pSvcLocator )
    : Consumer( name, pSvcLocator, KeyValue{"InputHltDecReportsLocation", LHCb::HltDecReportsLocation::Default} ) {
  m_rawEventLocations = {LHCb::RawEventLocation::Default};
  initRawEventSearch();
}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode DecisionBankKiller::initialize() {
  StatusCode sc = Decoder::AlgBase::initialize();
  if ( sc.isFailure() ) return sc;

  if ( m_rawEventLocations.empty() )
    return Error( "You didn't give me a list of RawEventLocations to look through", StatusCode::FAILURE );

  if ( m_killFromAll ) {
    info() << "Killing banks from everything in '" << m_rawEventLocations << "'" << endmsg;
  } else if ( m_rawEventLocations.size() != 1 || m_rawEventLocations[0] != LHCb::RawEventLocation::Default ) {
    info() << "Killing banks from first raw event in '" << m_rawEventLocations << "'" << endmsg;
  }

  m_linePersistenceSvc = svc<ILinePersistenceSvc>( m_linePersistenceSvcName.value() );
  if ( !m_linePersistenceSvc ) {
    throw GaudiException( "Could not acquire ILinePersistenceSvc", this->name(), StatusCode::FAILURE );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
void DecisionBankKiller::operator()( const LHCb::HltDecReports& decReports ) const {
  ReqBanksBitset reqBanks( m_alwaysKeepBanksSet );

  auto tck         = decReports.configuredTCK();
  auto cache_entry = m_filteredLinesCache.find( tck );
  if ( cache_entry == std::end( m_filteredLinesCache ) ) {
    auto lines  = filteredLines( decReports );
    auto status = m_filteredLinesCache.emplace( tck, lines );
    if ( !status.second ) {
      error() << "Error updating cache for TCK " << tck << " Returning an empty map... " << endmsg;
      throw GaudiException( "FATAL error", this->name(), StatusCode::FAILURE );
    }
    cache_entry = status.first;
  }

  reqBanks |= m_linePersistenceSvc->rawBanksToPersist( decReports, cache_entry->second );

  for ( auto loc : m_rawEventLocations ) {
    auto* rawEvt = tryEventAt( loc );
    if ( rawEvt ) {
      killFromRawEvent( rawEvt, reqBanks );
      if ( !m_killFromAll ) break;
    }
  }
}

void DecisionBankKiller::killFromRawEvent( LHCb::RawEvent* rawEvent, const ReqBanksBitset& reqBanks ) const {
  //
  for ( unsigned int ibank = 0; ibank < (unsigned int)LHCb::RawBank::LastType; ++ibank ) {
    if ( reqBanks[ibank] ) continue;
    LHCb::RawBank::BankType bankType = (LHCb::RawBank::BankType)ibank;
    killBankType( rawEvent, bankType, false );
  }
}

void DecisionBankKiller::killBankType( LHCb::RawEvent* rawEvent, LHCb::RawBank::BankType bankType,
                                       bool warningmsg ) const {
  const std::string bankTypeName = LHCb::RawBank::typeName( bankType );
  // look for all banks of this type and remove them
  const auto& banks = rawEvent->banks( bankType );
  if ( bankType == LHCb::RawBank::LastType || 0 == banks.size() ) {
    if ( warningmsg ) {
      std::stringstream s( "" );
      s << " No bank of type '" << bankTypeName << "' has been found - nothing to be removed";
      Warning( s.str(), StatusCode::SUCCESS, 0 ).ignore();
    }
    return;
  }

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "All banks of type '" << bankTypeName << "'  are to be removed - banks size =  " << banks.size()
            << endmsg;

  // note that we need to _copy_ the list of banks, as the original will be modified
  // due to calling removeBank while looping ...
  for ( const LHCb::RawBank* bnk : LHCb::RawBank::ConstVector{banks.begin(), banks.end()} ) {
    bool success = rawEvent->removeBank( bnk );
    if ( !success && warningmsg ) {
      if ( msgLevel( MSG::DEBUG ) )
        debug() << "The bank " << bnk << " of type '" << bankTypeName << "' has not been found to be removed "
                << endmsg;
      Warning( "The bank of type '" + bankTypeName + "' has not been found", StatusCode::SUCCESS, 0 ).ignore();
    }
  }
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "All banks of type '" << bankTypeName << "' have been removed  - "
            << " banks size is now : " << ( rawEvent->banks( bankType ) ).size() << endmsg;
}

ILinePersistenceSvc::LineDecNames DecisionBankKiller::filteredLines( const LHCb::HltDecReports& hdr ) const {
  //
  ILinePersistenceSvc::LineDecNames lines;

  for ( const auto& report_pair : hdr ) {
    const auto& report = report_pair.second;
    // keep only the turbo bit from the execution stage
    int                turbo = report.executionStage() & 0x80;
    LHCb::HltDecReport dummy_report{true, turbo, 0, 1, static_cast<int>( report.intDecisionID() )};

    LHCb::HltDecReports dummy_hdr{};
    dummy_hdr.insert( report_pair.first, dummy_report ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    dummy_hdr.setConfiguredTCK( hdr.configuredTCK() );
    if ( m_lineFilterTool->predicate( dummy_hdr ) ) { lines.insert( report_pair.first ); }
  }

  return lines;
}

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
#include <algorithm>
#include <vector>

// local
#include "Kernel/ISTReadoutTool.h"
#include "RawBankToSTLiteClusterAlg.h"

#include "LHCbMath/LHCbMath.h"

// Event
#include "Event/RawEvent.h"
#include "Event/STLiteCluster.h"

#include "Kernel/STDAQDefinitions.h"
#include "Kernel/STDataFunctor.h"
#include "Kernel/STDecoder.h"
#include "Kernel/STDetSwitch.h"
#include "Kernel/STTell1Board.h"
#include "Kernel/STTell1ID.h"
#include "Kernel/StripRepresentation.h"

#include "Kernel/FastClusterContainer.h"

using namespace LHCb;

namespace {
  struct Less_by_Channel {

    /** compare the channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    //
    inline bool operator()( LHCb::STLiteCluster obj1, LHCb::STLiteCluster obj2 ) const {
      return obj1.channelID() < obj2.channelID();
    }
  };
  struct Equal_Channel {

    /** compare the channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    //
    inline bool operator()( LHCb::STLiteCluster obj1, LHCb::STLiteCluster obj2 ) const {
      return obj1.channelID() == obj2.channelID();
    }
  };

} // namespace

//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToSTLiteClusterAlg
//
// 2004-01-07 : Matthew Needham
// 2016-10-07 : Sebastien Ponce
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( RawBankToSTLiteClusterAlg )

RawBankToSTLiteClusterAlg::RawBankToSTLiteClusterAlg( const std::string& name, ISvcLocator* pSvcLocator )
    : Transformer( name, pSvcLocator,
                   {KeyValue{"OdinLocation", LHCb::ODINLocation::Default},
                    KeyValue{"RawEventLocations", Gaudi::Functional::concat_alternatives(
                                                      LHCb::RawEventLocation::Tracker, LHCb::RawEventLocation::Other,
                                                      LHCb::RawEventLocation::Default )}},
                   KeyValue( "clusterLocation", STLiteClusterLocation::TTClusters ) ) {
  // Standard constructor, initializes variables
  declareSTConfigProperty( "BankType", m_bankTypeString, detType() );
}

StatusCode RawBankToSTLiteClusterAlg::initialize() {
  // Initialization
  StatusCode sc = Transformer::initialize();
  if ( sc.isFailure() ) return Error( "Failed to initialize", sc );
  // Spill
  computeSpillOffset( inputLocation<1>() );
  // return
  return StatusCode::SUCCESS;
}

LHCb::STLiteCluster::STLiteClusters RawBankToSTLiteClusterAlg::operator()( const LHCb::ODIN&     odin,
                                                                           const LHCb::RawEvent& rawEvt ) const {
  STLiteCluster::STLiteClusters fCont;
  if ( !validSpill( odin ) ) {
    warning() << "Not a valid spill" << endmsg;
  } else {
    fCont.reserve( 5000 );
    // decode banks
    StatusCode sc = decodeBanks( rawEvt, fCont );
    if ( sc.isFailure() ) {
      throw GaudiException( "Problems in decoding event skipped", "RawBankToSTLiteClusterAlg", StatusCode::FAILURE );
    }
  }
  return fCont;
}

StatusCode RawBankToSTLiteClusterAlg::decodeBanks( const RawEvent&                rawEvt,
                                                   STLiteCluster::STLiteClusters& fCont ) const {
  std::unique_ptr<LHCb::STTELL1BoardErrorBanks> errorBanks       = nullptr;
  bool                                          errorBanksFailed = false;

  const LHCb::span<const RawBank*> tBanks  = rawEvt.banks( bankType() );
  std::vector<unsigned int>        missing = missingInAction( tBanks );
  if ( !missing.empty() ) {
    m_lostBanks += missing.size();
    if ( tBanks.empty() ) {
      ++m_noBanksFound;
      return StatusCode::SUCCESS;
    }
  }

  const unsigned int pcn = pcnVote( tBanks );
  if ( msgLevel( MSG::DEBUG ) ) debug() << "PCN was voted to be " << pcn << endmsg;
  if ( pcn == STDAQ::inValidPcn ) {
    m_skippedBanks += tBanks.size();
    return Warning( "PCN vote failed", StatusCode::SUCCESS, 2 );
  }

  const bool isUT = ( detType() == "UT" );

  // loop over the banks of this type..

  for ( auto iterBank = tBanks.begin(); iterBank != tBanks.end(); ++iterBank ) {

    ++m_validBanks;

    // get the board and data
    STTell1Board* aBoard = readoutTool()->findByBoardID( STTell1ID( ( *iterBank )->sourceID() ) );
    if ( !aBoard && !m_skipErrors ) {
      Warning( "Invalid source ID --> skip bank" + std::to_string( ( *iterBank )->sourceID() ), StatusCode::SUCCESS, 2 )
          .ignore();
      m_skippedBanks += 1;
      continue;
    }

    ++m_validSourceID;

    if ( ( *iterBank )->magic() != RawBank::MagicPattern ) {
      Warning( "wrong magic pattern " + std::to_string( ( *iterBank )->sourceID() ), StatusCode::SUCCESS, 2 ).ignore();
      m_skippedBanks += tBanks.size();
      continue;
    }

    // make a SmartBank of shorts...
    STDecoder decoder( ( *iterBank )->data() );

    bool recover = false;
    if ( decoder.hasError() && !m_skipErrors ) {

      if ( !recoverMode() ) {
        Warning( "bank has errors, skip sourceID " + std::to_string( ( *iterBank )->sourceID() ), StatusCode::SUCCESS,
                 2 )
            .ignore();
        m_skippedBanks += 1;
        continue;
      } else {
        // flag that need to recover....
        recover = true;
        ++counter( "recovered banks" + std::to_string( ( *iterBank )->sourceID() ) );
      }
    }

    // ok this is a bit ugly.....
    STTELL1BoardErrorBank* errorBank = nullptr;
    if ( recover ) {
      if ( !errorBanks.get() && !errorBanksFailed ) {
        try {
          errorBanks = decodeErrors( rawEvt );
        } catch ( GaudiException& e ) {
          errorBanksFailed = true;
          warning() << e.what() << endmsg;
        }
      }
      if ( errorBanks.get() ) { errorBank = errorBanks->object( ( *iterBank )->sourceID() ); }
    }

    if ( errorBank ) {
      const unsigned bankpcn = decoder.header().pcn();
      if ( pcn != bankpcn && !m_skipErrors ) {
        if ( msgLevel( MSG::DEBUG ) ) debug() << "Expected " << pcn << " found " << bankpcn << endmsg;
        Warning( "PCNs out of sync sourceID " + std::to_string( ( *iterBank )->sourceID() ), StatusCode::SUCCESS, 2 )
            .ignore();
        m_skippedBanks += 1;
        continue;
      }
    } // errorbank == 0

    const STDAQ::version bankVersion = STDAQ::version( forceVersion() ? m_forcedVersion : ( *iterBank )->version() );

    // check the integrity of the bank --> always skip if not ok
    if ( !m_skipErrors && !checkDataIntegrity( decoder, aBoard, ( *iterBank )->size(), bankVersion ) ) continue;

    // read in the first half of the bank
    for ( auto iterDecoder : decoder.posRange() ) {

      if ( !recover ) {
        createCluster( aBoard, bankVersion, iterDecoder, fCont, isUT );
      } else {
        if ( errorBank && canBeRecovered( errorBank, iterDecoder, pcn ) ) {
          createCluster( aBoard, bankVersion, iterDecoder, fCont, isUT );
        } // errorbank
      }   // recover == false

    } // decoder

  } // iterBank

  // sort and remove any duplicates
  std::stable_sort( fCont.begin(), fCont.end(), Less_by_Channel() );
  auto iter = std::unique( fCont.begin(), fCont.end(), Equal_Channel() );
  if ( iter != fCont.end() ) {
    fCont.resize( iter - fCont.begin() );
    return Warning( "Removed duplicate clusters in the decoding", StatusCode::SUCCESS, 100 );
  }
  return StatusCode::SUCCESS;
}

StatusCode RawBankToSTLiteClusterAlg::finalize() {

  const double failed    = m_skippedBanks.sum();
  const double processed = m_validBanks.nEntries();
  double       eff       = ( !LHCb::Math::Equal_To<double>()( processed, 0.0 ) ? 1.0 - ( failed / processed ) : 0.0 );
  info() << "Successfully processed " << 100 * eff << " %" << endmsg;

  return Transformer::finalize();
}

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

// local
#include "UTDAQ/RawBankToUTClusterAlg.h"

// Event
#include "Event/RawEvent.h"
#include "Event/UTCluster.h"
#include "Event/UTSummary.h"

#include "Event/UTLiteCluster.h"
#include "Kernel/IUTReadoutTool.h"
#include "Kernel/UTDataFunctor.h"
#include "Kernel/UTDecoder.h"
#include "Kernel/UTFun.h"
#include "Kernel/UTRawBankMap.h"
#include "Kernel/UTStripRepresentation.h"
#include "Kernel/UTTell1Board.h"
#include "Kernel/UTTell1ID.h"
#include "LHCbMath/LHCbMath.h"
#include "SiDAQ/SiADCWord.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToUTClusterAlg
//
// 2004-01-07 : Matthew Needham
// 2016-10-07 : Sebastien Ponce
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( RawBankToUTClusterAlg )

RawBankToUTClusterAlg::RawBankToUTClusterAlg( const std::string& name, ISvcLocator* pSvcLocator )
    : MultiTransformer(
          name, pSvcLocator,
          {KeyValue{"OdinLocation", LHCb::ODINLocation::Default},
           KeyValue{"RawEventLocations", Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Tracker,
                                                                                 LHCb::RawEventLocation::Other,
                                                                                 LHCb::RawEventLocation::Default )}},
          {KeyValue( "clusterLocation", UTClusterLocation::UTClusters ),
           KeyValue( "summaryLocation", UTSummaryLocation::UTSummary )} ) {
  // Standard constructor, initializes variables
  declareUTConfigProperty( "BankType", m_bankTypeString, "UT" );
  declareUTConfigProperty( "PedestalBank", m_pedestalBankString, "UTPedestal" );
  declareUTConfigProperty( "FullBank", m_fullBankString, "UTFull" );
}

StatusCode RawBankToUTClusterAlg::initialize() {
  // Initialization
  StatusCode sc = MultiTransformer::initialize();
  if ( sc.isFailure() ) return Error( "Failed to initialize", sc );
  // pedestal bank
  m_pedestalType = UTRawBankMap::stringToType( m_pedestalBankString );
  if ( m_bankType == LHCb::RawBank::Velo ) {
    fatal() << "Wrong detector type: only UT !" << endmsg;
    return StatusCode::FAILURE;
  }
  // full bank
  m_fullType = UTRawBankMap::stringToType( m_fullBankString );
  if ( m_fullType == LHCb::RawBank::Velo ) {
    fatal() << "Wrong detector type: only UT!" << endmsg;
    return StatusCode::FAILURE;
  }
  // Spill
  computeSpillOffset( inputLocation<1>() );
  // return
  return StatusCode::SUCCESS;
}

std::tuple<LHCb::UTClusters, LHCb::UTSummary> RawBankToUTClusterAlg::operator()( const LHCb::ODIN&     odin,
                                                                                 const LHCb::RawEvent& rawEvt ) const {
  // make a new digits container
  UTClusters clusCont;
  if ( !validSpill( odin ) ) {
    warning() << "Not a valid spill" << endmsg;
  } else {
    clusCont.reserve( 2000 );
    // decode banks
    LHCb::UTSummary summary = decodeBanks( rawEvt, clusCont );
    // sort
    std::sort( clusCont.begin(), clusCont.end(), UTDataFunctor::Less_by_Channel<const UTCluster*>() );
    return std::make_tuple( std::move( clusCont ), std::move( summary ) );
  }
  return std::make_tuple( std::move( clusCont ), LHCb::UTSummary() );
}

LHCb::UTSummary RawBankToUTClusterAlg::decodeBanks( const RawEvent& rawEvt, LHCb::UTClusters& clusCont ) const {
  std::unique_ptr<LHCb::UTTELL1BoardErrorBanks> errorBanks       = nullptr;
  bool                                          errorBanksFailed = false;

  // create Clusters from this type
  bool                      pcnSync = true;
  std::vector<unsigned int> bankList;
  UTSummary::RecoveredInfo  recoveredBanks;

  const auto& tBanks = rawEvt.banks( bankType() );

  std::vector<unsigned int> missing = missingInAction( tBanks );
  if ( !missing.empty() ) {
    counter( "lost Banks" ) += missing.size();
    if ( tBanks.empty() ) {
      ++counter( "no banks found" );
      return createSummaryBlock( rawEvt, 0, UTDAQ::inValidPcn, false, 0, bankList, missing, recoveredBanks );
    }
  }

  // vote on the pcns
  const unsigned int pcn = pcnVote( tBanks );
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "PCN was voted to be " << pcn << endmsg;
  if ( pcn == UTDAQ::inValidPcn && !m_skipErrors ) {
    counter( "skipped Banks" ) += tBanks.size();
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "PCN vote failed with " << tBanks.size() << endmsg;
    warning() << "PCN vote failed" << endmsg;
    return UTSummary();
  }

  // loop over the banks of this type..
  for ( const auto& bank : tBanks ) {

    ++counter( "# banks found" );
    // get the board and data
    UTTell1ID           tell1ID = UTTell1ID( (unsigned int)bank->sourceID(), detType() == "UT" );
    const UTTell1Board* aBoard  = readoutTool()->findByBoardID( tell1ID );

    if ( !aBoard && !m_skipErrors ) {
      // not a valid UT
      Warning( "Invalid source ID --> skip bank" + std::to_string( bank->sourceID() ), StatusCode::SUCCESS, 2 )
          .ignore();
      ++counter( "skipped Banks" );
      continue;
    }

    ++counter( "# valid banks" );

    if ( bank->magic() != RawBank::MagicPattern ) {
      Warning( "wrong magic pattern " + std::to_string( bank->sourceID() ), StatusCode::SUCCESS, 2 ).ignore();
      ++counter( "skipped banks" );
      continue;
    }

    // make a decoder
    UTDecoder decoder( bank->data() );
    // get verion of the bank
    const UTDAQ::version bankVersion =
        forceVersion() ? UTDAQ::version( m_forcedVersion.value() ) : UTDAQ::version( bank->version() );

    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "decoding bank version " << bankVersion << endmsg;

    bool recover = false;
    if ( decoder.hasError() == true && !m_skipErrors ) {

      if ( !recoverMode() ) {
        bankList.push_back( bank->sourceID() );
        Warning( "bank has errors, skip sourceID " + std::to_string( bank->sourceID() ), StatusCode::SUCCESS, 2 )
            .ignore();
        ++counter( "skipped Banks" );
        continue;
      } else {
        // flag that need to recover....
        recover = true;
        ++counter( "recovered banks" + std::to_string( bank->sourceID() ) );
      }
    }

    UTTELL1BoardErrorBank* errorBank = nullptr;
    if ( recover ) {
      if ( !errorBanks.get() && !errorBanksFailed ) {
        try {
          errorBanks = decodeErrors( rawEvt );
        } catch ( GaudiException& e ) {
          errorBanksFailed = true;
          warning() << e.what() << endmsg;
        }
      }
      if ( errorBanks.get() ) { errorBank = errorBanks->object( bank->sourceID() ); }
      // check what fraction we can recover
      if ( errorBank != 0 ) recoveredBanks[bank->sourceID()] += errorBank->fractionOK( pcn );
    }

    if ( errorBank == 0 ) {
      const unsigned bankpcn = decoder.header().pcn();
      if ( pcn != bankpcn && !m_skipErrors ) {
        bankList.push_back( bank->sourceID() );
        if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Expected " << pcn << " found " << bankpcn << endmsg;
        Warning( "PCNs out of sync, sourceID " + std::to_string( bank->sourceID() ), StatusCode::SUCCESS, 2 ).ignore();
        ++counter( "skipped Banks" );
        continue;
      }
    }

    // check the integrity of the bank --> always skip if not ok
    if ( !m_skipErrors && checkDataIntegrity( decoder, aBoard, bank->size(), bankVersion ) == false ) {
      bankList.push_back( bank->sourceID() );
      continue;
    }

    // iterator over the data....
    for ( auto iterDecoder = decoder.posAdcBegin(); iterDecoder != decoder.posAdcEnd(); ++iterDecoder ) {
      if ( !recover ) {
        createCluster( iterDecoder->first, aBoard, iterDecoder->second, bankVersion, clusCont );
      } else {
        // check that this cluster is ok to be recovered
        if ( errorBank != 0 && canBeRecovered( errorBank, iterDecoder->first, pcn ) == true ) {
          createCluster( iterDecoder->first, aBoard, iterDecoder->second, bankVersion, clusCont );
        }
      }
    } // iterDecoder
  }   // bank

  const unsigned int bsize = byteSize( tBanks );
  return createSummaryBlock( rawEvt, clusCont.size(), pcn, pcnSync, bsize, bankList, missing, recoveredBanks );
}

void RawBankToUTClusterAlg::createCluster( const UTClusterWord& aWord, const UTTell1Board* aBoard,
                                           LHCb::span<const SiADCWord> adcValues, const UTDAQ::version& bankVersion,
                                           UTClusters& clusCont ) const {
  // stream the neighbour sum
  auto iterADC   = adcValues.begin();
  char neighbour = *iterADC;
  ++iterADC;

  unsigned int fracStrip = aWord.fracStripBits();

  // estimate the offset
  double stripNum      = mean( adcValues );
  double interStripPos = stripNum - floor( stripNum );

  // If fracStrip equals zero and the interStripPos equals one, the stripNum
  // must be incremented. Note that since the rounding can be different from
  // rounding on the Tell1, the interStripPos can be 0.75. Trust me, this is
  // correct.-- JvT
  if ( fracStrip == 0u && interStripPos > 0.5 ) stripNum += 1;
  unsigned int offset = (unsigned int)stripNum;

  UTCluster::ADCVector adcs;
  adcs.reserve( adcValues.size() );
  for ( unsigned int i = 1; i < adcValues.size(); ++i ) {
    adcs.emplace_back( i - 1 - offset, (int)adcValues[i].adc() );
  } // iDigit

  UTTell1Board::chanPair nearestChan =
      aBoard->DAQToOffline( fracStrip, bankVersion, UTDAQ::UTStripRepresentation( aWord.channelID() ) );

  aBoard->ADCToOffline( aWord.channelID(), adcs, bankVersion, offset, fracStrip );

  // make cluster +set things
  auto newCluster = std::make_unique<UTCluster>( this->word2LiteCluster( aWord, nearestChan.first, nearestChan.second ),
                                                 adcs, neighbour, aBoard->boardID().id(), aWord.channelID(), spill() );

  if ( !clusCont.object( nearestChan.first ) ) {
    clusCont.insert( newCluster.release(), nearestChan.first );
  } else {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
      debug() << "Cluster already exists not inserted: " << aBoard->boardID() << " " << aWord.channelID() << endmsg;
    Warning( "Failed to insert cluster --> exists in container", StatusCode::SUCCESS, 100 ).ignore();
  }
}

LHCb::UTSummary RawBankToUTClusterAlg::createSummaryBlock(
    const RawEvent& rawEvt, const unsigned int& nclus, const unsigned int& pcn, const bool pcnsync,
    const unsigned int bytes, const std::vector<unsigned int>& bankList, const std::vector<unsigned int>& missing,
    const LHCb::UTSummary::RecoveredInfo& recoveredBanks ) const {
  unsigned totalBytes = bytes;
  // get the error banks
  const auto& errorBanks = rawEvt.banks( LHCb::RawBank::BankType( m_errorType ) );
  totalBytes += byteSize( errorBanks );
  // get the pedestal banks
  const auto& pBanks = rawEvt.banks( LHCb::RawBank::BankType( m_pedestalType ) );
  totalBytes += byteSize( pBanks );
  // get the full banks
  const auto& fullBanks = rawEvt.banks( LHCb::RawBank::BankType( m_fullType ) );
  totalBytes += byteSize( fullBanks );
  return UTSummary( nclus, pcn, pcnsync, totalBytes, fullBanks.size(), pBanks.size(), errorBanks.size(), bankList,
                    missing, recoveredBanks );
}

double RawBankToUTClusterAlg::mean( LHCb::span<const SiADCWord> adcValues ) const {
  double sum       = 0;
  double totCharge = 0;
  // note the first is the neighbour sum..
  for ( unsigned int i = 1; i < adcValues.size(); ++i ) {
    sum += adcValues[i].adc() * ( i - 1 );
    totCharge += adcValues[i].adc();
  } // i
  return ( sum / totCharge );
}

StatusCode RawBankToUTClusterAlg::finalize() {

  const double failed    = counter( "skipped Banks" ).flag();
  const double processed = counter( "# valid banks" ).flag();

  double eff = 0.0;
  if ( !LHCb::Math::Equal_To<double>()( processed, 0.0 ) ) { eff = 1.0 - ( failed / processed ); }
  info() << "Successfully processed " << 100 * eff << " %" << endmsg;

  return MultiTransformer::finalize();
}

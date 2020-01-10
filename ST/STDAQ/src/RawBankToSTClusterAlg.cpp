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
#include "RawBankToSTClusterAlg.h"

// Event
#include "Event/RawEvent.h"
#include "Event/STCluster.h"
#include "Event/STSummary.h"

#include "Event/STLiteCluster.h"
#include "Kernel/ISTReadoutTool.h"
#include "Kernel/STDataFunctor.h"
#include "Kernel/STDecoder.h"
#include "Kernel/STFun.h"
#include "Kernel/STRawBankMap.h"
#include "Kernel/STTell1Board.h"
#include "Kernel/STTell1ID.h"
#include "Kernel/StripRepresentation.h"
#include "LHCbMath/LHCbMath.h"
#include "SiDAQ/SiADCWord.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToSTClusterAlg
//
// 2004-01-07 : Matthew Needham
// 2016-10-07 : Sebastien Ponce
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( RawBankToSTClusterAlg )

RawBankToSTClusterAlg::RawBankToSTClusterAlg( const std::string& name, ISvcLocator* pSvcLocator )
    : MultiTransformer(
          name, pSvcLocator,
          {KeyValue{"OdinLocation", LHCb::ODINLocation::Default},
           KeyValue{"RawEventLocations", Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Tracker,
                                                                                 LHCb::RawEventLocation::Other,
                                                                                 LHCb::RawEventLocation::Default )}},
          {KeyValue( "clusterLocation", STClusterLocation::TTClusters ),
           KeyValue( "summaryLocation", STSummaryLocation::TTSummary )} ) {
  // Standard constructor, initializes variables
  declareSTConfigProperty( "BankType", m_bankTypeString, "TT" );
  declareSTConfigProperty( "PedestalBank", m_pedestalBankString, "TTPedestal" );
  declareSTConfigProperty( "FullBank", m_fullBankString, "TTFull" );
}

StatusCode RawBankToSTClusterAlg::initialize() {
  // Initialization
  StatusCode sc = MultiTransformer::initialize();
  if ( sc.isFailure() ) return Error( "Failed to initialize", sc );
  // pedestal bank
  m_pedestalType = STRawBankMap::stringToType( m_pedestalBankString );
  if ( m_bankType == LHCb::RawBank::Velo ) {
    fatal() << "Wrong detector type: only IT or TT !" << endmsg;
    return StatusCode::FAILURE;
  }
  // full bank
  m_fullType = STRawBankMap::stringToType( m_fullBankString );
  if ( m_fullType == LHCb::RawBank::Velo ) {
    fatal() << "Wrong detector type: only IT or TT !" << endmsg;
    return StatusCode::FAILURE;
  }
  // Spill
  computeSpillOffset( inputLocation<1>() );
  // return
  return StatusCode::SUCCESS;
}

std::tuple<LHCb::STClusters, LHCb::STSummary> RawBankToSTClusterAlg::operator()( const LHCb::ODIN&     odin,
                                                                                 const LHCb::RawEvent& rawEvt ) const {
  // make a new digits container
  STClusters clusCont;
  if ( !validSpill( odin ) ) {
    warning() << "Not a valid spill" << endmsg;
  } else {
    clusCont.reserve( 2000 );
    // decode banks
    LHCb::STSummary summary = decodeBanks( rawEvt, clusCont );
    // sort
    std::sort( clusCont.begin(), clusCont.end(), STDataFunctor::Less_by_Channel<const STCluster*>() );
    return std::make_tuple( std::move( clusCont ), std::move( summary ) );
  }
  return std::make_tuple( std::move( clusCont ), LHCb::STSummary() );
}

LHCb::STSummary RawBankToSTClusterAlg::decodeBanks( const RawEvent& rawEvt, LHCb::STClusters& clusCont ) const {
  std::unique_ptr<LHCb::STTELL1BoardErrorBanks> errorBanks       = nullptr;
  bool                                          errorBanksFailed = false;

  // create Clusters from this type
  bool                      pcnSync = true;
  std::vector<unsigned int> bankList;
  STSummary::RecoveredInfo  recoveredBanks;

  const auto& tBanks = rawEvt.banks( bankType() );

  std::vector<unsigned int> missing = missingInAction( tBanks );
  if ( !missing.empty() ) {
    m_lostBanks += missing.size();
    if ( tBanks.empty() ) {
      ++m_noBanksFound;
      return createSummaryBlock( rawEvt, 0, STDAQ::inValidPcn, false, 0, bankList, missing, recoveredBanks );
    }
  }

  // vote on the pcns
  const unsigned int pcn = pcnVote( tBanks );
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "PCN was voted to be " << pcn << endmsg;
  if ( pcn == STDAQ::inValidPcn && !m_skipErrors ) {
    m_skippedBanks += tBanks.size();
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "PCN vote failed with " << tBanks.size() << endmsg;
    warning() << "PCN vote failed" << endmsg;
    return STSummary();
  }

  // loop over the banks of this type..
  for ( const auto& bank : tBanks ) {

    ++m_banksFound;
    // get the board and data
    STTell1ID           tell1ID = STTell1ID( (unsigned int)bank->sourceID(), detType() == "UT" );
    const STTell1Board* aBoard  = readoutTool()->findByBoardID( tell1ID );

    if ( !aBoard && !m_skipErrors ) {
      // not a valid IT
      Warning( "Invalid source ID --> skip bank" + std::to_string( bank->sourceID() ), StatusCode::SUCCESS, 2 )
          .ignore();
      m_skippedBanks += 1;
      continue;
    }

    ++m_validBanks;

    if ( bank->magic() != RawBank::MagicPattern ) {
      Warning( "wrong magic pattern " + std::to_string( bank->sourceID() ), StatusCode::SUCCESS, 2 ).ignore();
      m_skippedBanks += 1;
      continue;
    }

    // make a decoder
    STDecoder decoder( bank->data() );
    // get verion of the bank
    const STDAQ::version bankVersion =
        forceVersion() ? STDAQ::version( m_forcedVersion ) : STDAQ::version( bank->version() );

    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "decoding bank version " << bankVersion << endmsg;

    bool recover = false;
    if ( decoder.hasError() == true && !m_skipErrors ) {

      if ( !recoverMode() ) {
        bankList.push_back( bank->sourceID() );
        Warning( "bank has errors, skip sourceID " + std::to_string( bank->sourceID() ), StatusCode::SUCCESS, 2 )
            .ignore();
        m_skippedBanks += 1;
        continue;
      } else {
        // flag that need to recover....
        recover = true;
        ++counter( "recovered banks" + std::to_string( bank->sourceID() ) );
      }
    }

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
        m_skippedBanks += 1;
        continue;
      }
    }

    // check the integrity of the bank --> always skip if not ok
    if ( !m_skipErrors && !checkDataIntegrity( decoder, aBoard, bank->size(), bankVersion ) ) {
      bankList.push_back( bank->sourceID() );
      continue;
    }

    // iterator over the data....
    for ( const auto& iterDecoder : decoder.posAdcRange() ) {
      if ( !recover ) {
        createCluster( iterDecoder.first, aBoard, iterDecoder.second, bankVersion, clusCont );
      } else {
        // check that this cluster is ok to be recovered
        if ( errorBank != 0 && canBeRecovered( errorBank, iterDecoder.first, pcn ) ) {
          createCluster( iterDecoder.first, aBoard, iterDecoder.second, bankVersion, clusCont );
        }
      }
    } // iterDecoder
  }   // bank

  const unsigned int bsize = byteSize( tBanks );
  return createSummaryBlock( rawEvt, clusCont.size(), pcn, pcnSync, bsize, bankList, missing, recoveredBanks );
}

void RawBankToSTClusterAlg::createCluster( const STClusterWord& aWord, const STTell1Board* aBoard,
                                           LHCb::span<const SiADCWord> adcValues, const STDAQ::version& bankVersion,
                                           STClusters& clusCont ) const {
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

  STCluster::ADCVector adcs;
  adcs.reserve( adcValues.size() );
  for ( unsigned int i = 1; i < adcValues.size(); ++i ) {
    adcs.emplace_back( i - 1 - offset, (int)adcValues[i].adc() );
  } // iDigit

  STTell1Board::chanPair nearestChan =
      aBoard->DAQToOffline( fracStrip, bankVersion, STDAQ::StripRepresentation( aWord.channelID() ) );

  aBoard->ADCToOffline( aWord.channelID(), adcs, bankVersion, offset, fracStrip );

  // make cluster +set things
  auto newCluster = std::make_unique<STCluster>( this->word2LiteCluster( aWord, nearestChan.first, nearestChan.second ),
                                                 adcs, neighbour, aBoard->boardID().id(), aWord.channelID(), spill() );

  if ( !clusCont.object( nearestChan.first ) ) {
    clusCont.insert( newCluster.release(), nearestChan.first );
  } else {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
      debug() << "Cluster already exists not inserted: " << aBoard->boardID() << " " << aWord.channelID() << endmsg;
    Warning( "Failed to insert cluster --> exists in container", StatusCode::SUCCESS, 100 ).ignore();
  }
}

LHCb::STSummary RawBankToSTClusterAlg::createSummaryBlock(
    const RawEvent& rawEvt, const unsigned int& nclus, const unsigned int& pcn, const bool pcnsync,
    const unsigned int bytes, const std::vector<unsigned int>& bankList, const std::vector<unsigned int>& missing,
    const LHCb::STSummary::RecoveredInfo& recoveredBanks ) const {
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
  return STSummary( nclus, pcn, pcnsync, totalBytes, fullBanks.size(), pBanks.size(), errorBanks.size(), bankList,
                    missing, recoveredBanks );
}

double RawBankToSTClusterAlg::mean( LHCb::span<const SiADCWord> adcValues ) const {
  double sum       = 0;
  double totCharge = 0;
  // note the first is the neighbour sum..
  for ( unsigned int i = 1; i < adcValues.size(); ++i ) {
    sum += adcValues[i].adc() * ( i - 1 );
    totCharge += adcValues[i].adc();
  } // i
  return ( sum / totCharge );
}

StatusCode RawBankToSTClusterAlg::finalize() {

  const double failed    = m_skippedBanks.sum();
  const double processed = m_validBanks.nEntries();

  double eff = 0.0;
  if ( !LHCb::Math::Equal_To<double>()( processed, 0.0 ) ) { eff = 1.0 - ( failed / processed ); }
  info() << "Successfully processed " << 100 * eff << " %" << endmsg;

  return MultiTransformer::finalize();
}

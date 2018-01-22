#include <algorithm>
#include <vector>

// local
#include "RawBankToSTLiteClusterAlg.h"
#include "Kernel/ISTReadoutTool.h"

#include "LHCbMath/LHCbMath.h"

// Event
#include "Event/RawEvent.h"
#include "Event/STLiteCluster.h"


#include "Kernel/STTell1Board.h"
#include "Kernel/STTell1ID.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/STDecoder.h"
#include "Kernel/STDetSwitch.h"
#include "Kernel/STDataFunctor.h"
#include "Kernel/StripRepresentation.h"

#include "Kernel/FastClusterContainer.h"

using namespace LHCb;

namespace {
  struct Less_by_Channel  {

    /** compare the channel of one object with the
     *  channel of another object
     *  @param obj1   first  object
     *  @param obj2   second object
     *  @return  result of the comparision
     */
    //
    inline bool operator() ( LHCb::STLiteCluster obj1 , LHCb::STLiteCluster obj2 ) const
    {
      return obj1.channelID() < obj2.channelID() ;
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
    inline bool operator() ( LHCb::STLiteCluster obj1 , LHCb::STLiteCluster obj2 ) const
    {
      return obj1.channelID() == obj2.channelID() ;
    }
  };

}

//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToSTLiteClusterAlg
//
// 2004-01-07 : Matthew Needham
// 2016-10-07 : Sebastien Ponce
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( RawBankToSTLiteClusterAlg )

RawBankToSTLiteClusterAlg::RawBankToSTLiteClusterAlg(const std::string& name,
                                                     ISvcLocator* pSvcLocator) :
Transformer(name, pSvcLocator,
            {KeyValue{"OdinLocation", LHCb::ODINLocation::Default},
             KeyValue{"RawEventLocations",
                      Gaudi::Functional::concat_alternatives(LHCb::RawEventLocation::Tracker,
                                                             LHCb::RawEventLocation::Other,
                                                             LHCb::RawEventLocation::Default)}},
            KeyValue("clusterLocation", STLiteClusterLocation::TTClusters)) {
  // Standard constructor, initializes variables
  declareSTConfigProperty("BankType", m_bankTypeString , detType() );
}

StatusCode RawBankToSTLiteClusterAlg::initialize() {
  // Initialization
  StatusCode sc = Transformer::initialize();
  if (sc.isFailure()) return Error("Failed to initialize", sc);
  // Spill
  computeSpillOffset(inputLocation<1>());
  // return
  return StatusCode::SUCCESS;
}

LHCb::STLiteCluster::STLiteClusters RawBankToSTLiteClusterAlg::operator()(const LHCb::ODIN& odin, const LHCb::RawEvent& rawEvt) const {
  STLiteCluster::STLiteClusters fCont;
  if (!validSpill(odin)) {
    warning() << "Not a valid spill" << endmsg;
  } else {
    fCont.reserve(5000);
    // decode banks
    StatusCode sc = decodeBanks(rawEvt, fCont);
    if (sc.isFailure()){
      throw GaudiException("Problems in decoding event skipped", "RawBankToSTLiteClusterAlg", StatusCode::FAILURE );
    }
  }
  return fCont;
}


StatusCode RawBankToSTLiteClusterAlg::decodeBanks(const RawEvent& rawEvt,
                                                  STLiteCluster::STLiteClusters& fCont) const {
  std::unique_ptr<LHCb::STTELL1BoardErrorBanks> errorBanks = nullptr;

  const auto&  tBanks = rawEvt.banks(bankType());
  std::vector<unsigned int> missing = missingInAction(tBanks);
  if ( !missing.empty() ){
    m_lostBanks += missing.size() ;
    if (tBanks.empty()){
      ++m_noBanksFound;
      return StatusCode::SUCCESS;
    }
  }

  const bool isUT = (detType() == "UT");
  bool errorBanksFailed = false;

  auto validBanksBuf = m_validBanks.buffer();
  auto skippedBanksBuf = m_skippedBanks.buffer();
  auto validSourceIDBuf = m_validSourceID.buffer();
  for (auto& bank : tBanks){
    ++validBanksBuf;
    // get the board and data
    if (bank->magic() != RawBank::MagicPattern) {
      Warning( "wrong magic pattern "+ std::to_string(bank->sourceID()),
               StatusCode::SUCCESS,2).ignore();
      skippedBanksBuf += tBanks.size();
      continue;
    }
    STTell1Board* aBoard = readoutTool()->findByBoardID(STTell1ID(bank->sourceID()));
    if (!aBoard) {
      Warning( "Invalid source ID --> skip bank"+ std::to_string(bank->sourceID()),
               StatusCode::SUCCESS,2).ignore();
      skippedBanksBuf += 1;
      continue;
    }

    // Error handling part
    bool pcnVoted = false;
    bool recover = false;
    STTELL1BoardErrorBank* errorBank = nullptr;
    unsigned int pcn = STDAQ::inValidPcn;
    STDecoder decoder(bank->data());
    if (decoder.hasError() && !m_skipErrors) {
      if (!recoverMode()) {
        Warning( "bank has errors, skip sourceID " + std::to_string(bank->sourceID()),
                 StatusCode::SUCCESS, 2).ignore();
        skippedBanksBuf += 1;
        continue;
      }
      // flag that need to recover....
      ++counter("recovered banks" +  std::to_string(bank->sourceID()));
      recover = true;
      // ok this is a bit ugly.....
      if (!errorBanks.get() && !errorBanksFailed) {
        try {
          errorBanks = decodeErrors(rawEvt);
        } catch (GaudiException &e) {
          errorBanksFailed = true;
          warning() << e.what() << endmsg;
        }
      }
      if (errorBanks.get()) {
        // vote for pcn if needed
        if (!pcnVoted) {
          pcn = pcnVote(tBanks);
          if (pcn == STDAQ::inValidPcn) {
            skippedBanksBuf += tBanks.size();
            return Warning("PCN vote failed", StatusCode::SUCCESS,2);
          }
        }
        errorBank = errorBanks->object(bank->sourceID());
        const unsigned bankpcn = decoder.header().pcn();
        if (pcn != bankpcn && !m_skipErrors){
          debug() << "Expected " << pcn << " found " << bankpcn << endmsg;
          if ( msgLevel(MSG::DEBUG) )
            Warning("PCNs out of sync sourceID "+ std::to_string(bank->sourceID())
                    , StatusCode::SUCCESS,2).ignore();
          skippedBanksBuf += 1;
          continue;
        }
      } // errorbank == 0
    }

    ++validSourceIDBuf;
    const STDAQ::version bankVersion = STDAQ::version(forceVersion() ? m_forcedVersion : bank->version() );

    // check the integrity of the bank --> always skip if not ok
    if (!m_skipErrors && !checkDataIntegrity(decoder, aBoard, bank->size(), bankVersion)) continue;

    // read in the first half of the bank
    for (auto iterDecoder = decoder.posBegin();iterDecoder != decoder.posEnd(); ++iterDecoder){
      if (!recover) {
        createCluster(aBoard,bankVersion ,*iterDecoder, fCont, isUT);
      } else {
        if (errorBank && canBeRecovered(errorBank,*iterDecoder, pcn)){
          createCluster(aBoard, bankVersion, *iterDecoder, fCont, isUT);
        }
      }
    }
  }

  return StatusCode::SUCCESS;
}


StatusCode RawBankToSTLiteClusterAlg::finalize() {

  const double failed = m_skippedBanks.nEntries();
  const double processed = m_validBanks.nEntries();
  double eff = ( !LHCb::Math::Equal_To<double>()(processed, 0.0) ?
                            1.0 - (failed/processed) : 0.0 );
  info() << "Successfully processed " << 100* eff << " %"  << endmsg;

  return Transformer::finalize();
}

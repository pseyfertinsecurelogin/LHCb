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

  const std::vector<RawBank* >&  tBanks = rawEvt.banks(bankType());
  std::vector<unsigned int> missing = missingInAction(tBanks);
  if ( !missing.empty() ){
    //counter("lost Banks") += missing.size() ;
    if (tBanks.empty()){
      //++counter("no banks found");
      return StatusCode::SUCCESS;
    }
  }

  const bool isUT = (detType() == "UT");

  for (auto& bank : tBanks){
    //++counter("# valid banks");
    // get the board and data
    if (bank->magic() != RawBank::MagicPattern) {
      Warning( "wrong magic pattern "+ std::to_string(bank->sourceID()),
               StatusCode::SUCCESS,2).ignore();
      //counter("skipped Banks") += tBanks.size();
      continue;
    }
    STTell1Board* aBoard = readoutTool()->findByBoardID(STTell1ID(bank->sourceID()));
    if (!aBoard) {
      Warning( "Invalid source ID --> skip bank"+ std::to_string(bank->sourceID()),
               StatusCode::SUCCESS,2).ignore();
      //counter("skipped Banks") += tBanks.size();
      continue;
    }
    const STDAQ::version bankVersion = STDAQ::version(forceVersion() ? m_forcedVersion
                                                                     : bank->version() );
    STDecoder decoder(bank->data());
    // read in the first half of the bank
    for (auto iterDecoder = decoder.posBegin();iterDecoder != decoder.posEnd(); ++iterDecoder){
        createCluster(aBoard,bankVersion ,*iterDecoder, fCont, isUT);
    } //decoder
  } // iterBank

  return StatusCode::SUCCESS;
}


StatusCode RawBankToSTLiteClusterAlg::finalize() {

  const double failed = counter("skipped Banks").flag();
  const double processed = counter("# valid banks").flag();
  double eff = ( !LHCb::Math::Equal_To<double>()(processed, 0.0) ?
                            1.0 - (failed/processed) : 0.0 );
  info() << "Successfully processed " << 100* eff << " %"  << endmsg;

  return Transformer::finalize();
}

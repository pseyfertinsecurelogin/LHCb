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
//-----------------------------------------------------------------------------

DECLARE_ALGORITHM_FACTORY( RawBankToSTLiteClusterAlg )

RawBankToSTLiteClusterAlg::RawBankToSTLiteClusterAlg( const std::string& name,
                                           ISvcLocator* pSvcLocator ):
STDecodingBaseAlg (name , pSvcLocator){

 // Standard constructor, initializes variables
  declareSTConfigProperty( "clusterLocation", m_clusterLocation , STLiteClusterLocation::TTClusters);
  declareSTConfigProperty("BankType", m_bankTypeString , detType() );
}


StatusCode RawBankToSTLiteClusterAlg::execute() {

  STLiteCluster::STLiteClusters* fCont = new STLiteCluster::STLiteClusters();
  fCont->reserve(5000);
  put(fCont, m_clusterLocation);

  if (!validSpill()) {
    return Warning("Not a valid spill",StatusCode::SUCCESS, 1);
  }

  // Retrieve the RawEvent:
  LHCb::RawEvent* rawEvt = nullptr;
  for (const auto& loc : m_rawEventLocations ) {
    rawEvt = getIfExists<LHCb::RawEvent>(loc);
    if (rawEvt) break;
  }
  if( !rawEvt ) return Error("Failed to find raw data");

  // decode banks
  StatusCode sc = decodeBanks(rawEvt, fCont);
  return  sc.isFailure() ? Error("Problems in decoding event skipped", sc)
                         : sc ;
}


StatusCode RawBankToSTLiteClusterAlg::decodeBanks(RawEvent* rawEvt,STLiteCluster::STLiteClusters* fCont) const{

  const std::vector<RawBank* >&  tBanks = rawEvt->banks(bankType());
  std::vector<unsigned int> missing = missingInAction(tBanks);
  if ( !missing.empty() ){
    counter("lost Banks") += missing.size() ;
    if (tBanks.empty()){
      ++counter("no banks found");
      return StatusCode::SUCCESS;
    }
  }


  const unsigned int pcn = pcnVote(tBanks);
  if ( msgLevel(MSG::DEBUG) ) debug() << "PCN was voted to be " << pcn << endmsg;
  if (pcn == STDAQ::inValidPcn) {
    counter("skipped Banks") += tBanks.size();
    return Warning("PCN vote failed", StatusCode::SUCCESS,2);
  }

  const bool isUT = (detType() == "UT");

  // loop over the banks of this type..
  for ( auto iterBank =  tBanks.begin(); iterBank != tBanks.end() ; ++iterBank){

    ++counter("# valid banks");

    // get the board and data
    STTell1Board* aBoard = readoutTool()->findByBoardID(STTell1ID((*iterBank)->sourceID()));
    if (!aBoard && !m_skipErrors){
      Warning( "Invalid source ID --> skip bank"+ std::to_string((*iterBank)->sourceID()),
               StatusCode::SUCCESS,2).ignore();
      ++counter("skipped Banks");
      continue;
    }

   ++counter("# valid source ID");

   if ((*iterBank)->magic() != RawBank::MagicPattern) {
      Warning( "wrong magic pattern "+ std::to_string((*iterBank)->sourceID()),
               StatusCode::SUCCESS,2).ignore();
      counter("skipped Banks") += tBanks.size();
      continue;
    }

    // make a SmartBank of shorts...
    STDecoder decoder((*iterBank)->data());

    bool recover = false;
    if (decoder.hasError() && !m_skipErrors){

      if (!recoverMode()){
        Warning( "bank has errors, skip sourceID " + std::to_string((*iterBank)->sourceID()),
                 StatusCode::SUCCESS, 2).ignore();
        ++counter("skipped Banks");
        continue;
      }else{
        // flag that need to recover....
        recover = true;
        ++counter("recovered banks" +  std::to_string((*iterBank)->sourceID()));
      }
    }

    // ok this is a bit ugly.....
    STTELL1BoardErrorBank* errorBank = ( recover ? findErrorBank((*iterBank)->sourceID())
                                                 : nullptr );

    if (!errorBank) {
      const unsigned bankpcn = decoder.header().pcn();
      if (pcn != bankpcn && !m_skipErrors){
        if ( msgLevel(MSG::DEBUG) )
          debug() << "Expected " << pcn << " found " << bankpcn << endmsg;
        Warning( "PCNs out of sync sourceID "+ std::to_string((*iterBank)->sourceID()),
                 StatusCode::SUCCESS,2).ignore();
        ++counter("skipped Banks");
        continue;
      }
    } // errorbank == 0

    const STDAQ::version bankVersion = STDAQ::version(forceVersion() ? m_forcedVersion
                                                                     : (*iterBank)->version() );

    // check the integrity of the bank --> always skip if not ok
    if (!m_skipErrors && !checkDataIntegrity(decoder, aBoard , (*iterBank)->size() , bankVersion)) continue;

    // read in the first half of the bank
    for ( auto iterDecoder = decoder.posBegin();iterDecoder != decoder.posEnd(); ++iterDecoder){


      if (!recover){
        createCluster(aBoard,bankVersion ,*iterDecoder, *fCont, isUT);
      }else{
        if (errorBank && canBeRecovered(errorBank,*iterDecoder, pcn)){
          createCluster(aBoard, bankVersion, *iterDecoder, *fCont, isUT);
        } // errorbanl
      } // recover == false

    } //decoder

  } // iterBank

  // sort and remove any duplicates
  std::stable_sort(fCont->begin(),fCont->end(), Less_by_Channel());
  auto iter =  std::unique(fCont->begin(), fCont->end(), Equal_Channel());
  if (iter != fCont->end()){
    fCont->resize(iter - fCont->begin());
    return Warning("Removed duplicate clusters in the decoding", StatusCode::SUCCESS, 100);
  }
  return StatusCode::SUCCESS;
}


StatusCode RawBankToSTLiteClusterAlg::finalize() {

  const double failed = counter("skipped Banks").flag();
  const double processed = counter("# valid banks").flag();
  double eff = ( !LHCb::Math::Equal_To<double>()(processed, 0.0) ?
                            1.0 - (failed/processed) : 0.0 );
  info() << "Successfully processed " << 100* eff << " %"  << endmsg;

  return STDecodingBaseAlg::finalize();
}

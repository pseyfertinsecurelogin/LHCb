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

//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToSTLiteClusterAlg
//
// 2004-01-07 : Matthew Needham
//-----------------------------------------------------------------------------

DECLARE_ALGORITHM_FACTORY( RawBankToSTLiteClusterAlg )

RawBankToSTLiteClusterAlg::RawBankToSTLiteClusterAlg( const std::string& name,
                                                      ISvcLocator* pSvcLocator )
: STDecodingBaseAlg (name , pSvcLocator)
{
  // Standard constructor, initializes variables
  auto p = declareSTConfigProperty( "clusterLocation", m_clusterLocation , STLiteClusterLocation::TTClusters);
  p->declareUpdateHandler([=](Property&){  this->m_clusterDh.updateKey( this->m_clusterLocation ); } );
  p->useUpdateHandler();
  declareSTConfigProperty("BankType", m_bankTypeString , detType() );
}

StatusCode RawBankToSTLiteClusterAlg::initialize() {

  // Initialization
  StatusCode sc = STDecodingBaseAlg::initialize();
  if (sc.isFailure()) return Error("Failed to initialize", sc);
  
  // FIXME: STDecodingBaseAlg::initialize uses DeclareSTConfigProperty declared properties
  // to 'flip' their detector types by directly writing to eg. m_clusterLocation
  // the callback doesn't get invoked, so m_clusterDh gets out of sync. So we need to explicilty fix
  // the key at the end of initialize...
  m_clusterDh.updateKey(m_clusterLocation);
  return StatusCode::SUCCESS;
}


StatusCode RawBankToSTLiteClusterAlg::execute() {

  if (!validSpill()) return Warning("Not a valid spill",StatusCode::SUCCESS, 1);

  // Retrieve the RawEvent:
  LHCb::RawEvent* rawEvt = nullptr;
  for (const auto& p : m_rawEventLocations) {
    rawEvt = getIfExists<LHCb::RawEvent>(p);
    if (rawEvt) break;
  }
  if( !rawEvt ) return Error("Failed to find raw data");

  // decode banks
  STLiteCluster::STLiteClusters fCont;
  fCont.reserve(5000);
  StatusCode sc = decodeBanks(rawEvt, fCont);
  if (sc.isFailure()){
    return Error("Problems in decoding event skipped", sc);
  }

  // put clusters into Event Store
  m_clusterDh.put(std::move(fCont));

  return sc;
}


StatusCode RawBankToSTLiteClusterAlg::decodeBanks(RawEvent* rawEvt,STLiteCluster::STLiteClusters& fCont) const{

  const std::vector<RawBank* >&  tBanks = rawEvt->banks(bankType());
  std::vector<unsigned int> missing = missingInAction(tBanks);
  if ( !missing.empty() ){
    counter("lost Banks") += missing.size() ;
    if (tBanks.size() == 0){
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

  for (auto iterBank =  tBanks.begin(); iterBank != tBanks.end() ; ++iterBank){

    ++counter("# valid banks");

    // get the board and data
    STTell1Board* aBoard = readoutTool()->findByBoardID(STTell1ID((*iterBank)->sourceID()));
    if (!aBoard && !m_skipErrors){
      Warning( "Invalid source ID --> skip bank"+ std::to_string((*iterBank)->sourceID()),
              StatusCode::SUCCESS,2);
      ++counter("skipped Banks");
      continue;
    }

   ++counter("# valid source ID");

   if ((*iterBank)->magic() != RawBank::MagicPattern) {
      Warning( "wrong magic pattern "+ std::to_string((*iterBank)->sourceID()),
               StatusCode::SUCCESS,2);
      counter("skipped Banks") += tBanks.size();
      continue;
    }

    // make a SmartBank of shorts...
    STDecoder decoder((*iterBank)->data());

    bool recover = false;
    if (decoder.hasError() == true && !m_skipErrors){

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
    STTELL1BoardErrorBank* errorBank = recover ? findErrorBank((*iterBank)->sourceID()) : nullptr;

    if (errorBank) {
      const unsigned bankpcn = decoder.header().pcn();
      if (pcn != bankpcn && !m_skipErrors){
          debug() << "Expected " << pcn << " found " << bankpcn << endmsg;
        if ( msgLevel(MSG::DEBUG) )
        Warning("PCNs out of sync sourceID "+ std::to_string((*iterBank)->sourceID())
                , StatusCode::SUCCESS,2).ignore();
        ++counter("skipped Banks");
        continue;
      }
    } // errorbank == 0

    const STDAQ::version bankVersion = forceVersion() ? STDAQ::version(m_forcedVersion): STDAQ::version((*iterBank)->version());

    // check the integrity of the bank --> always skip if not ok
    if (!m_skipErrors && checkDataIntegrity(decoder, aBoard , (*iterBank)->size() , bankVersion) == false) continue;




    // read in the first half of the bank
    
    for (auto iterDecoder = decoder.posBegin();iterDecoder != decoder.posEnd(); ++iterDecoder){

      if (!recover){
        createCluster(aBoard,bankVersion ,*iterDecoder, &fCont, isUT);
      }else{
        if (errorBank && canBeRecovered(errorBank,*iterDecoder, pcn)){
          createCluster(aBoard, bankVersion, *iterDecoder, &fCont, isUT);
        } // errorbank
      } // recover == false

    } //decoder

  } // iterBank

  // sort and remove any duplicates
  std::stable_sort(fCont.begin(),fCont.end(), Less_by_Channel());
  STLiteCluster::STLiteClusters::iterator iter =  std::unique(fCont.begin(), fCont.end(), Equal_Channel());
  if (iter != fCont.end()){
    const unsigned int nvalid = iter - fCont.begin();
    fCont.resize(nvalid);
    return Warning("Removed duplicate clusters in the decoding", StatusCode::SUCCESS, 100);
  }

  return StatusCode::SUCCESS;

}


StatusCode RawBankToSTLiteClusterAlg::finalize() {

  const double failed = counter("skipped Banks").flag();
  const double processed = counter("# valid banks").flag();
  double eff = 0.0;
  if (!LHCb::Math::Equal_To<double>()(processed, 0.0)){
    eff = 1.0 - (failed/processed);
  }
  info() << "Successfully processed " << 100* eff << " %"  << endmsg;

  return STDecodingBaseAlg::finalize();
}


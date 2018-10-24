/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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
#include "UTDAQ/RawBankToUTLiteClusterAlg.h"
#include "Kernel/IUTReadoutTool.h"

#include "LHCbMath/LHCbMath.h"

// Event
#include "Event/RawEvent.h"
#include "Event/UTLiteCluster.h"


#include "Kernel/UTTell1Board.h"
#include "Kernel/UTTell1ID.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/UTDecoder.h"
#include "Kernel/UTDataFunctor.h"
#include "Kernel/UTStripRepresentation.h"

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
    inline bool operator() ( LHCb::UTLiteCluster obj1 , LHCb::UTLiteCluster obj2 ) const
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
    inline bool operator() ( LHCb::UTLiteCluster obj1 , LHCb::UTLiteCluster obj2 ) const
    {
      return obj1.channelID() == obj2.channelID() ;
    }
  };

}

//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToUTLiteClusterAlg
//
// 2004-01-07 : Matthew Needham
// 2016-10-07 : Sebastien Ponce
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( RawBankToUTLiteClusterAlg )

RawBankToUTLiteClusterAlg::RawBankToUTLiteClusterAlg(const std::string& name,
                                                     ISvcLocator* pSvcLocator) :
Transformer(name, pSvcLocator,
            {KeyValue{"OdinLocation", LHCb::ODINLocation::Default},
             KeyValue{"RawEventLocations",
                      Gaudi::Functional::concat_alternatives(LHCb::RawEventLocation::Tracker,
                                                             LHCb::RawEventLocation::Other,
                                                             LHCb::RawEventLocation::Default)}},
            KeyValue("clusterLocation", UTLiteClusterLocation::UTClusters)) {
  // Standard constructor, initializes variables
  declareUTConfigProperty("BankType", m_bankTypeString , detType() );
}

StatusCode RawBankToUTLiteClusterAlg::initialize() {
  // Initialization
  StatusCode sc = Transformer::initialize();
  if (sc.isFailure()) return Error("Failed to initialize", sc);
  // Spill
  computeSpillOffset(inputLocation<1>());
  // return
  return StatusCode::SUCCESS;
}

LHCb::UTLiteCluster::UTLiteClusters RawBankToUTLiteClusterAlg::operator()(const LHCb::ODIN& odin, const LHCb::RawEvent& rawEvt) const {
  UTLiteCluster::UTLiteClusters fCont;
  if (!validSpill(odin)) {
    warning() << "Not a valid spill" << endmsg;
  } else {
    fCont.reserve(5000);
    // decode banks
    StatusCode sc = decodeBanks(rawEvt, fCont);
    if (sc.isFailure()){
      throw GaudiException("Problems in decoding event skipped", "RawBankToUTLiteClusterAlg", StatusCode::FAILURE );
    }
  }
  return fCont;
}


StatusCode RawBankToUTLiteClusterAlg::decodeBanks(const RawEvent& rawEvt,
                                                  UTLiteCluster::UTLiteClusters& fCont) const {
  std::unique_ptr<LHCb::UTTELL1BoardErrorBanks> errorBanks = nullptr;
  bool errorBanksFailed = false;

  const LHCb::span<const RawBank* >  tBanks = rawEvt.banks(bankType());
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
  if (pcn == UTDAQ::inValidPcn) {
    counter("skipped Banks") += tBanks.size();
    return Warning("PCN vote failed", StatusCode::SUCCESS,2);
  }

  const bool isUT = (detType() == "UT");

  // loop over the banks of this type..

  for (auto iterBank =  tBanks.begin(); iterBank != tBanks.end() ; ++iterBank){

    ++counter("# valid banks");

    // get the board and data
    UTTell1Board* aBoard = readoutTool()->findByBoardID(UTTell1ID((*iterBank)->sourceID()));
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
    UTDecoder decoder((*iterBank)->data());

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
    UTTELL1BoardErrorBank* errorBank = nullptr;
    if (recover) {
      if (!errorBanks.get() && !errorBanksFailed) {
        try {
          errorBanks = decodeErrors(rawEvt);
        } catch (GaudiException &e) {
          errorBanksFailed = true;
          warning() << e.what() << endmsg;
        }
      }
      if (errorBanks.get()) {
        errorBank = errorBanks->object((*iterBank)->sourceID());
      }
    }

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

    const UTDAQ::version bankVersion = UTDAQ::version(forceVersion() ? m_forcedVersion.value()
                                                                     : (*iterBank)->version() );

    // check the integrity of the bank --> always skip if not ok
    if (!m_skipErrors && !checkDataIntegrity(decoder, aBoard , (*iterBank)->size() , bankVersion)) continue;

    // read in the first half of the bank
    for (auto iterDecoder = decoder.posBegin();iterDecoder != decoder.posEnd(); ++iterDecoder){

      if (!recover){
        createCluster(aBoard,bankVersion ,*iterDecoder, fCont, isUT);
      }else{
        if (errorBank && canBeRecovered(errorBank,*iterDecoder, pcn)){
          createCluster(aBoard, bankVersion, *iterDecoder, fCont, isUT);
        } // errorbank
      } // recover == false

    } //decoder

  } // iterBank

  // sort and remove any duplicates
  std::stable_sort(fCont.begin(),fCont.end(), Less_by_Channel());
  auto iter =  std::unique(fCont.begin(), fCont.end(), Equal_Channel());
  if (iter != fCont.end()){
    fCont.resize(iter - fCont.begin());
    return Warning("Removed duplicate clusters in the decoding", StatusCode::SUCCESS, 100);
  }
  return StatusCode::SUCCESS;
}


StatusCode RawBankToUTLiteClusterAlg::finalize() {

  const double failed = counter("skipped Banks").flag();
  const double processed = counter("# valid banks").flag();
  double eff = ( !LHCb::Math::Equal_To<double>()(processed, 0.0) ?
                            1.0 - (failed/processed) : 0.0 );
  info() << "Successfully processed " << 100* eff << " %"  << endmsg;

  return Transformer::finalize();
}

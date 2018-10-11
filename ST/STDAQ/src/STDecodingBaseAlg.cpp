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

// local
#include "STDecodingBaseAlg.h"

// Event
#include "Event/RawEvent.h"
#include "Event/ByteStream.h"
#include "Event/STCluster.h"
#include "Event/ODIN.h"

#include "Kernel/STDataFunctor.h"
#include "Kernel/ISTReadoutTool.h"
#include "Kernel/STTell1Board.h"
#include "Kernel/STTell1ID.h"
#include "Kernel/STRawBankMap.h"
#include "Kernel/STDecoder.h"
#include "Kernel/STLexicalCaster.h"


#include "SiDAQ/SiHeaderWord.h"
#include "SiDAQ/SiADCWord.h"
#include "boost/lexical_cast.hpp"


#include "STDet/DeSTDetector.h"


using namespace LHCb;


//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToSTClusterAlg
//
// 2004-01-07 : Matthew Needham
// 2016-10-07 : Sebastien Ponce
//-----------------------------------------------------------------------------


STDecodingBaseAlg::STDecodingBaseAlg( const std::string& name,
                                           ISvcLocator* pSvcLocator )
    : ST::AlgBase (name , pSvcLocator)
{
 // Standard constructor, initializes variables
 declareSTConfigProperty("ErrorBank", m_errorBankString , "TTError");

 declareProperty("skipBanksWithErrors", m_skipErrors = false );
 declareProperty("recoverMode", m_recoverMode = true);

 declareProperty("forcedVersion", m_forcedVersion = STDAQ::inValidVersion);
 declareProperty("checkValidity", m_checkValidSpill = true);

 setForcedInit();
}

StatusCode STDecodingBaseAlg::initialize() {

  // Initialization
  StatusCode sc = ST::AlgBase::initialize();
  if (sc.isFailure()) return sc;
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> initialize " << endmsg;

  // bank type
  if (!m_bankTypeString.empty()){
    m_bankType =  STRawBankMap::stringToType(m_bankTypeString);
    if (m_bankType ==  LHCb::RawBank::Velo){
      fatal() << "Wrong detector type: only IT or TT !"<< endmsg;
      return StatusCode::FAILURE;
    }
  }

  // bank type
  m_errorType =  STRawBankMap::stringToType(m_errorBankString);
  if (m_errorType ==  LHCb::RawBank::Velo){
    fatal() << "Wrong detector type: only IT or TT error banks!"<< endmsg;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}

unsigned int STDecodingBaseAlg::pcnVote(LHCb::span<const RawBank* > banks) const{

  // make a majority vote to get the correct PCN in the event
  std::map<unsigned int, unsigned int> pcns;
  for (const auto& bank : banks) {
    STDecoder decoder(bank->data());
    // only the good are allowed to vote [the US system..]
    if (!decoder.header().hasError()) ++pcns[decoder.header().pcn()];
   } // banks

  auto max = std::max_element( pcns.begin(), pcns.end(),
                               [](const std::pair<unsigned int, unsigned int>& lhs,
                                  const std::pair<unsigned int, unsigned int>& rhs)
                               { return lhs.second < rhs.second; } );
  return max == pcns.end() ? STDAQ::inValidPcn : max->first;
}

bool STDecodingBaseAlg::checkDataIntegrity(STDecoder& decoder,
                                           const STTell1Board* aBoard,
                                           const unsigned int bankSize,
                                           const STDAQ::version& bankVersion) const
{
  // check the consistancy of the data

  bool ok = true;
  auto iterDecoder = decoder.posAdcBegin();
  for (;iterDecoder != decoder.posAdcEnd(); ++iterDecoder){

    const STClusterWord aWord = iterDecoder->first;

    // make some consistancy checks
    if ((iterDecoder->second.size() - 1u  < aWord.pseudoSize())) {
      unsigned int fracStrip = aWord.fracStripBits();
      if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
        debug() << "adc values do not match ! " << iterDecoder->second.size()-1u << " "
                << aWord.pseudoSize() << " offline chan "
                << aBoard->DAQToOffline(fracStrip,bankVersion,STDAQ::StripRepresentation(aWord.channelID()))
                << " source ID  " << aBoard->boardID()  <<  " chan "  << aWord.channelID()
                << endmsg ;
      Warning("ADC values do not match", StatusCode::SUCCESS,2).ignore();
      ok = false;
      break;
    }

    // decode the channel
    if (!aBoard->validChannel(aWord.channelID())){
      if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
        debug() << "invalid TELL1 channel number board: " << aBoard->boardID() << " chan " << aWord.channelID() << endmsg;
      Warning("Invalid tell1 channel", StatusCode::SUCCESS,2).ignore();
      ok = false;
      break;
    }

  } // loop clusters

  // final check that we read the total number of bytes in the bank
  if (ok && (unsigned int)iterDecoder.bytesRead() != bankSize){
    ok = false;
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "Inconsistant byte count " << aBoard->boardID() << " Read: "  << iterDecoder.bytesRead()
              << " Expected: " << bankSize << endmsg;
    Warning("Inconsistant byte count", StatusCode::SUCCESS).ignore();
  }

  if (!ok) ++m_skippedBanks;

  return ok;
}

std::vector<unsigned int> STDecodingBaseAlg::missingInAction(LHCb::span<const RawBank* > banks) const{

  std::vector<unsigned int> missing;
  if ( banks.size() != readoutTool()->nBoard()) {
    for (unsigned int iBoard = 0u; iBoard < readoutTool()->nBoard() ; ++iBoard ){
      int testID = readoutTool()->findByOrder(iBoard)->boardID().id();
      auto iterBank = std::find_if(banks.begin(),banks.end(),
                                   [&](const RawBank* b) { return b->sourceID() == testID; } );
      if (iterBank == banks.end()){
        missing.push_back((unsigned int)testID);
        std::string lostBank = "lost bank "+ boost::lexical_cast<std::string>(testID);
        Warning(lostBank, StatusCode::SUCCESS,0).ignore();
      }
    } // iBoard
  }
  return missing;
}

std::unique_ptr<LHCb::STTELL1BoardErrorBanks> STDecodingBaseAlg::decodeErrors(const LHCb::RawEvent& raw) const {

 if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute " << endmsg;

 // make an empty output vector
 auto outputErrors = std::make_unique<STTELL1BoardErrorBanks>();

 // Pick up ITError bank
 const auto& itf = raw.banks(LHCb::RawBank::BankType(m_errorType));

 if (itf.empty()){
   if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
     debug() <<"event has no error banks " << endmsg;
 } else {
    ++m_evtsWithErrorBanks;
    m_totalErrorBanks += itf.size();
 }

 if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
   debug() << "Starting to decode " << itf.size() << detType() <<"Error bank(s)" <<  endmsg;

 for( const auto& bank : itf ) {

   std::string errorBank = "sourceID "+
	boost::lexical_cast<std::string>(bank->sourceID());
   ++counter(errorBank);

   if (bank->magic() != RawBank::MagicPattern) {
     std::string pattern = "wrong magic pattern "+
	boost::lexical_cast<std::string>(bank->sourceID());
     Warning(pattern, StatusCode::SUCCESS,2).ignore();
     continue;
   }

   const unsigned int* p = bank->data();
   unsigned int w=0;
   const unsigned int bankEnd = bank->size()/sizeof(unsigned int);

   // bank has to be at least 28 words
   if (bankEnd < STDAQ::minErrorBankWords){
     warning() << "Error bank length is " << bankEnd << " and should be at least " << STDAQ::minErrorBankWords << endmsg;
     Warning("Error bank too short --> not decoded for TELL1 " + ST::toString(bank->sourceID()), StatusCode::SUCCESS,2).ignore();
     continue;
   }

   // and less than 56 words
   if (bankEnd > STDAQ::maxErrorBankWords){
     warning() << "Error bank length is " << bankEnd << " and should be at most " << STDAQ::maxErrorBankWords << endmsg;
     Warning("Error bank too long --> not decoded for TELL1 " + ST::toString(bank->sourceID()), StatusCode::SUCCESS,2).ignore();
     continue;
   }

    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "Decoding bank number of type "<< detType() << "Error (TELL1 ID: " <<  bank->sourceID()
              << ", Size: " <<  bank->size() << " bytes)" << endmsg;


   // make an empty tell1 data object
   STTELL1BoardErrorBank* myData = new STTELL1BoardErrorBank();
   outputErrors->insert(myData, bank->sourceID());

   for (unsigned int ipp = 0; ipp < STDAQ::npp && w != bankEnd ; ++ipp ){

     if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
       debug() << "#######  Parsing now data from PP " << ipp << " #####################"<< endmsg;

     // we must find 5 words
     if (bankEnd - w < 5 ){
       Warning("Ran out of words to read", StatusCode::SUCCESS,2).ignore();
       break;
     }

     STTELL1Error* errorInfo = new STTELL1Error(p[w], p[w+1], p[w+2], p[w+3], p[w+4]);
     myData->addToErrorInfo(errorInfo);
     w +=5; // read 5 first words

     const unsigned int  nOptional = errorInfo->nOptionalWords();

     // we must find the optional words + 2 more control words
     if (bankEnd - w < nOptional + 2 ){
       Warning("Ran out of words to read", StatusCode::SUCCESS,2).ignore();
        break;
      }

     const unsigned int *eInfo = nullptr;

     if (errorInfo->hasErrorInfo()){
       //errorInfo->setOptionalErrorWords(p[w], p[w+1], p[w+2], p[w+3], p[w+4]);
       eInfo = &p[w];
       w+= 5;
     } // has error information

     errorInfo->setWord10(p[w]); ++w;
     errorInfo->setWord11(p[w]); ++w;

     // then some more optional stuff
     if (errorInfo->hasNZS()){
       errorInfo->setWord12(p[w]); ++w;
     }  // nsz info...

     // then some more optional stuff
     if (errorInfo->hasPed()){
       errorInfo->setWord13(p[w]); ++w;
     }

     if (errorInfo->hasErrorInfo()){
       errorInfo->setOptionalErrorWords(eInfo[0], eInfo[1], eInfo[2], eInfo[3], eInfo[4]);
     } // has error information

   } //  loop ip [ppx's]

   if (w != bankEnd){
     error() << "read " << w << " words, expected: " << bankEnd << endmsg;
   }

  }// end of loop over banks of a certain type

  return outputErrors;
}

std::string STDecodingBaseAlg::toSpill(const std::string& location) const{

  std::string theSpill;
  for (const auto* name : {"Prev", "Next"} ) {
    auto iPos = location.find(name);
    if (iPos != std::string::npos){
      std::string startSpill = location.substr(iPos);
      std::string::size_type iPos2 = startSpill.find("/");
      theSpill = startSpill.substr(0,iPos2);
      break;
    }
  }  // is
  return theSpill;
}

void STDecodingBaseAlg::computeSpillOffset(const std::string& location) {
  // convert spill to offset in time
  std::string spill = toSpill(location);
  m_spillOffset = (spill.size() > 4u ? LHCb::STCluster::SpillToType(spill) : LHCb::STCluster::Spill::Central);
}

bool STDecodingBaseAlg::validSpill(const LHCb::ODIN& odin) const{
  if (!m_checkValidSpill) return true;

  // check spill is actually read out using the ODIN
  const unsigned int numberOfSpills = odin.timeAlignmentEventWindow();
  return (unsigned int)abs(m_spillOffset) <= numberOfSpills ;
}


unsigned int STDecodingBaseAlg::byteSize(LHCb::span<const RawBank*> banks) const {
  return std::accumulate(banks.begin(), banks.end(), 0u,
                         [](unsigned int s, const RawBank* b) {
                    return s + b->totalSize();
  });
}

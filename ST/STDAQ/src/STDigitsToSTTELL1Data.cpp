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
#include "STDigitsToSTTELL1Data.h"

// Event
#include "Event/STTELL1Data.h"
#include "Event/STDigit.h"


#include "Kernel/LHCbConstants.h"
#include "Kernel/STTell1Board.h"
#include "Kernel/ISTReadoutTool.h"
#include "Kernel/STDAQDefinitions.h"


#include "STDet/DeSTDetector.h"


using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToSTClusterAlg
//
// 2004-01-07 : Matthew Needham
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( STDigitsToSTTELL1Data )

STDigitsToSTTELL1Data::STDigitsToSTTELL1Data( const std::string& name,
                                           ISvcLocator* pSvcLocator ):
ST::AlgBase (name , pSvcLocator)
{

 // Standard constructor, initializes variables
 declareSTConfigProperty( "outputLocation", m_outputLocation , STTELL1DataLocation::TTSubPeds);
 declareSTConfigProperty( "inputLocation", m_inputLocation , STDigitLocation::TTDigits);

}

StatusCode STDigitsToSTTELL1Data::execute() {

  // Retrieve the digits
  const STDigits* digitCont = get<STDigits>(m_inputLocation);

  // make a new digits container
  auto outCont = std::make_unique<STTELL1Datas>();

  StatusCode sc = createTell1Data(digitCont,outCont.get());

  if (sc.isFailure()) {
    return Warning("Problems creating Tell1 data", StatusCode::FAILURE, 1);
  }

  put(outCont.release(), m_outputLocation);
  return StatusCode::SUCCESS;
}


StatusCode STDigitsToSTTELL1Data::createTell1Data(const STDigits* digits, STTELL1Datas* outCont) const {

  if (digits->size() != tracker()->nStrip()) {
    return Warning("Digit cont size does not equal number of detector strips",StatusCode::SUCCESS, 1);
  }

  // make correct number of output containers
  for (unsigned int i = 0; i < readoutTool()->nBoard(); ++i){
    STTell1Board* board = readoutTool()->findByOrder(i);
    STTELL1Data::Data dataVec;
    dataVec.resize(STDAQ::noptlinks);
    for(auto& dv : dataVec) dv.resize(LHCbConstants::nStripsInBeetle);
    STTELL1Data* tell1Data = new STTELL1Data(dataVec);
    int key = (int)board->boardID().id();
    outCont->insert(tell1Data,key);
  }  //nBoard

  // then its just one big loop
  for (const auto& digit : *digits) {
     STDAQ::chanPair aPair = readoutTool()->offlineChanToDAQ(digit->channelID(),0.0);
     STTELL1Data* adcBank = outCont->object(aPair.first.id());
     STTELL1Data::Data& dataVec = adcBank->data();
     const unsigned int beetle = aPair.second/LHCbConstants::nStripsInBeetle;
     const unsigned int strip = aPair.second%LHCbConstants::nStripsInBeetle;
     dataVec[beetle][strip] = int(digit->depositedCharge());
  }

  return StatusCode::SUCCESS;
}

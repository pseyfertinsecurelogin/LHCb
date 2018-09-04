#include <algorithm>

// local
#include "UTDAQ/UTDigitsToUTTELL1Data.h"

// Event
#include "Event/UTTELL1Data.h"
#include "Event/UTDigit.h"


#include "Kernel/LHCbConstants.h"
#include "Kernel/UTTell1Board.h"
#include "Kernel/IUTReadoutTool.h"
#include "Kernel/UTDAQDefinitions.h"


#include "UTDet/DeUTDetector.h"


using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToUTClusterAlg
//
// 2004-01-07 : Matthew Needham
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( UTDigitsToUTTELL1Data )

UTDigitsToUTTELL1Data::UTDigitsToUTTELL1Data( const std::string& name,
                                           ISvcLocator* pSvcLocator ):
UT::AlgBase (name , pSvcLocator)
{

 // Standard constructor, initializes variables
 declareUTConfigProperty( "outputLocation", m_outputLocation , UTTELL1DataLocation::UTSubPeds);
 declareUTConfigProperty( "inputLocation", m_inputLocation , UTDigitLocation::UTDigits);

}

StatusCode UTDigitsToUTTELL1Data::execute() {

  // Retrieve the digits
  const UTDigits* digitCont = get<UTDigits>(m_inputLocation);

  // make a new digits container
  auto outCont = std::make_unique<UTTELL1Datas>();

  StatusCode sc = createTell1Data(digitCont,outCont.get());

  if (sc.isFailure()) {
    return Warning("Problems creating Tell1 data", StatusCode::FAILURE, 1);
  }

  put(outCont.release(), m_outputLocation);
  return StatusCode::SUCCESS;
}


StatusCode UTDigitsToUTTELL1Data::createTell1Data(const UTDigits* digits, UTTELL1Datas* outCont) const {

  if (digits->size() != tracker()->nStrip()) {
    return Warning("Digit cont size does not equal number of detector strips",StatusCode::SUCCESS, 1);
  }

  // make correct number of output containers
  for (unsigned int i = 0; i < readoutTool()->nBoard(); ++i){
    UTTell1Board* board = readoutTool()->findByOrder(i);
    UTTELL1Data::Data dataVec;
    dataVec.resize(UTDAQ::noptlinks);
    for(auto& dv : dataVec) dv.resize(LHCbConstants::nStripsInBeetle);
    UTTELL1Data* tell1Data = new UTTELL1Data(dataVec);
    int key = (int)board->boardID().id();
    outCont->insert(tell1Data,key);
  }  //nBoard

  // then its just one big loop
  for (const auto& digit : *digits) {
     UTDAQ::chanPair aPair = readoutTool()->offlineChanToDAQ(digit->channelID(),0.0);
     UTTELL1Data* adcBank = outCont->object(aPair.first.id());
     UTTELL1Data::Data& dataVec = adcBank->data();
     const unsigned int beetle = aPair.second/LHCbConstants::nStripsInBeetle;
     const unsigned int strip = aPair.second%LHCbConstants::nStripsInBeetle;
     dataVec[beetle][strip] = int(digit->depositedCharge());
  }

  return StatusCode::SUCCESS;
}

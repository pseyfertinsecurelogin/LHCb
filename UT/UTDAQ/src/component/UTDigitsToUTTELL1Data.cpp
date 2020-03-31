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
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "Event/UTDigit.h"
#include "Event/UTTELL1Data.h"
#include "GaudiAlg/Transformer.h"
#include "Kernel/IUTReadoutTool.h"
#include "Kernel/LHCbConstants.h"
#include "Kernel/UTAlgBase.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/UTTell1Board.h"
#include "UTDet/DeUTDetector.h"
#include <algorithm>
#include <string>
#include <vector>

/** @class RawBankToUTProcFull RawBankToUTProcFull.h
 *
 *  Algorithm to create UTTELL1Data (type ProcFull) from RawEvent object
 *
 *  @author A. Beiter (based on code by M. Needham)
 *  @date   2018-09-04
 */

class UTDigitsToUTTELL1Data
    : public Gaudi::Functional::Transformer<LHCb::UTTELL1Datas( const LHCb::UTDigits& ),
                                            Gaudi::Functional::Traits::BaseClass_t<UT::AlgBase>> {

public:
  /// Standard constructor
  UTDigitsToUTTELL1Data( const std::string& name, ISvcLocator* pSvcLocator );

  LHCb::UTTELL1Datas operator()( const LHCb::UTDigits& ) const override; ///< Algorithm execution

private:
  StatusCode createTell1Data( const LHCb::UTDigits* digits, LHCb::UTTELL1Datas* outCont ) const;

  std::string m_inputLocation;
  std::string m_outputLocation;
};

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : RawBufferToUTClusterAlg
//
// 2004-01-07 : Matthew Needham
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( UTDigitsToUTTELL1Data )

UTDigitsToUTTELL1Data::UTDigitsToUTTELL1Data( const std::string& name, ISvcLocator* pSvcLocator )
    : Transformer{name,
                  pSvcLocator,

                  {"inputLocation", UTDigitLocation::UTDigits},

                  {"outputLocation", UTTELL1DataLocation::UTSubPeds}} {}

LHCb::UTTELL1Datas UTDigitsToUTTELL1Data::operator()( const LHCb::UTDigits& digitCont ) const {

  // make a new digits container
  UTTELL1Datas outCont;
  createTell1Data( &digitCont, &outCont ).orThrow( "Problems creating Tell1 data", "UTDigitsToUTTELL1Data" ).ignore();
  return outCont;
}

StatusCode UTDigitsToUTTELL1Data::createTell1Data( const UTDigits* digits, UTTELL1Datas* outCont ) const {

  if ( digits->size() != tracker()->nStrip() ) {
    return Warning( "Digit cont size does not equal number of detector strips", StatusCode::SUCCESS, 1 );
  }

  // make correct number of output containers
  for ( unsigned int i = 0; i < readoutTool()->nBoard(); ++i ) {
    UTTell1Board*     board = readoutTool()->findByOrder( i );
    UTTELL1Data::Data dataVec;
    dataVec.resize( UTDAQ::noptlinks );
    for ( auto& dv : dataVec ) dv.resize( LHCbConstants::nStripsInBeetle );
    UTTELL1Data* tell1Data = new UTTELL1Data( dataVec );
    int          key       = (int)board->boardID().id();
    outCont->insert( tell1Data, key );
  } // nBoard

  // then its just one big loop
  for ( const auto& digit : *digits ) {
    UTDAQ::chanPair    aPair   = readoutTool()->offlineChanToDAQ( digit->channelID(), 0.0 );
    UTTELL1Data*       adcBank = outCont->object( aPair.first.id() );
    UTTELL1Data::Data& dataVec = adcBank->data();
    const unsigned int beetle  = aPair.second / LHCbConstants::nStripsInBeetle;
    const unsigned int strip   = aPair.second % LHCbConstants::nStripsInBeetle;
    dataVec[beetle][strip]     = int( digit->depositedCharge() );
  }

  return StatusCode::SUCCESS;
}

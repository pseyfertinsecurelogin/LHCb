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
#pragma once
// Include files
// from Gaudi
#include "Event/RawEvent.h"
#include "GaudiAlg/GaudiTupleAlg.h"

// STL
#include <map>
#include <utility>
#include <vector>

// data model
#include "Kernel/VeloChannelID.h"
#include "Tell1Kernel/VeloDecodeConf.h"
#include "Tell1Kernel/VeloDecodeCore.h"
#include "Tell1Kernel/VeloTell1Core.h"
#include "VeloEvent/VeloFullBank.h"

#include "Event/RawBank.h"

/** @class PrepareVeloFullRawBuffer PrepareVeloFullRawBuffer.h
 *
 *
 *  @author Tomasz Szumlak
 *  @date   2006-03-22
 *
 *  Algorithm dedicated to decoding the new 1MHz non-zero suppresed
 *  RawBank which is sent out by TELL1 pre-processing board.
 *  The input for the algorithm is binary file and the output
 *  is container with VeloDigits objects.
 *  The container is the input for the TELL1
 *  algorithms (pedestal subtraction and following, FIR filter nad
 *  linear CM subtraction)
 */

namespace LHCb {
  class RawEvent;
}

using namespace VeloTELL1;
using namespace LHCb;

class PrepareVeloFullRawBuffer : public GaudiTupleAlg {
public:
  //
  /// Standard constructor
  using GaudiTupleAlg::GaudiTupleAlg;
  StatusCode execute() override; ///< Algorithm execution

private:
  typedef std::map<unsigned int, std::pair<unsigned int, const unsigned int*>> DATA_REPO;

  enum parameters {
    ERROR_HEADER_1       = 1,
    ERROR_HEADER_2       = 2,
    ERROR_SECOND_SECTION = 3,
    ADCHeadersShift      = 4,
    ERROR_FIRST_SECTION  = 5,
    ERROR_EVT_INFO       = 8,
    ODIN_DATA_WORDS      = 9,
    ERROR_THIRD_SECTION  = 10,
    ALinkShift           = 36
  };
  enum bitMasks { ADCMask = 0x000003ff };

  enum bitShifts { ADCShift = 10 };

  enum BANKS_SIZES { WORD2BYTE = 4, EB_FPGAx1 = 6, COMPLETE_EB = 52, FPGAx1 = 224, FPGAx4 = 896 };

  // fetch the RawEvent
  StatusCode getRawEvent();
  // extract vector of the RawBanks of the type VeloFull (non-zero supp.)
  StatusCode getRawBanks();
  void       createOrderedSections();
  // write-out the Gaudi model of the RawBank (with digit objects,
  // header infos and Beetle headers)
  StatusCode writeVeloFull();
  // run after decoding each event
  void resetMemory();
  // use only as emergency checker!
  void dumpADCs( const dataVec& inADCs );

  void setADCBankFlag();
  void setPedBankFlag();
  void unsetADCBankFlag();
  void unsetPedBankFlag();
  bool adcBankFlag();
  bool pedBankFlag();
  void setDecodeWithErrorBank();
  bool getDecodeWithErrorBank() const;

  const LHCb::RawEvent*                m_rawEvent = nullptr; ///< pointer to RawEvent container
  DataObjectReadHandle<LHCb::RawEvent> m_rawEventLoc{
      this, "RawEventLocation", LHCb::RawEventLocation::Default};      ///< location of RawEvent container
  DATA_REPO                            m_fullData2Decode;              ///< source id and ptr to the bank body - full
  DATA_REPO                            m_partialData2Decode;           ///< source id and ptr to the bank body - partial
  DATA_REPO                            m_pedestalData2Decode;          ///< src id and ptr to the bank body - pedestals
  DATA_REPO                            m_errorBanks2Check;             ///< the same for the error banks
  VeloFullBanks*                       m_veloADCData        = nullptr; ///< container to store ordered data
  VeloFullBanks*                       m_veloADCPartialData = nullptr;
  VeloFullBanks*                       m_veloPedestals      = nullptr; ///< container to store ordered pedestals
  DataObjectWriteHandle<VeloFullBanks> m_veloADCDataLoc{
      this, "ADCLocation", VeloFullBankLocation::Default}; /// TES locations of decoded data
  DataObjectWriteHandle<VeloFullBanks> m_veloPedestalsLoc{this, "PedestalsLoc", VeloFullBankLocation::Pedestals};
  DataObjectWriteHandle<VeloFullBanks> m_veloADCPartialDataLoc{this, "ADCPartialLoc", "Raw/Velo/PreparedPartialADC"};
  bool                                 m_adcBankPresent = false; /// flag to check if data is sent out
  bool                                 m_pedBankPresent = false; /// flag to check if ped is sent out
  Gaudi::Property<bool>                m_runWithODIN{this, "RunWithODIN", true};
  Gaudi::Property<bool>                m_roundRobin{this, "RoundRobin", false};
  Gaudi::Property<bool>                m_ignoreErrorBanks{this, "IgnoreErrorBanks", true};
};

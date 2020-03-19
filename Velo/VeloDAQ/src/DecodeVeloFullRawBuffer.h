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
#ifndef DECODEVELOFULLRAWBUFFER_H
#define DECODEVELOFULLRAWBUFFER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// local
#include "Tell1Kernel/VeloDecodeConf.h"
#include "Tell1Kernel/VeloDecodeCore.h"
#include "VeloEvent/EvtInfo.h"
#include "VeloEvent/VeloFullBank.h"
#include "VeloEvent/VeloTELL1Data.h"
#include "VeloFullDecoder.h"

/** @class DecodeVeloFullRawBuffer DecodeVeloFullRawBuffer.h
 *
 *
 *  @author Tomasz Szumlak
 *  @date   2006-04-21
 */

using namespace VeloTELL1;

class DecodeVeloFullRawBuffer : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
private:
  StatusCode getData();
  StatusCode decodeData();

  void sortAndWriteDecodedData();
  void setADCDataFlag();
  void setPedDataFlag();
  bool adcDataFlag();
  bool pedDataFlag();
  void resetMemory();
  void unsetADCDataFlag();
  void unsetPedDataFlag();

private:
  // location paths
  DataObjectReadHandle<VeloFullBanks> m_veloADCLocation{this, "ADCLocation", VeloFullBankLocation::Default};
  DataObjectReadHandle<VeloFullBanks> m_veloPartialADCLocation{this, "PartialADCLocation",
                                                               "Raw/Velo/PreparedPartialADC"};
  DataObjectReadHandle<VeloFullBanks> m_veloPedLocation{this, "PedestalLocation", VeloFullBankLocation::Pedestals};
  DataObjectWriteHandle<LHCb::VeloTELL1Datas> m_decodedADCLocation{this, "DecodedADCLocation",
                                                                   LHCb::VeloTELL1DataLocation::ADCs};
  DataObjectWriteHandle<LHCb::VeloTELL1Datas> m_decodedPartialADCLocation{this, "DecodedPartialADCLocation",
                                                                          "Raw/Velo/PartialADCs"};
  DataObjectWriteHandle<LHCb::VeloTELL1Datas> m_decodedPedLocation{this, "DecodedPedestalLocation",
                                                                   LHCb::VeloTELL1DataLocation::Pedestals};
  DataObjectWriteHandle<LHCb::VeloTELL1Datas> m_decodedHeaderLocation{this, "DecodedHeaderLocation",
                                                                      LHCb::VeloTELL1DataLocation::Headers};
  DataObjectWriteHandle<EvtInfos>             m_evtInfoLocation{this, "EventInfoLocation", EvtInfoLocation::Default};
  // input data
  const VeloFullBanks* m_veloADCs        = nullptr;
  const VeloFullBanks* m_veloPartialADCs = nullptr;
  const VeloFullBanks* m_veloPeds        = nullptr;
  // decoded data for futher processing
  LHCb::VeloTELL1Datas* m_decodedADC        = nullptr;
  LHCb::VeloTELL1Datas* m_decodedPartialADC = nullptr;
  LHCb::VeloTELL1Datas* m_decodedPed        = nullptr;
  LHCb::VeloTELL1Datas* m_decodedHeader     = nullptr;
  EvtInfos*             m_evtInfo           = nullptr;
  // flags
  bool                  m_adcDataPresent = false;
  bool                  m_pedDataPresent = false;
  Gaudi::Property<bool> m_sectorCorrection{this, "SectorCorrection", true};
  // data buffers
  sdataVec m_signADC                 = sdataVec( VeloTELL1::SENSOR_CHANNELS );
  sdataVec m_signPartialADC          = sdataVec( VeloTELL1::SENSOR_CHANNELS );
  sdataVec m_signADCReordered        = sdataVec( VeloTELL1::SENSOR_CHANNELS );
  sdataVec m_signPartialADCReordered = sdataVec( VeloTELL1::SENSOR_CHANNELS );
  sdataVec m_signHeader              = sdataVec( 256 );
  sdataVec m_signHeaderReordered     = sdataVec( 256 );
  sdataVec m_signPed                 = sdataVec( VeloTELL1::SENSOR_CHANNELS );
  sdataVec m_signPedReordered        = sdataVec( VeloTELL1::SENSOR_CHANNELS );
  // decoders
  VeloFullDecoder m_ADCDecoder{VeloFull};
  VeloFullDecoder m_ADCPartialDecoder{VeloFull};
  VeloFullDecoder m_HeaderDecoder{VeloHeader};
  VeloFullDecoder m_PedDecoder{VeloPedestal};
  // cable order
  Gaudi::Property<std::vector<unsigned int>> m_cableOrder{this, "CableOrder", {}};
};
#endif // DECODEVELOFULLRAWBUFFER_H

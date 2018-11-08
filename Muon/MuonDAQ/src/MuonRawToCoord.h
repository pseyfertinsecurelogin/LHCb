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
#ifndef MUONRAWTOCOORD_MUONRAWTOCOORD_H
#define MUONRAWTOCOORD_MUONRAWTOCOORD_H 1

// Include files
// from STL
#include <string>
#include <array>

// from Gaudi
#include "GaudiAlg/Transformer.h"

#include "MuonDet/MuonBasicGeometry.h"
#include "MuonDet/DeMuonDetector.h"
#include "Event/MuonCoord.h"
#include "Event/RawBank.h"
#include "boost/range/iterator_range.hpp"
#include "Event/RawEvent.h"
#include "Event/MuonODEData.h"
#include "Event/MuonPPEventInfo.h"
#include "DAQKernel/DecoderToolBase.h"

/** @class MuonRawToCoord MuonRawToCoord.h
 *  This is the muon reconstruction algorithm
 *  This just crosses the logical strips back into pads
 */
class MuonRawToCoord final : public Gaudi::Functional::Transformer<LHCb::MuonCoords (const LHCb::RawEvent &)> {
public:
  /// Standard constructor
  MuonRawToCoord( const std::string& name, ISvcLocator* pSvcLocator );

  //TODO make Digit and Digits common to all MuonDAQ
  using Digit = std::pair<LHCb::MuonTileID, unsigned int>;
  using Digits = std::vector<Digit>;
  using DigitsRange = boost::iterator_range<Digits::iterator>;
  StatusCode initialize() override;    ///< Algorithm initialization
  LHCb::MuonCoords operator()(const LHCb::RawEvent& event) const override;

private:

  StatusCode checkBankSize(const LHCb::RawBank*) const;
  std::vector<std::pair<LHCb::MuonTileID, unsigned int>> decodeTileAndTDCV1(const LHCb::RawBank*) const;

  /// Copy MuonTileID from digits to coord by crossing the digits
  void addCoordsCrossingMap(DigitsRange&, LHCb::MuonCoords&) const;

  /// fills in the two readout layouts by querying the DeMuonRegion
  void makeStripLayouts(unsigned station , unsigned region,
                              MuonLayout &layout1,
                              MuonLayout &layout2) const;

  DeMuonDetector* m_muonDetector = nullptr;
  int m_NStation = 0;
  int m_NRegion = 0;
  bool m_forceResetDAQ = false;

};
#endif // MUONRAWTOCOORD_MUONRAWTOCOORD_H

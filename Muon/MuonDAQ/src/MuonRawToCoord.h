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
 *
 *  @author David Hutchcroft
 *  @date   22/03/2002
 */
class MuonRawToCoord : public Gaudi::Functional::Transformer<LHCb::MuonCoords (const LHCb::RawEvent &)> {
public:
  /// Standard constructor
  MuonRawToCoord( const std::string& name, ISvcLocator* pSvcLocator );

  //TODO make Digit and Digits common to all MuonDAQ
  using Digit = std::pair<LHCb::MuonTileID, unsigned int>;
  using Digits = std::vector<Digit>;
  using DigitsRange = boost::iterator_range<Digits::iterator>;
  StatusCode initialize() override;    ///< Algorithm initialization
  LHCb::MuonCoords operator()(const LHCb::RawEvent& event) const override;
  //LHCb::MuonCoords operator()(Digits &decoding) const override;
//  LHCb::MuonCoords operator()(const Digits &decoding) const override;
  StatusCode finalize  () override;    ///< Algorithm finalization

  struct Coord {
      LHCb::MuonTileID m_pad;
      unsigned m_tdc1;
      unsigned m_tdc2;
      const LHCb::MuonTileID& m_tile1;
      const LHCb::MuonTileID& m_tile2;
      Coord(const LHCb::MuonTileID& pad, unsigned tdc1, unsigned tdc2, const LHCb::MuonTileID& tile1, const LHCb::MuonTileID& tile2) :
          m_pad(pad), m_tdc1(tdc1), m_tdc2(tdc2), m_tile1(tile1), m_tile2(tile2) {}
  };
  using Coords = std::vector<Coord>;
private:

  StatusCode checkBankSize(const LHCb::RawBank*) const;
//  StatusCode checkAllHeaders(const LHCb::RawEvent*);
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
//  mutable int  m_Exccounter = 0;

};
#endif // MUONRAWTOCOORD_MUONRAWTOCOORD_H

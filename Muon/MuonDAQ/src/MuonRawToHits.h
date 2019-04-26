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
#ifndef MUONRAWTOHITS_MUONRAWTOHITS_H
#define MUONRAWTOHITS_MUONRAWTOHITS_H 1

// Include files
// from STL
#include "boost/range/iterator_range.hpp"
#include <array>
#include <boost/numeric/conversion/cast.hpp>
#include <string>
#include <vector>

// from Gaudi
#include "GaudiAlg/Transformer.h"

#include "DAQKernel/DecoderToolBase.h"
#include "Event/MuonODEData.h"
#include "Event/MuonPPEventInfo.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "GaudiKernel/ToolHandle.h"
#include "MuonDAQ/MuonHitContainer.h"
#include "MuonDet/DeMuonDetector.h"
#include "MuonDet/IMuonFastPosTool.h"
#include "MuonDet/MuonBasicGeometry.h"

/** @class MuonRawToHits MuonRawToHits.h
 *  This is the muon reconstruction algorithm
 *  This just crosses the logical strips back into pads
 */

struct Digit {
  LHCb::MuonTileID tile;
  unsigned int     tdc;
};
using Digits      = std::vector<Digit>;
using DigitsRange = boost::iterator_range<Digits::iterator>;

class MuonRawToHits final : public Gaudi::Functional::Transformer<MuonHitContainer( const LHCb::RawEvent& )> {
public:
  /// Standard constructor
  MuonRawToHits( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode       initialize() override; ///< Algorithm initialization
  MuonHitContainer operator()( const LHCb::RawEvent& event ) const override;

private:
  std::array<std::vector<Digit>, 4> decodeTileAndTDC( LHCb::span<const LHCb::RawBank*> ) const;

  std::array<MuonLayout, 2> makeStripLayouts( const unsigned int, const unsigned int ) const;

  void addCoordsCrossingMap( DigitsRange&, CommonMuonHits& ) const;

  DeMuonDetector*                    m_muonDetector = nullptr;
  PublicToolHandle<IMuonFastPosTool> m_muonPosTool{this, "PosTool", "MuonFastPosTool"};

  size_t m_nStations = 0;
};
#endif // MUONRAWTOHITS_MUONRAWTOHITS_H

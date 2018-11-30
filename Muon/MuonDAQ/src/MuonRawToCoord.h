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
#ifndef MUONRAWTOCOORD_MUONRAWTOCOORD_H
#define MUONRAWTOCOORD_MUONRAWTOCOORD_H 1

#include "GaudiAlg/Transformer.h"

#include "MuonDet/MuonBasicGeometry.h"
#include "MuonDet/DeMuonDetector.h"
#include "Event/MuonCoord.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"

/** @class MuonRawToCoord MuonRawToCoord.h
 *  This is the muon reconstruction algorithm
 *  This just crosses the logical strips back into pads
 */
class MuonRawToCoord final : public Gaudi::Functional::Transformer<LHCb::MuonCoords (const LHCb::RawEvent &)> {
public:
  /// Standard constructor
  MuonRawToCoord( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  LHCb::MuonCoords operator()(const LHCb::RawEvent& event) const override;

private:

  template <typename OutputIterator>
  OutputIterator decodeTileAndTDCV1(const LHCb::RawBank&, OutputIterator) const;

  DeMuonDetector* m_muonDetector = nullptr;
  int m_NStation = 0;
  int m_NRegion = 0;
  bool m_forceResetDAQ = false;

  mutable Gaudi::Accumulators::BinomialCounter<>  m_invalid_add{ this, "invalid add" };
  mutable Gaudi::Accumulators::AveragingCounter<> m_digits{ this, "#digits" };
  mutable Gaudi::Accumulators::AveragingCounter<> m_coords{ this, "#coords" };

};
#endif // MUONRAWTOCOORD_MUONRAWTOCOORD_H

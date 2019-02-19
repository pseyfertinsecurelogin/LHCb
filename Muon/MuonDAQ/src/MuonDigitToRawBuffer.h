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
#ifndef MUONDIGITTORAWBUFFER_H
#define MUONDIGITTORAWBUFFER_H 1

// Include files
// from STL
#include <string>

#include "Kernel/MuonTileID.h"
#include "MuonDet/DeMuonDetector.h"
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

/** @class MuonDigitToRawBuffer MuonDigitToRawBuffer.h
 *
 *
 *  @author Alessia Satta
 *  @date   2004-01-19
 */
class MuonDigitToRawBuffer : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  StatusCode ProcessDC06();
  StatusCode ProcessV1();
  StatusCode ProcessDigitDC06();
  StatusCode ProcessDigitV1();
  StatusCode ProcessPads();

  DeMuonDetector* m_muonDet = nullptr;

  std::vector<unsigned int> m_digitsInODE[MuonDAQHelper_maxODENumber];
  std::vector<unsigned int> m_digitsInL1[MuonDAQHelper_maxTell1Number];
  unsigned int              firedInODE[MuonDAQHelper_maxODENumber];
  unsigned int              firedInPP[MuonDAQHelper_maxTell1Number * 4];

  std::vector<unsigned int> m_padInL1[MuonDAQHelper_maxTell1Number];

  long         m_TotL1Board = 0;
  unsigned int m_M1Tell1    = 0;

  Gaudi::Property<unsigned int> m_vtype{this, "VType", 2};
};
#endif // MUONDIGITTORAWBUFFER_H

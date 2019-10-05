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
#ifndef MUONDAQTEST_H
#define MUONDAQTEST_H 1

// Include files

#include "Event/MuonDigit.h"
#include "MuonDAQ/IMuonRawBuffer.h"

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
/** @class MuonDAQTest MuonDAQTest.h
 *
 *
 *  @author Alessia Satta
 *  @date   2005-10-19
 */
class MuonDAQTest : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  IMuonRawBuffer* m_MuonBuffer = nullptr;
};
#endif // MUONDAQTEST_H

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
#ifndef CHECKMCEVENTTOOL_H
#define CHECKMCEVENTTOOL_H 1

// Include files

// from Gaudi
#include "GaudiAlg/GaudiTool.h"

// interface
#include "Kernel/ICheckTool.h"

/** @class CheckMCEventTool CheckMCEventTool.h
 *  Tool to check integrity of MCEvent structure
 *
 *  @author Marco Cattaneo
 *  @date   2003-02-24
 */
class CheckMCEventTool : public extends<GaudiTool, ICheckTool> {
public:
  /// Standard constructor
  using base_class::base_class;

  StatusCode check() override; ///< Perform the check

private:
  long long  m_event  = -1;                  ///< last event number
  StatusCode m_status = StatusCode::SUCCESS; ///< Status of last check
};
#endif // CHECKMCEVENTTOOL_H

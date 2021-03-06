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
#include "GaudiAlg/GaudiTupleAlg.h"

class DeCalorimeter;

/** @class L0CaloNtpMonitor L0CaloNtpMonitor.h
 *
 *  @author Marie-Helene Schune
 *  @date 2008-02-22
 */
class L0CaloNtpMonitor : public GaudiTupleAlg {
public:
  /// Standard constructor
  L0CaloNtpMonitor( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~L0CaloNtpMonitor(); ///< Destructor

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

protected:
private:
  std::string m_inputDataSuffix;

  DeCalorimeter* m_ecal; ///< Pointer to Ecal detector element
  DeCalorimeter* m_hcal; ///< Pointer to Hcal detector element
};

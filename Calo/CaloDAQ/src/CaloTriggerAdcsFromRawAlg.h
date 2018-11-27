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
#ifndef CALOTRIGGERADCSFROMRAWALG_H
#define CALOTRIGGERADCSFROMRAWALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// CaloDAQ
#include "CaloDAQ/ICaloTriggerAdcsFromRaw.h"

/** @class CaloTriggerAdcsFromRawAlg CaloTriggerAdcsFromRawAlg.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2006-04-07
 */

class CaloTriggerAdcsFromRawAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloTriggerAdcsFromRawAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  Gaudi::Property<std::string> m_outputData { this, "OutputData" };          // Output container
  Gaudi::Property<std::string> m_pinContainer{ this, "PinContainer" } ;        // Output container for pinData
  Gaudi::Property<std::string> m_extension { this,  "Extension" };
  Gaudi::Property<bool> m_statusOnTES{ this,  "StatusOnTES"   , true };
  ICaloTriggerAdcsFromRaw*    m_l0AdcTool = nullptr;
};

#endif // CALOTRIGGERADCSFROMRAWALG_H

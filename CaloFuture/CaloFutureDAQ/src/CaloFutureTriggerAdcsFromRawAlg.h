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
#ifndef CALOFUTURETRIGGERADCSFROMRAWALG_H
#define CALOFUTURETRIGGERADCSFROMRAWALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// CaloFutureDAQ
#include "CaloFutureDAQ/ICaloFutureTriggerAdcsFromRaw.h"

/** @class CaloFutureTriggerAdcsFromRawAlg CaloFutureTriggerAdcsFromRawAlg.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2006-04-07
 */

class CaloFutureTriggerAdcsFromRawAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloFutureTriggerAdcsFromRawAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  Gaudi::Property<std::string> m_outputData { this, "OutputData" };          // Output container
  Gaudi::Property<std::string> m_pinContainer{ this, "PinContainer" } ;        // Output container for pinData
  Gaudi::Property<std::string> m_extension { this,  "Extension" };
  Gaudi::Property<bool> m_statusOnTES{ this,  "StatusOnTES"   , true };
  ICaloFutureTriggerAdcsFromRaw*    m_l0AdcTool = nullptr;
};

#endif // CALOFUTURETRIGGERADCSFROMRAWALG_H

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
#ifndef L0DUFROMRAWALG_H
#define L0DUFROMRAWALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
//Base
#include "L0Base/L0FromRawBase.h"

// Interfaces
#include "L0Interfaces/IL0DUFromRawTool.h"


/** @class L0DUFromRawAlg L0DUFromRawAlg.h
 *
 * Decode L0DU rawBank
 * Produce L0DUReport on TES
 * ReBuilt L0ProcessorData from L0DU rawBank
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-10-25
 */
class L0DUFromRawAlg : public L0FromRawBase {
public:
  /// Standard constructor
  L0DUFromRawAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

protected:

  void compareReports();

private:
  std::string m_L0DUReportLocation = LHCb::L0DUReportLocation::Default ;
  std::string m_procDataLocation   = LHCb::L0ProcessorDataLocation::L0DU  ;
  std::string m_fromRawTool = "L0DUFromRawTool" ;
  IL0DUFromRawTool* m_fromRaw = nullptr;
  IL0DUFromRawTool* m_fromRaw2 = nullptr;
  bool m_hlt1 = false;
  bool m_compare = false;
  bool m_ensureKnownTCK = false;
};
#endif // L0DUFROMRAWALG_H

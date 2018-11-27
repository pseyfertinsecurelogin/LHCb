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
#ifndef L0DATANTP_H
#define L0DATANTP_H 1

// Include files
#include "GaudiAlg/GaudiTupleAlg.h"
#include "CaloUtils/CaloAlgUtils.h"
#include "L0Interfaces/IL0DUFromRawTool.h"
#include "L0Interfaces/IL0DUConfigProvider.h"
#include "L0Interfaces/IL0DUEmulatorTool.h"


/** @class L0DataNtp L0DataNtp.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2012-04-10
 */
class L0DataNtp : public GaudiTupleAlg {
public:
  /// Standard constructor
  L0DataNtp( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  std::string rename(std::string name);
  std::vector<std::string> m_list;
  IL0DUFromRawTool* m_l0;
  IL0DUConfigProvider* m_l0conf;
  IL0DUEmulatorTool* m_l0emu;
  bool m_scale;
  std::vector<std::string> m_tcks;
};
#endif // L0DATANTP_H

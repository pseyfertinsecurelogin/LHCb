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
#ifndef L0DATAFILTER_H
#define L0DATAFILTER_H 1

// Include files
#include "L0Base/L0AlgBase.h"


/** @class L0DataFilter L0DataFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2012-04-10
 */
class L0DataFilter : public L0AlgBase {
public:
  /// Standard constructor
  L0DataFilter( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  std::string m_l0Location;
  std::string m_logical;
  std::map<std::string,std::vector<std::string> > m_selection ;
  bool m_revert;
};
#endif // L0DATAFILTER_H

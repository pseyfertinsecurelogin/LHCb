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
#ifndef L0TCKFILTER_H
#define L0TCKFILTER_H 1

// Include files
// from Gaudi
#include "L0Base/L0AlgBase.h"


/** @class L0TCKfilter L0TCKfilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-03-25
 */
class L0TCKfilter : public L0AlgBase {
public:
  /// Standard constructor
  L0TCKfilter( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute   () override;    ///< Algorithm execution

private:

  std::vector<std::string> m_list;
  std::string m_reportLocation;


};
#endif // L0TCKFILTER_H

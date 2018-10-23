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
#ifndef ICALOFUTURESHOWEROVERLAPTOOL_H 
#define ICALOFUTURESHOWEROVERLAPTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

namespace LHCb{
  class CaloCluster;
}


/** @class ICaloFutureShowerOverlapTool ICaloFutureShowerOverlapTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2014-06-03
 */
struct ICaloFutureShowerOverlapTool : extend_interfaces<IAlgTool> 
{
  // Return the interface ID
  DeclareInterfaceID(ICaloFutureShowerOverlapTool, 2, 0 );

  virtual StatusCode setProfile(std::string)=0;
  virtual void process(const LHCb::CaloCluster* c1,const LHCb::CaloCluster* c2,
                       int spd=0,int niter=5,bool propagateInitialWeights=false)=0;
  
};
#endif // ICALOFUTURESHOWEROVERLAPTOOL_H

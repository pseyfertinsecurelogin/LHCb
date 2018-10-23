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
#ifndef IFORCEDBDECAYTOOL_H 
#define IFORCEDBDECAYTOOL_H 1

#include <string>
#include "GaudiKernel/IAlgTool.h"

namespace LHCb {
  class MCParticle;
}


/** @class IForcedBDecayTool IForcedBDecayTool.h 
 *  
 *  v1.2
 *  @author Marco Musy (Milano)
 *  @date   2007-02-06
 */
struct IForcedBDecayTool : extend_interfaces<IAlgTool> {
  DeclareInterfaceID(IForcedBDecayTool, 2 , 0); 
  virtual const LHCb::MCParticle*  forcedB( void )=0;
};
#endif // IFORCEDBDECAYTOOL_H

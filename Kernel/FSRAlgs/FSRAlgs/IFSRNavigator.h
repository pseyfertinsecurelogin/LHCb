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
#ifndef IFSRNAVIGATOR_H
#define IFSRNAVIGATOR_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/IRegistry.h"

// forward declarations
namespace LHCb {
  class LumiFSR;
  class LumiIntegral;
}


/** @class IFSRNavigator IFSRNavigator.h
 *
 *
 *  @author Jaap Panman
 *  @date   2010-10-05
 */
class IFSRNavigator : public extend_interfaces<IAlgTool> {
public:
  DeclareInterfaceID(IFSRNavigator, 2, 0 );

  // return list of addresses found below the address
  virtual std::vector< std::string > navigate(std::string, std::string) = 0;

protected:
  virtual void explore(IRegistry* pObj, std::string tag, std::vector< std::string >& addresses) = 0;

};
#endif // IFSRNAVIGATOR_H

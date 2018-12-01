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
#ifndef IL0DUCONFIGPROVIDER_H
#define IL0DUCONFIGPROVIDER_H 1

// Include files
// from STL
#include <string>
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
//from Event
#include "Event/L0DUConfig.h"


/** @class IL0DUConfigProvider IL0DUConfigProvider.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-09-19
 */
struct IL0DUConfigProvider : extend_interfaces<IAlgTool> {

  // Declare the interface ID
  DeclareInterfaceID( IL0DUConfigProvider, 3, 0 );

  virtual LHCb::L0DUConfig* config(long tck,std::string slot="T0")=0;
  virtual LHCb::L0DUConfig::Container* configs(std::string slot="T0")=0;

};
#endif // IL0DUCONFIGPROVIDER_H

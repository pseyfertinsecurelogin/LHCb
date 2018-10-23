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
#ifndef MUONDAQ_IMUONTELL1CONFIG_H 
#define MUONDAQ_IMUONTELL1CONFIG_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"


/** @class IMuonTell1Config IMuonTell1Config.h MuonDAQ/IMuonTell1Config.h
 *  
 *
 *  @author 
 *  @date   2008-02-29
 */
struct IMuonTell1Config : extend_interfaces<IAlgTool> {
  DeclareInterfaceID(  IMuonTell1Config, 2, 0 );
  virtual StatusCode configTell1(int Tell1) const = 0;
};


#endif // MUONDAQ_IMUONTELL1CONFIG_H

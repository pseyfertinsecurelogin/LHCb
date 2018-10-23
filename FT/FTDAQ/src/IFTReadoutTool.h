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
#ifndef _IFTReadoutTool_H
#define _IFTReadoutTool_H 1

#include "GaudiKernel/IAlgTool.h"
#include "Kernel/FTChannelID.h"

/** @class IFTReadoutTool IFTReadoutTool.h  FTDAQ/IFTReadoutTool
*
*  Interface Class for things related to the Readout - Tell1 Board
*
*  @author L.Henry
*  @date   07/11/2017
*/

struct IFTReadoutTool: extend_interfaces< IAlgTool >{

  /// Static access to interface id
  DeclareInterfaceID(IFTReadoutTool, 1, 0);
  
  virtual LHCb::FTChannelID channelIDShift(const unsigned int bankNumber ) const = 0;
  virtual unsigned int bankNumber        (LHCb::FTChannelID id    ) const = 0;
  virtual unsigned int nBanks() const = 0;
};

#endif // _IFTReadoutTool_H

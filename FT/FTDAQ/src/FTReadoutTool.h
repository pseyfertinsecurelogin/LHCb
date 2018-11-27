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
#ifndef _FTReadoutTool_H
#define _FTReadoutTool_H 1

#include "GaudiAlg/GaudiTool.h"
#include "Kernel/FTChannelID.h"
#include "IFTReadoutTool.h"

/** @class FTReadoutTool FTReadoutTool.h
 *
 *  Concrete class for things related to the Readout - Tell40 Board
 *
 *  @author L.Henry
 *  @date   20/10/2017
*/

class FTReadoutTool : public extends<GaudiTool, IFTReadoutTool>{
 public:
  using base_class::base_class;  

  StatusCode initialize() override;
  StatusCode readFile();

  // First FTChannelID <-> banknumber
  LHCb::FTChannelID channelIDShift(unsigned int bankNumber) const override;
  unsigned int bankNumber(LHCb::FTChannelID id) const override;
  
  //Getters
  unsigned int nBanks() const override { return m_FTBankFirstChannel.size(); };

 private:

  std::vector<LHCb::FTChannelID> m_FTBankFirstChannel;
  bool m_initialized = false;

  Gaudi::Property<std::string> m_conditionLocation { this, "conditionLocation",
    "/dd/Conditions/ReadoutConf/FT/ReadoutMap" };

};

#endif // _FTReadoutTool_H

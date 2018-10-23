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
#ifndef CALODAQ_ICALOTRIGGERBITSFROMRAW_H 
#define CALODAQ_ICALOTRIGGERBITSFROMRAW_H 1

// Include files
// from STL
#include <string>

#include "ICaloReadoutTool.h"
#include "Kernel/CaloCellID.h"


/** @class ICaloTriggerBitsFromRaw ICaloTriggerBitsFromRaw.h CaloDAQ/ICaloTriggerBitsFromRaw.h
 *  Interface to the Prs/SPD bit decoding
 *
 *  @author Olivier Callot
 *  @date   2005-11-07
 */
namespace LHCb{
  namespace Calo{
  typedef std::vector<LHCb::CaloCellID> FiredCells;
  typedef std::pair<FiredCells,FiredCells> PrsSpdFiredCells;
  }
}

struct ICaloTriggerBitsFromRaw : extend_interfaces<ICaloReadoutTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloTriggerBitsFromRaw, 4, 0 );
  virtual const LHCb::Calo::FiredCells& prsCells( ) = 0;
  virtual const LHCb::Calo::FiredCells& spdCells( ) = 0;
  virtual const LHCb::Calo::PrsSpdFiredCells& prsSpdCells( ) = 0;
  virtual const LHCb::Calo::PrsSpdFiredCells& prsSpdCells(int source ) = 0;
  virtual const LHCb::Calo::PrsSpdFiredCells& prsSpdCells(const LHCb::RawBank& bank ) = 0;

};
#endif // CALODAQ_ICALOTRIGGERBITSFROMRAW_H

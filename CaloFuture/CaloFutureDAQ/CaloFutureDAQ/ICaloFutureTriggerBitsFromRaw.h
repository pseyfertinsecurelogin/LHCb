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
#ifndef CALOFUTUREDAQ_ICALOFUTURETRIGGERBITSFROMRAW_H 
#define CALOFUTUREDAQ_ICALOFUTURETRIGGERBITSFROMRAW_H 1

// Include files
// from STL
#include <string>

#include "ICaloFutureReadoutTool.h"
#include "Kernel/CaloCellID.h"


/** @class ICaloFutureTriggerBitsFromRaw ICaloFutureTriggerBitsFromRaw.h CaloFutureDAQ/ICaloFutureTriggerBitsFromRaw.h
 *  Interface to the Prs/SPD bit decoding
 *
 *  @author Olivier Callot
 *  @date   2005-11-07
 */
namespace LHCb{
  namespace CaloFuture{
  typedef std::vector<LHCb::CaloCellID> FiredCells;
  typedef std::pair<FiredCells,FiredCells> PrsSpdFiredCells;
  }
}

struct ICaloFutureTriggerBitsFromRaw : extend_interfaces<ICaloFutureReadoutTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloFutureTriggerBitsFromRaw, 4, 0 );
  virtual const LHCb::CaloFuture::FiredCells& prsCells( ) = 0;
  virtual const LHCb::CaloFuture::FiredCells& spdCells( ) = 0;
  virtual const LHCb::CaloFuture::PrsSpdFiredCells& prsSpdCells( ) = 0;
  virtual const LHCb::CaloFuture::PrsSpdFiredCells& prsSpdCells(int source ) = 0;
  virtual const LHCb::CaloFuture::PrsSpdFiredCells& prsSpdCells(const LHCb::RawBank& bank ) = 0;

};
#endif // CALOFUTUREDAQ_ICALOFUTURETRIGGERBITSFROMRAW_H

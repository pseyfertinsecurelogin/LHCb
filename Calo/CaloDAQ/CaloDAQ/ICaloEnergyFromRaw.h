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
#ifndef CALODAQ_ICALOENERGYFROMRAW_H 
#define CALODAQ_ICALOENERGYFROMRAW_H 1

// Include files
// from STL
#include <string>

#include "ICaloReadoutTool.h"
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"


/** @class ICaloEnergyFromRaw ICaloEnergyFromRaw.h CaloDAQ/ICaloEnergyFromRaw.h
 *  Interface to the tool decoding the (cell+energy) from Raw
 *
 *  @author Olivier Callot
 *  @date   2005-01-10
 */
struct ICaloEnergyFromRaw : extend_interfaces<ICaloReadoutTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloEnergyFromRaw, 5, 0 );

  virtual const std::vector<LHCb::CaloAdc>&   adcs( ) = 0;
  virtual const std::vector<LHCb::CaloAdc>&   adcs( int source ) = 0;
  virtual const std::vector<LHCb::CaloAdc>&   adcs( const LHCb::RawBank& bank ) = 0;
  virtual const std::vector<LHCb::CaloAdc>&   pinAdcs( ) = 0;
  
  virtual const std::vector<LHCb::CaloDigit>& digits( ) = 0;
  virtual const std::vector<LHCb::CaloDigit>& digits( int source  ) = 0;
  virtual const std::vector<LHCb::CaloDigit>& digits( const LHCb::RawBank& bank ) = 0;
  
};
#endif // CALODAQ_ICALOENERGYFROMRAW_H

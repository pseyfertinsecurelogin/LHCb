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
#ifndef CALOFUTUREDAQ_ICALOFUTURETRIGGERADCSFROMRAW_H 
#define CALOFUTUREDAQ_ICALOFUTURETRIGGERADCSFROMRAW_H 1

// Include files
// from STL
#include <string>

#include "ICaloFutureReadoutTool.h"
#include "Event/L0CaloAdc.h"


/** @class ICaloFutureTriggerAdcsFromRaw ICaloFutureTriggerAdcsFromRaw.h CaloFutureDAQ/ICaloFutureTriggerAdcsFromRaw.h
 *  INterface for trigger adc decoding
 *
 *  @author Olivier Callot
 *  @date   2005-01-05
 */
struct ICaloFutureTriggerAdcsFromRaw : extend_interfaces<ICaloFutureReadoutTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloFutureTriggerAdcsFromRaw, 4, 0 );

  virtual const std::vector<LHCb::L0CaloAdc>&    adcs( ) = 0;
  virtual const std::vector<LHCb::L0CaloAdc>&    adcs( int source ) = 0;
  virtual const std::vector<LHCb::L0CaloAdc>&    adcs( const LHCb::RawBank& bank ) = 0;
  virtual const std::vector<LHCb::L0CaloAdc>&    pinAdcs( ) = 0;

};
#endif // CALOFUTUREDAQ_ICALOFUTURETRIGGERADCSFROMRAW_H

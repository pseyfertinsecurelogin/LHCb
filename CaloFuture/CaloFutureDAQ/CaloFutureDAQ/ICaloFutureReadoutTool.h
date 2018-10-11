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
#ifndef CALOFUTUREDAQ_ICALOFUTUREREADOUTTOOL_H 
#define CALOFUTUREDAQ_ICALOFUTUREREADOUTTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/STLExtensions.h"
#include "Event/RawEvent.h"
#include "Event/RawBankReadoutStatus.h"
#include "CaloDet/DeCalorimeter.h"


/** @class ICaloFutureReadoutTool ICaloFutureReadoutTool.h CaloFutureDAQ/ICaloFutureReadoutTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2007-07-30
 */
struct ICaloFutureReadoutTool : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( ICaloFutureReadoutTool, 6, 0 );

  virtual StatusCode  _setProperty(const std::string& p,const std::string& v)=0;
  virtual bool getBanks()=0;
  virtual void setBanks(LHCb::span<const LHCb::RawBank*> bank )=0;
  virtual void clear()=0;
  virtual void cleanData(int feb)=0;
  virtual LHCb::RawBankReadoutStatus& status()=0;
  virtual void putStatusOnTES()=0;
  virtual bool ok()=0;
  virtual DeCalorimeter* deCaloFuture()=0;

};
#endif // CALOFUTUREDAQ_ICALOFUTUREREADOUTTOOL_H

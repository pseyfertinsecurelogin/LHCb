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
#ifndef CALODAQ_ICALOREADOUTTOOL_H
#define CALODAQ_ICALOREADOUTTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "CaloDet/DeCalorimeter.h"
#include "Event/RawBankReadoutStatus.h"
#include "Event/RawEvent.h"
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/STLExtensions.h"

/** @class ICaloReadoutTool ICaloReadoutTool.h CaloDAQ/ICaloReadoutTool.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-07-30
 */
struct ICaloReadoutTool : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( ICaloReadoutTool, 6, 0 );

  virtual StatusCode                  _setProperty( const std::string& p, const std::string& v ) = 0;
  virtual bool                        getBanks()                                                 = 0;
  virtual void                        setBanks( LHCb::span<const LHCb::RawBank*> bank )          = 0;
  virtual void                        clear()                                                    = 0;
  virtual void                        cleanData( int feb )                                       = 0;
  virtual LHCb::RawBankReadoutStatus& status()                                                   = 0;
  virtual void                        putStatusOnTES()                                           = 0;
  virtual bool                        ok()                                                       = 0;
  virtual DeCalorimeter*              deCalo()                                                   = 0;
};
#endif // CALODAQ_ICALOREADOUTTOOL_H

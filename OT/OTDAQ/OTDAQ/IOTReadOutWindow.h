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
#ifndef OTSIMULATION_IOTREADOUTWINDOW_H 
#define OTSIMULATION_IOTREADOUTWINDOW_H 1

// Include files
#include "GaudiKernel/IAlgTool.h"

/** @class IOTReadOutWindow IOTReadOutWindow.h "OTSimulation/IOTReadOutWindow.h"
 *  
 *  Outer tracker ReadOutWindow interface.
 *
 *  @author Jeroen van Tilburg
 *  @date   11/11/2002
 */
struct IOTReadOutWindow : extend_interfaces<IAlgTool> {
  /// Retrieve interface ID
  DeclareInterfaceID(IOTReadOutWindow, 2, 0 );
  /// Start of read out window for each station
  virtual std::vector<double> startReadOutGate() const = 0;
  /// Size of the read out window
  virtual double sizeOfReadOutGate() const = 0;
};
#endif // OTSIMULATION_IOTREADOUTWINDOW_H

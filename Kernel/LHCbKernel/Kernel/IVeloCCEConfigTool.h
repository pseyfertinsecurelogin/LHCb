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
#ifndef IVELOCCECONFIGTOOL_H
#define IVELOCCECONFIGTOOL_H 1

// Include files

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

/** @class IVeloCCEConfigTool IVeloCCEConfigTool.h
 *  Interface class for CCE scan parameter tool
 *
 *  @author Jon Harrison
 *  @date   2016-03-16
 */

// Declaration of  the interface ID

struct IVeloCCEConfigTool : extend_interfaces<IAlgTool> {
  DeclareInterfaceID(IVeloCCEConfigTool, 2, 0 );

  /** List of killed sensors and bias voltage of test
  ** sensor for a step in the CCE scan
  **/
  virtual int findKilledSensors(int &CCEstep, std::vector<int> &killSensorList) const = 0;
  /** List of bad strips for a sensor in the CCE scan
  **/
  virtual void findBadStrips(unsigned int &sensorNum, std::vector<int> &badStripList) const = 0;

};
#endif // IVELOCCECONFIGTOOL_H

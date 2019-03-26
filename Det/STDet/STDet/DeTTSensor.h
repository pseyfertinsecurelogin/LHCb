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
#ifndef _DeTTSensor_H_
#define _DeTTSensor_H_

#include <string>

#include "Kernel/STChannelID.h"
#include "STDet/DeSTSensor.h"

/** @class DeTTSensor DeTTSensor.h "STDet/DeTTSensor.h"
 *
 *  Class representing a TT Sensor
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 *
 * <b> Additional information: </b>
 *   \li  <a href="http://ckm.physik.unizh.ch/software"><b>ST Software page</b></a><p>
 *
 */

class DeSTSector;

static const CLID CLID_DeTTSensor = 9130;

class DeTTSensor : public DeSTSensor {

public:
  /** parent type */
  typedef STDetTraits<DeTTSensor>::parent parent_type;

  /** Constructor */
  DeTTSensor( const std::string& name = "" );

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID() { return CLID_DeTTSensor; }

  /**
   * another reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override;

  /** initialization method
   * @return Status of initialisation
   */
  StatusCode initialize() override;

private:
  parent_type* m_parent;
};

#endif // _DeTTSensor_H

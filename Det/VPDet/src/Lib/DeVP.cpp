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
#include <algorithm>

// Gaudi
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/SystemOfUnits.h"

// Det/DetDesc
#include "DetDesc/Condition.h"

// Local
#include "VPDet/DeVP.h"
#include "getOutputLevel.h"


//============================================================================
// Object identification
//============================================================================
const CLID& DeVP::clID() const {

  return DeVP::classID();

}

//============================================================================
// Initialisation method
//============================================================================
StatusCode DeVP::initialize() {

  auto sc = initOutputLevel(msgSvc(), "DeVP");
  if (!sc) return sc;

  const auto lvl = msgSvc()->outputLevel("DeVP");
  m_debug = lvl <= MSG::DEBUG;

  // Initialise the base class.
  sc = DetectorElement::initialize();
  if (sc.isFailure()) {
    msg() << MSG::ERROR << "Cannot initialize DetectorElement" << endmsg;
    return sc;
  }

  // Get all daughter detector elements.
  std::vector<DeVPSensor*> sensors;
  findSensors(this, sensors);
  if (m_debug) {
    msg() << MSG::DEBUG << "Found " << sensors.size() << " sensors" << endmsg;
  }
  std::sort(sensors.begin(), sensors.end(), less_SensorNumber());

  unsigned int nLeftSensors = 0;
  unsigned int nRightSensors = 0;
  for (auto it = sensors.begin(), end = sensors.end(); it != end; ++it) {
    m_sensors.push_back(*it);
    if ((*it)->isLeft()) {
      ++nLeftSensors;
    } else {
      ++nRightSensors;
    }
  }
  msg() << MSG::INFO << "There are " << m_sensors.size() << " sensors "
        << "(left: " << nLeftSensors << ", right: " << nRightSensors << ")"
        << endmsg;
  return StatusCode::SUCCESS;

}

//============================================================================
// Get sensitive volume identifier for a given point in the global frame
//============================================================================
int DeVP::sensitiveVolumeID(const Gaudi::XYZPoint& point) const {

  const double z = point.z();
  constexpr double tol = 10 * Gaudi::Units::mm;
  auto i = std::find_if( m_sensors.begin(), m_sensors.end(),
                         [&]( const DeVPSensor* s) {
    // Quickly skip sensors which are far away in z, only
    // then check whether point is inside this sensor
    return (fabs(z - s->z()) < tol) && s->isInside(point);
  });
  if (i!=m_sensors.end()) return (*i)->sensorNumber();
  msg() << MSG::ERROR << "No sensitive volume at (" << point.x() << ", "
        << point.y() << ", " << point.z() << ")" << endmsg;
  return -999;

}

//============================================================================
// Scan detector element tree for VP sensors
//============================================================================
void DeVP::findSensors(IDetectorElement* det,
                       std::vector<DeVPSensor*>& sensors) {

  // Get the daughter detector elements.
  auto elements = det->childIDetectorElements();
  for (auto it = elements.begin(), end = elements.end(); it != end; ++it) {
    if (m_debug) {
      msg() << MSG::DEBUG << std::setw(12) << std::setiosflags(std::ios::left)
            << (*it)->name() << endmsg;
    }
    // Check if this is a VP sensor.
    DeVPSensor* p = dynamic_cast<DeVPSensor*>(*it);
    if (p) {
      // Add the sensor to the list.
      sensors.push_back(p);
      if (m_debug) {
        msg() << MSG::DEBUG << "Storing detector " << (*it)->name() << endmsg;
      }
    } else {
      // Recurse.
      findSensors(*it, sensors);
    }
  }
}

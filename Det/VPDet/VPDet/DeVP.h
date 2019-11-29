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
#ifndef VPDET_DEVP_H
#define VPDET_DEVP_H 1

// Local
#include "VPDet/DeVPSensor.h"

/** @class DeVP DeVP.h VPDet/DeVP.h
 *
 *  Detector element class for the VP as a whole.
 *
 *  @author Victor Coco Victor.Coco@cern.ch
 *  @date 20/5/2009
 */

constexpr CLID CLID_DeVP = 8200;

namespace DeVPLocation {
  inline const std::string Default = "/dd/Structure/LHCb/BeforeMagnetRegion/VP";
}

class DeVP : public DetectorElement {

public:
  /// Constructor
  using DetectorElement::DetectorElement;

  /// Object identification
  static const CLID&  classID() { return CLID_DeVP; }
  virtual const CLID& clID() const override;

  /// Initialization method
  StatusCode initialize() override;

  /// Return sensitive volume identifier for a given point in the global frame.
  int sensitiveVolumeID( const Gaudi::XYZPoint& point ) const override;

  /// Return the number of sensors.
  unsigned int numberSensors() const { return m_sensors.size(); }

  template<class Operation>
  /// runs the given callable on every sensor
  void runOnAllSensors( Operation op ) const {
    for ( auto& sensor : m_sensors ) op(*sensor);
  }

  /// Return pointer to sensor for a given point in the global frame.
  const DeVPSensor& sensor( const Gaudi::XYZPoint& point ) const {
    const int sensorNumber = sensitiveVolumeID( point );
    if (sensorNumber >= 0) return *m_sensors[sensorNumber];
    throw std::runtime_error("Invalid sensor number in DeVP");
  }
  /// Return pointer to sensor for a given channel ID.
  const DeVPSensor& sensor( LHCb::VPChannelID channel ) const { return sensor( channel.sensor() ); }
  /// Return pointer to sensor for a given sensor number.
  const DeVPSensor& sensor( unsigned int sensorNumber ) const { return *m_sensors[sensorNumber]; }

private:
  /// Find sensors inside detector element tree.
  void findSensors( IDetectorElement* det, std::vector<DeVPSensor*>& sensors );

  /// List of pointers to all sensors.
  std::vector<DeVPSensor*> m_sensors;

  /// Custom operator for sorting sensors by sensor number.
  struct less_SensorNumber {
    bool operator()( DeVPSensor* const& x, DeVPSensor* const& y ) { return ( x->sensorNumber() < y->sensorNumber() ); }
  };

  /// Output level flag
  bool m_debug = false;

  /// Message stream
  mutable std::unique_ptr<MsgStream> m_msg;
  /// On-demand access to message stream.
  MsgStream& msg() const {
    if ( !m_msg ) m_msg.reset( new MsgStream( msgSvc(), "DeVP" ) );
    return *m_msg;
  }
};

#endif

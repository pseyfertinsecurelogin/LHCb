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

static const CLID CLID_DeVP = 8200;

#ifdef __INTEL_COMPILER         // Disable ICC remark
#pragma warning(disable : 177)  // variable was declared but never referenced
#endif
namespace DeVPLocation {
static const std::string& Default = "/dd/Structure/LHCb/BeforeMagnetRegion/VP";
}

class DeVP : public DetectorElement {

 public:

  /// Constructor
  DeVP(const std::string& name = "");
  /// Destructor
  virtual ~DeVP();

  /// Object identification
  static const CLID& classID() { return CLID_DeVP; }
  virtual const CLID& clID() const;

  /// Initialization method
  virtual StatusCode initialize();

  /// Return sensitive volume identifier for a given point in the global frame.
  virtual int sensitiveVolumeID(const Gaudi::XYZPoint& point) const;

  /// Return the number of sensors.
  unsigned int numberSensors() const { return m_sensors.size(); }

  /// Return vector of sensors.
  const std::vector<DeVPSensor*>& sensors() const { return m_sensors; }

  /// Return pointer to sensor for a given point in the global frame.
  const DeVPSensor* sensor(const Gaudi::XYZPoint& point) const {
    const int sensorNumber = sensitiveVolumeID(point);
    return sensorNumber >= 0 ? m_sensors[sensorNumber] : nullptr;
  }
  /// Return pointer to sensor for a given channel ID.
  const DeVPSensor* sensorOfChannel(const LHCb::VPChannelID channel) const {
    return sensor(channel.sensor());
  }
  /// Return pointer to sensor for a given sensor number.
  const DeVPSensor* sensor(const unsigned int sensorNumber) const {
    return m_sensors[sensorNumber];
  }

 private:

  /// Find sensors inside detector element tree.
  void findSensors(IDetectorElement* det, std::vector<DeVPSensor*>& sensors);

  /// List of pointers to all sensors.
  std::vector<DeVPSensor*> m_sensors;

  /// Custom operator for sorting sensors by sensor number.
  struct less_SensorNumber {
    bool operator()(DeVPSensor* const& x, DeVPSensor* const& y) {
      return (x->sensorNumber() < y->sensorNumber());
    }
  };

  /// Output level flag
  bool m_debug = false;

  /// Message stream
  mutable std::unique_ptr<MsgStream> m_msg;
  /// On-demand access to message stream.
  MsgStream& msg() const {
    if (!m_msg) m_msg.reset( new MsgStream(msgSvc(), "DeVP") );
    return *m_msg;
  }

};

#endif

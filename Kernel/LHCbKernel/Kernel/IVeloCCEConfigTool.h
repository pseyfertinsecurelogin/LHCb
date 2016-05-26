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
static const InterfaceID IID_IVeloCCEConfigTool( "IVeloCCEConfigTool", 1, 0 );

class IVeloCCEConfigTool : virtual public IAlgTool {
public:
  /// Static access to interface id
  static const InterfaceID& interfaceID() { return IID_IVeloCCEConfigTool; }

  /** List of killed sensors and bias voltage of test
  ** sensor for a step in the CCE scan
  **/
  virtual int FindKilledSensors(int &CCEstep, std::vector<int> &killSensorList) const = 0;
  /** List of bad strips for a sensor in the CCE scan
  **/
  virtual void FindBadStrips(unsigned int &sensorNum, std::vector<int> &badStripList) const = 0;

};
#endif // IVELOCCECONFIGTOOL_H 

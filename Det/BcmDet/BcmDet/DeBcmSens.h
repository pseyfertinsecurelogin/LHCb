#ifndef BCMDET_DEBCMSENS_H
#define BCMDET_DEBCMSENS_H 1

#include <string>
// Include files
#include <vector>

#include "DetDesc/DetectorElement.h"
#include "DetDesc/IGeometryInfo.h"
#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/StatusCode.h"


static const CLID& CLID_DEBcmSens = 14000;

class DeBcmSens: public DetectorElement {

public:

  DeBcmSens();

  DeBcmSens(int nSensor);

  ~DeBcmSens();

  inline static const CLID& classID(){
    return CLID_DEBcmSens;
  }

  inline const CLID& clID() const override {
    return classID();
  }

  StatusCode initialize() override;

  inline int sensorNumber() const {
    return m_SensorNumber;
  }

  void setSensorNumber(int nSensor){
    m_SensorNumber = nSensor;
  }

  int sensitiveVolumeID( const Gaudi::XYZPoint& point ) const override;

private:
  int m_SensorNumber;

};

#endif

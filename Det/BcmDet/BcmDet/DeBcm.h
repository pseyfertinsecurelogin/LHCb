#ifndef BCMDET_DEBCM_H
#define BCMDET_DEBCM_H 1

// Include files
#include <vector>
#include <string>

#include "DetDesc/DetectorElement.h"
#include "DetDesc/IGeometryInfo.h"

static const CLID& CLID_DeBcm = 14001;

namespace DeBcmLocation{
  static const std::string& BcmUp = "/dd/Structure/LHCb/BeforeMagnetRegion/BcmUp";
  static const std::string& BcmDown = "/dd/Structure/LHCb/MagnetRegion/BcmDown";
  static const std::string& Default
#ifdef __GNUC__
    __attribute__ ((unused))
#endif
    = BcmUp;
}

namespace DeBcmShifts{
    static const unsigned int shiftStationID         = 3;
    static const unsigned int shiftSensorID          = 0;
}

class DeBcm: public DetectorElement {

public:

  DeBcm(int nStation = 0);

  inline static const CLID& classID(){
    return CLID_DeBcm;
  }

  inline const CLID& clID() const override {
    return classID();
  }

  StatusCode initialize() override;

  inline int stationNumber() const {
    return m_StationNumber;
  }

  void setStationNumber(int nStation){
    m_StationNumber = nStation;
  }

  int sensitiveVolumeID( const Gaudi::XYZPoint& point ) const override;

private:
  int m_StationNumber;

};

#endif

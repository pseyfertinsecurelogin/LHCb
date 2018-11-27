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
#ifndef BCMDET_DEBCMSENS_H
#define BCMDET_DEBCMSENS_H 1

// Include files
#include <vector>
#include <string>

#include "DetDesc/DetectorElement.h"
#include "DetDesc/IGeometryInfo.h"


static const CLID& CLID_DEBcmSens = 14000;

class DeBcmSens: public DetectorElement {

public:

  DeBcmSens(int nSensor = 0);


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

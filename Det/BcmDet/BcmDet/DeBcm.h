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
#ifndef BCMDET_DEBCM_H
#define BCMDET_DEBCM_H 1

// Include files
#include <string>
#include <vector>

#include "DetDesc/DetectorElement.h"
#include "DetDesc/IGeometryInfo.h"

static const CLID& CLID_DeBcm = 14001;

namespace DeBcmLocation {
  inline const std::string BcmUp   = "/dd/Structure/LHCb/BeforeMagnetRegion/BcmUp";
  inline const std::string BcmDown = "/dd/Structure/LHCb/MagnetRegion/BcmDown";
  inline const std::string Default = BcmUp;
} // namespace DeBcmLocation

namespace DeBcmShifts {
  constexpr unsigned int shiftStationID = 3;
  constexpr unsigned int shiftSensorID  = 0;
} // namespace DeBcmShifts

class DeBcm : public DetectorElement {

public:
  DeBcm( int nStation = 0 );

  static const CLID& classID() { return CLID_DeBcm; }

  const CLID& clID() const override { return classID(); }

  StatusCode initialize() override;

  int stationNumber() const { return m_StationNumber; }

  void setStationNumber( int nStation ) { m_StationNumber = nStation; }

  int sensitiveVolumeID( const Gaudi::XYZPoint& point ) const override;

private:
  int m_StationNumber;
};

#endif

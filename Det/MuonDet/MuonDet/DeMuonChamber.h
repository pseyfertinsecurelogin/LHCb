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
// ============================================================================
#ifndef MUONDET_DEMUONCHAMBER_H
#define MUONDET_DEMUONCHAMBER_H 1

// Include files
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "GaudiKernel/MsgStream.h"

#include "DetDesc/DetectorElement.h"
#include "DetDesc/PVolume.h"
#include "MuonDet/MuonNamespace.h"

/** @class DeMuonChamber DeMuonChamber.h MuonDet/DeMuonChamber.h
 *
 *  Detector element class for a single chamber in the muon system
 *
 *  @author David Hutchcroft
 *  @date   21/01/2002
 */

/// Class ID of chambers
static const CLID CLID_DEMuonChamber = 11006;

class DeMuonChamber : public DetectorElement {

public:
  /// Constructor (empty)
  DeMuonChamber() = default;

  /// Constructor used by XmlMuonRegionCnv to create chambers
  /// pad sizes in mm
  DeMuonChamber( int nStation, int nRegion, int nChamber );

  inline static const CLID& classID() { return CLID_DEMuonChamber; }

  inline const CLID& clID() const override { return classID(); }

  // Initialize
  StatusCode initialize() override;

  /// get Station Number
  //  inline int stationNumber() const {
  inline std::string stationNumber() const { return m_StationName.str(); }

  /// set Station Number (from 0 -> nStations)
  void setStationNumber( int nStation ) {
    m_StationNumber = nStation;
    m_StationName << "M" << nStation + 1;
  }

  /// get Region Number
  inline int regionNumber() const { return m_RegionNumber; }

  /// set Region Number
  void setRegionNumber( int nRegion ) { m_RegionNumber = nRegion; }

  /// get Chamber Number
  inline int chamberNumber() const { return m_ChamberNumber; }

  /// set Chamber Number
  void setChamberNumber( int nChamber ) { m_ChamberNumber = nChamber; }

  /// get chamber Grid
  inline std::string getGridName() const { return m_chmbGrid; }

  /// set chamber Grid
  void setGridName( std::string grid ) { m_chmbGrid = grid; }

  IPVolume*  getFirstGasGap();
  IPVolume*  getGasGap( int number );
  int        getGasGapNumber();
  StatusCode isPointInGasGap( Gaudi::XYZPoint pointInChamber, Gaudi::XYZPoint& pointInGap, IPVolume*& gasVolume );
  StatusCode isPointInGasGap( Gaudi::XYZPoint pointInChamber, Gaudi::XYZPoint& pointInGap, int& number,
                              IPVolume*& gasVolume );
  IPVolume*  getGasGapLayer( int number );

private:
  /// Access to Msgstream object
  inline MsgStream& msgStream() const {
    if ( UNLIKELY( !m_msgStream ) ) m_msgStream.reset( new MsgStream( msgSvc(), name() ) );
    return *m_msgStream;
  }

  mutable std::unique_ptr<MsgStream> m_msgStream;

  /// Chamber Grid
  std::string m_chmbGrid;

  /// Station number
  int m_StationNumber = 0;

  /// Region number in station
  int m_RegionNumber = 0;

  /// Chamber number in region
  int m_ChamberNumber = 0;

  /// Station name
  std::stringstream m_StationName;
};

#endif // MUONDET_DEMUONCHAMBER_H

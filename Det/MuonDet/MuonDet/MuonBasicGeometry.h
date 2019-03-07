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
#ifndef MUONDET_MUONBASICGEOMETRY_H
#define MUONDET_MUONBASICGEOMETRY_H 1

#include <string>
#include <vector>

class IDataProviderSvc;
class IMessageSvc;

// Include files

/** @class MuonBasicGeometry MuonBasicGeometry.h MuonAlgs/MuonBasicGeometry.h
 *
 *
 *  @author Alessia Satta
 *  @date   2002-10-07
 */
class MuonBasicGeometry {
public:
  /// Standard constructor
  MuonBasicGeometry( IDataProviderSvc* detSvc, IMessageSvc* msgSvc );
  ~MuonBasicGeometry(){}; ///< Destructor
  int         getStations();
  int         getRegions();
  int         getPartitions();
  std::string getStationName( int station );
  int         getStationNumber( std::string stationName );
  int         retrieveM1status();

private:
  int                                      m_stationNumber;
  int                                      m_regionNumber;
  int                                      m_isM1defined;
  int                                      m_partition;
  std::vector<std::pair<std::string, int>> m_stations;
};
#endif // MUONALGS_MUONBASICGEOMETRY_H

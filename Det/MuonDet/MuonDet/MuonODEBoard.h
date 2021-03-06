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
#ifndef MUONDET_MUONODEBOARD_H
#define MUONDET_MUONODEBOARD_H 1

// Include files
#include "DetDesc/Condition.h"
#include "GaudiKernel/DataObject.h"

// Include files
#include "Kernel/MuonTileID.h"
#include "MuonDet/CLID_MuonODEBoard.h"

/** @class MuonODEBoard MuonODEBoard.h MuonDet/MuonODEBoard.h
 *
 *
 *  @author Alessia Satta
 *  @date   2004-01-12
 */
class MuonODEBoard : public Condition {
public:
  /// Standard constructor
  MuonODEBoard();

  virtual ~MuonODEBoard(); ///< Destructor
  inline static const CLID& classID() { return CLID_MuonODEBoard; }

  using Condition::initialize;
  void initialize( long number, long region );

  using Condition::update;
  StatusCode update( long tslayx, long tslayy, long tsnumb, std::vector<long> gridx, std::vector<long> gridy,
                     std::vector<long> quadrant );

  StatusCode         addTSName( std::string name );
  inline long        getODESerialNumber() { return m_ODENumber; }
  inline long        getTSLayoutX() { return m_TSLayoutX; }
  inline long        getTSLayoutY() { return m_TSLayoutY; }
  inline long        getTSNumber() { return m_TSNumber; }
  inline long        getTSGridX( int i ) { return m_TSGridX[i]; }
  inline long        getTSGridY( int i ) { return m_TSGridY[i]; }
  inline long        getTSQuadrant( int i ) { return m_TSQuadrant[i]; }
  inline std::string getTSName( int i ) { return m_TSName[i]; }
  inline void        setRegion( long i ) { m_region = i; }
  inline long        region() { return m_region; }
  void               setQuadrants();
  bool               isQuadrantContained( long quadrant );
  bool               isTSContained( LHCb::MuonTileID TSTile );
  void               setECSName( std::string name ) { m_ECSName = name; }

  inline std::string ECSName() { return m_ECSName; }

protected:
private:
  long                     m_region;
  long                     m_ODENumber;
  long                     m_TSLayoutX;
  long                     m_TSLayoutY;
  long                     m_TSNumber;
  std::vector<long>        m_TSGridX;
  std::vector<long>        m_TSGridY;
  std::vector<long>        m_TSQuadrant;
  std::vector<std::string> m_TSName;
  std::string              m_ECSName;
  long                     m_quadrant[4];
};
#endif // MUONDET_MUONODEBOARD_H

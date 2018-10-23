/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef MUONDET_MUONL1BOARD_H
#define MUONDET_MUONL1BOARD_H 1
#include <vector>
#include "GaudiKernel/DataObject.h"
#include "DetDesc/Condition.h"


// Include files
#include "MuonDet/CLID_MuonL1Board.h"
//static const CLID  CLID_MuonL1Board = 11090;

/** @class MuonL1Board MuonL1Board.h MuonDet/MuonL1Board.h
 *
 *
 *  @author Alessia Satta
 *  @date   2004-01-05
 */
class MuonL1Board:public Condition {
public:
  /// Standard constructor
  MuonL1Board( ) = default;

  /// Class ID of this class
  static  const CLID& classID() {
    return CLID_MuonL1Board;
  }
  long L1Number(){return m_L1Number;};
  void  setL1Number(long number){m_L1Number=number;};
  void  setL1Station(long number){m_station=number;};

  long numberOfODE(){return m_numberOfODE;};
  void setNumberOfODE(long ode){m_numberOfODE=ode;};
  void setLinkConnection(long link,long ode) {m_linkConnection[link]=ode;};
  long getLinkConnection(long link){return m_linkConnection[link];};
  bool isLinkConnected(long link){ return m_linkConnection[link]>0; };

  void addODE(long number, std::string name);
  void addLayout(long region, long X,long Y);
  std::string getODEName(int i){return m_ODEName[i];};
  long getTSLayoutX(long i){return m_regionLayout[i][0];}  ;
  long getTSLayoutY(long i){return m_regionLayout[i][1];}  ;
  long getODEPosition(long number,bool hole=true);
  int getStation(){return m_station;};

private:
  long m_station;
  long m_numberOfODE;
  long m_L1Number;
  std::vector<long> m_ODENumberList;
  std::vector<std::string> m_ODEName;
  long m_regionLayout[4][2]  ;
  long m_linkConnection[24];
};
#endif // MUONDET_MUONL1BOARD_H

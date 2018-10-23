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
#ifndef L0DUEMULATORTOOL_H
#define L0DUEMULATORTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
// Interfaces
#include "L0Interfaces/IL0DUEmulatorTool.h"
#include "L0Interfaces/IL0ProcessorDataDecoder.h"
#include "L0Interfaces/IL0CondDBProvider.h"
#include "GaudiKernel/IEventTimeDecoder.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"


/** @class L0DUEmulatorTool L0DUEmulatorTool.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-10-10
 */
class L0DUEmulatorTool : public extends<GaudiTool, IL0DUEmulatorTool, IIncidentListener> {
public:
  /// Standard constructor
  L0DUEmulatorTool( const std::string& type,
                   const std::string& name,
                   const IInterface* parent);

  StatusCode initialize() override;
  StatusCode process(LHCb::L0DUConfig* config, LHCb::L0ProcessorDatas* datas) override;
  StatusCode process(LHCb::L0DUConfig* config, std::vector<std::string> dataLocs) override;
  const std::vector<unsigned int> bank(unsigned int version) override;
  const LHCb::L0DUReport emulatedReport() override;


  void handle(const Incident& /* inc */ ) override {
    if ( msgLevel(MSG::DEBUG) ) debug() << "IIncident Svc reset" << endmsg;
    m_begEvent = true ;
    for (auto& i : m_procMap) i.second=true;
  }

private:
  std::vector<int> bxList(const std::array<unsigned int,L0DUBase::Index::Size>& base);
  unsigned long digit(const std::array<unsigned int, L0DUBase::Index::Size>& base,int bx=0);
  double        scale(const std::array<unsigned int, L0DUBase::Index::Size>& base);
  long          max  (const std::array<unsigned int, L0DUBase::Index::Size>& base);
  void setDataValue(LHCb::L0DUElementaryData* data,
                    const std::array<unsigned int,L0DUBase::Index::Size>& base);
  StatusCode processing();
  StatusCode fillData();
  StatusCode dataTree(LHCb::L0DUElementaryData* data, LHCb::L0DUElementaryData::Map dataMap);

  //
  LHCb::L0DUReport          m_report;              ///<  L0DU Report
  bool                      m_muCleaning;          ///<  Activate muon cleaning
  bool                      m_muZeroSup;           ///<  Activate muon 0-suppression
  bool                      m_emuBX;               ///<  Emulate Prev2/Prev1/Next2/Next1 data in the bank
  std::vector<int>          m_muHighest;           ///<  Array of  index of the 3 highest muon (Pt-decreasing order)
  int                       m_muPattern = 0;       ///<  Pattern of muon cleaning
  int                       m_nMu;
  IL0ProcessorDataDecoder*  m_decoder = nullptr;
  IL0CondDBProvider*        m_condDB = nullptr;
  IEventTimeDecoder*        m_odin = nullptr;
  LHCb::L0DUConfig*         m_config = nullptr;
  bool                      m_begEvent = true;
  std::map<unsigned int,bool> m_procMap;
};
#endif // L0DUEMULATORTOOL_H

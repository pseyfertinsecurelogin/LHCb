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
#ifndef L0DUREPORTMONITOR_H
#define L0DUREPORTMONITOR_H 1

// Include files
// from Gaudi
#include "L0Base/L0AlgBase.h"
// from Event
#include "Event/L0DUReport.h"
#include <TAxis.h>
#include <TH1D.h>
#include <TH2D.h>

/** @class L0DUReportMonitor L0DUReportMonitor.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-10-25
 */
class L0DUReportMonitor : public L0AlgBase {
public:
  /// Standard constructor
  L0DUReportMonitor( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  typedef std::map<std::string, int>                            SeqMap;
  typedef std::map<std::string, double>                         CounterMap;
  typedef std::map<unsigned int, std::map<std::string, double>> CounterMapMap;

  SeqMap m_condSeq;
  SeqMap m_chanSeq;
  SeqMap m_trigSeq;

  CounterMap m_chanCnt;
  CounterMap m_condCnt;
  CounterMap m_chanRate;
  CounterMap m_condRate;
  CounterMap m_trigCnt;
  CounterMap m_trigRate;

  CounterMapMap m_chanCntMap;
  CounterMapMap m_condCntMap;
  CounterMapMap m_chanRateMap;
  CounterMapMap m_condRateMap;
  CounterMapMap m_trigCntMap;
  CounterMapMap m_trigRateMap;

  std::map<int, CounterMap>                     m_chanRelRate;
  std::map<int, CounterMap>                     m_trigRelRate;
  std::map<int, CounterMapMap>                  m_trigRelRateMap;
  std::map<int, CounterMapMap>                  m_chanRelRateMap;
  std::map<int, double>                         m_decCnt;
  std::map<int, std::map<unsigned int, double>> m_decCntMap;
  std::map<unsigned int, double>                m_evtCntMap;
  std::map<int, std::map<std::string, int>>     m_condBXMap;
  std::map<int, std::map<std::string, int>>     m_condErrorMap;
  double                                        m_evtCnt;

  std::map<unsigned int, LHCb::L0DUConfig*> m_configs;
  int                                       m_prevTCK;
  std::string                               m_reportLocation;
  bool                                      m_data;
  bool                                      m_cond;
  bool                                      m_chan;
  bool                                      m_trig;
  bool                                      m_dec;
  bool                                      m_full;
  int                                       m_bin;
  bool                                      m_split;
  std::string                               m_lab;
  void label( AIDA::IHistogram1D* h1d, std::map<int, std::string> labels, bool count = true, std::string opt = "" );
  void label( AIDA::IHistogram2D* h2d, std::map<int, std::string> labels, int mask = 0x3, bool count = true,
              std::string opt = "" );
  bool m_generic;
  bool m_swap;
  std::map<std::string, int> m_condBX;
  std::map<std::string, int> m_condError;
};
#endif // L0DUREPORTMONITOR_H

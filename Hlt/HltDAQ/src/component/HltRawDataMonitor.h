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
#ifndef HLTRAWDATAMONITOR_H
#define HLTRAWDATAMONITOR_H 1

// Include files
// from Gaudi
#include "AIDA/IAxis.h"
#include "AIDA/IHistogram1D.h"
#include "Event/HltObjectSummary.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiAlg/GaudiTupleAlg.h"
#include "Kernel/IANNSvc.h"

/** @class HltRawDataMonitor HltRawDataMonitor.h
 *
 *
 *  @author Chun-Min Jen
 *  @date   2008-10-07
 **/

class HltRawDataMonitor : public GaudiHistoAlg {
public:
  enum HeaderIDs { kVersionNumber = 2 };

  /// Standard constructor
  HltRawDataMonitor( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  /// location of input
  std::string              m_inputRawEventLocation;
  std::vector<std::string> m_rawEventLocations;

  /// SourceID to decode 0=Hlt 1=Hlt1 2=Hlt2 ... (1,2 will decode from 0 if 1,2 not found)
  UnsignedIntegerProperty m_sourceID;

  /// <0 never 0=at finalize >0 event frequency
  IntegerProperty m_diagnosticsFrequency;

private:
  typedef std::pair<std::pair<std::string, AIDA::IHistogram1D*>, double> HltSortedSelName;
  class sort {
  public:
    bool operator()( const HltSortedSelName& q1, const HltSortedSelName& q2 ) const;
  };

  void fillDiag( const char* trigName, double length );

  StatusCode fillRawBank();
  StatusCode fillHltSelRep();
  StatusCode selectionDiagnostics();

  int    nLength( const LHCb::HltObjectSummary& p );
  double nMedian( const AIDA::IHistogram1D& q );
  double nMedian2( unsigned int NumOfEvt, const AIDA::IHistogram1D& q );
  double nMean2( unsigned int NumOfEvt, const AIDA::IHistogram1D& q );
  double nRMS2( unsigned int NumOfEvt, const AIDA::IHistogram1D& q );

  // lots of different sorts of histograms are declared here
  // call/book histograms here
  AIDA::IHistogram1D* m_bankSize    = 0;
  AIDA::IHistogram1D* m_hitSize     = 0;
  AIDA::IHistogram1D* m_objtypSize  = 0;
  AIDA::IHistogram1D* m_substrSize  = 0;
  AIDA::IHistogram1D* m_stdinfoSize = 0;
  AIDA::IHistogram1D* m_extinfoSize = 0;

  std::vector<std::pair<std::string, AIDA::IHistogram1D*>> m_hltSelNameList;
  std::vector<HltSortedSelName>                            m_hltRankedSelName;

  unsigned int m_event = 0;
};
#endif // HLTRAWDATAMONITOR_H

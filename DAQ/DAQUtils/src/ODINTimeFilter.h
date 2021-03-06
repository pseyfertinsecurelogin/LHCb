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
#ifndef ODINTIMEFILTER_H
#define ODINTIMEFILTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// from LHCb
#include "Event/ODIN.h"

/** @class ODINTimeFilter ODINTimeFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2009-10-07
 */
class ODINTimeFilter : public GaudiAlgorithm {
public:
  /// Standard constructor
  ODINTimeFilter( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

protected:
private:
  void criteriaPrintOut();
  int  val( int v1, int v2, int v3 = 0 );
  bool check( unsigned long long val, std::pair<double, double> range );
  bool check( int val, std::pair<int, int> range );
  bool def( std::pair<double, double> range );
  bool def( std::pair<int, int> range );

  mutable Gaudi::Accumulators::Counter<> m_eventCount{this, "Event"};
  mutable Gaudi::Accumulators::Counter<> m_filteredEventCount{this, "Filtered Events"};

  std::pair<int, int>       m_yRange;
  std::pair<int, int>       m_mRange;
  std::pair<int, int>       m_dRange;
  std::pair<int, int>       m_hRange;
  std::pair<int, int>       m_mnRange;
  std::pair<int, int>       m_sRange;
  std::pair<int, int>       m_nsRange;
  std::pair<int, int>       m_bRange;
  std::pair<double, double> m_eRange;
  bool                      m_evt;
  bool                      m_time;
  bool                      m_bx;
  bool                      m_loc;
  bool                      m_print;
};
#endif // ODINTIMEFILTER_H

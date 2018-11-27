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
#ifndef FASTL0DUFILTER_H
#define FASTL0DUFILTER_H 1

// Include files
// from Gaudi
#include "L0Base/L0FromRawBase.h"
#include "Event/RawEvent.h"


/** @class FastL0DUFilter FastL0DUFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-11-12
 */
class FastL0DUFilter : public L0FromRawBase {
public:
  /// Standard constructor
  FastL0DUFilter( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:
  int m_source;
  bool m_noMuonSup;
  long m_sumCut;
  long m_haCut;
  long m_elCut;
  long m_muCut;
  long m_spdCut;
  bool m_useDecInBank;
  unsigned long m_count;
  unsigned long m_sel;

};
#endif // FASTL0DUFILTER_H

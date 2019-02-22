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
// $Id: MuonTSMap.h,v 1.3 2009-07-08 12:42:47 cattanem Exp $
#ifndef MUONDET_MUONTSMAP_H
#define MUONDET_MUONTSMAP_H 1
#include "DetDesc/Condition.h"
#include "GaudiKernel/DataObject.h"
#include <vector>

// Include files
#include "MuonDet/CLID_MuonTSMap.h"
/** @class MuonTSMap MuonTSMap.h MuonDet/MuonTSMap.h
 *
 *
 *  @author Alessia Satta
 *  @date   2004-01-05
 */
class MuonTSMap : public Condition {
public:
  /// Standard constructor
  MuonTSMap();

  virtual ~MuonTSMap(); ///< Destructor
  /// Class ID of this class
  inline static const CLID& classID() { return CLID_MuonTSMap; }

  using Condition::initialize;
  StatusCode initialize( long num, long gridx[2], long gridy[2] );

  using Condition::update;
  StatusCode update( long output, std::vector<long> lay, std::vector<long> gridx, std::vector<long> gridy,
                     std::vector<long> synch );

  inline long numberOfLayout() { return m_NumberLogLayout; }
  inline long gridXLayout( int i ) { return m_GridXLayout[i]; }
  inline long gridYLayout( int i ) { return m_GridYLayout[i]; }
  inline long numberOfOutputSignal() { return m_OutputSignal; }
  inline long layoutOutputChannel( int i ) { return m_OutputLayoutSequence[i]; }
  inline long gridXOutputChannel( int i ) { return m_OutputGridXSequence[i]; }
  inline long gridYOutputChannel( int i ) { return m_OutputGridYSequence[i]; }
  inline long synchChSize() { return m_OutputSynchSequence.size(); }
  inline bool synchChUsed( int i ) { return m_OutputSynchSequence[i] == 0 ? false : true; }
  inline long numberOfPad() { return m_pad; }

protected:
private:
  long              m_NumberLogLayout;
  long              m_GridXLayout[2];
  long              m_GridYLayout[2];
  long              m_OutputSignal;
  std::vector<long> m_OutputLayoutSequence;
  std::vector<long> m_OutputGridXSequence;
  std::vector<long> m_OutputGridYSequence;
  std::vector<long> m_OutputSynchSequence;
  long              m_pad;
};

#endif // MUONDET_MUONTSMAP_H

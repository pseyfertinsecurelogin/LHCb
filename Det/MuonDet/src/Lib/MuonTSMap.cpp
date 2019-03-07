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
// Include files

// local
#include "MuonDet/MuonTSMap.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MuonTSMap
//
// 2004-01-13 : Alessia Satta
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonTSMap::MuonTSMap() {}
//=============================================================================
// Destructor
//=============================================================================
MuonTSMap::~MuonTSMap() {}

//=============================================================================
StatusCode MuonTSMap::initialize( long num, long gridx[2], long gridy[2] ) {
  m_NumberLogLayout = num;
  m_GridXLayout[0]  = gridx[0];
  m_GridXLayout[1]  = gridx[1];
  m_GridYLayout[0]  = gridy[0];
  m_GridYLayout[1]  = gridy[1];
  if ( m_NumberLogLayout == 1 ) {
    m_pad = m_GridXLayout[0] * m_GridYLayout[0];
  } else {
    if ( m_GridXLayout[0] > m_GridXLayout[1] ) {
      m_pad = m_GridXLayout[0] * m_GridYLayout[1];
    } else
      m_pad = m_GridXLayout[1] * m_GridYLayout[0];
  }

  return StatusCode::SUCCESS;
}

StatusCode MuonTSMap::update( long output, std::vector<long> lay, std::vector<long> gridx, std::vector<long> gridy,
                              std::vector<long> synch ) {
  m_OutputSignal         = output;
  m_OutputLayoutSequence = lay;
  m_OutputGridXSequence  = gridx;
  m_OutputGridYSequence  = gridy;
  m_OutputSynchSequence  = synch;
  return StatusCode::SUCCESS;
}

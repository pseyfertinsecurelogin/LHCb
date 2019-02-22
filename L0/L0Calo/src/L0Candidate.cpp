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
// $Id: L0Candidate.cpp,v 1.1 2009-10-14 10:18:52 robbep Exp $
// local
#include "L0Candidate.h"

// From CaloDet
#include "CaloDet/DeCalorimeter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : L0Candidate
//-----------------------------------------------------------------------------

//=====================================================================
// Functions of the auxilliary class L0Candidate
//=====================================================================
void L0Candidate::setCandidate( int et, LHCb::CaloCellID ID ) {
  m_ID = ID;
  if ( 255 >= et ) {
    m_et = et;
  } else {
    m_et = 255;
  }

  // If the cluster is identified by a non-existent cell, one uses the other
  // cells of the cluster in the order:
  // the top cell: (+1,0)
  // the right cell: (0,+1) if the top does not exist
  // the corner cell: (+1,+1) if the two others do not exist

  if ( !m_det->valid( ID ) ) {

    LHCb::CaloCellID topCell( ID.calo(), ID.area(), ID.row() + 1, ID.col() );
    m_ID = topCell;

    if ( !m_det->valid( topCell ) ) {
      LHCb::CaloCellID rightCell( ID.calo(), ID.area(), ID.row(), ID.col() + 1 );
      m_ID = rightCell;

      if ( !m_det->valid( rightCell ) ) {
        LHCb::CaloCellID cornerCell( ID.calo(), ID.area(), ID.row() + 1, ID.col() + 1 );
        m_ID = cornerCell;
      }
    }
  }
}

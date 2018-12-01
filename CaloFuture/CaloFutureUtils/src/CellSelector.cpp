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
#include "CaloDet/DeCalorimeter.h"
#include "CaloFutureUtils/CellSelector.h"
// constructor
CellSelector::CellSelector( const DeCalorimeter* det , std::string selector )
  : CellMatrix( det ), m_selector( selector ){
  m_cell3x3.setDet( det );
  m_cell2x2.setDet( det );
  m_cellSwissCross.setDet( det );
  m_cellNeighbour.setDet( det );
}
void CellSelector::setDet(const DeCalorimeter* det ){
  CellMatrix::setDet( det );
  m_cell3x3.setDet( det );
  m_cell2x2.setDet( det );
  m_cellSwissCross.setDet( det );
  m_cellNeighbour.setDet( det );
}

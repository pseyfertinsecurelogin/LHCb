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
#include "CaloFutureUtils/CellSelector.h"
#include "CaloDet/DeCalorimeter.h"
// constructor
CellSelector::CellSelector( const DeCalorimeter* det, CellSelector::Selector selector )
    : CellMatrix( det ), m_selector( selector ) {
  m_cell3x3.setDet( det );
  m_cell2x2.setDet( det );
  m_cellSwissCross.setDet( det );
  m_cellNeighbour.setDet( det );
}
void CellSelector::setDet( const DeCalorimeter* det ) {
  CellMatrix::setDet( det );
  m_cell3x3.setDet( det );
  m_cell2x2.setDet( det );
  m_cellSwissCross.setDet( det );
  m_cellNeighbour.setDet( det );
}
StatusCode parse( CellSelector::Selector& s, const std::string& str ) {
  for ( auto r : {CellSelector::Selector::s3x3, CellSelector::Selector::s2x2, CellSelector::Selector::SwissCross,
                  CellSelector::Selector::Neighbour} ) {
    if ( str != toString( r ) ) continue;
    s = r;
    return StatusCode::SUCCESS;
  }
  return StatusCode::FAILURE;
}

namespace {
  const std::string s_3x3        = "3x3";
  const std::string s_2x2        = "2x2";
  const std::string s_SwissCross = "SwissCross";
  const std::string s_Neighbour  = "Neighbour";
  const std::string s_invalid    = "INVALID";
} // namespace
const std::string& toString( CellSelector::Selector s ) {
  switch ( s ) {
  case CellSelector::Selector::s3x3:
    return s_3x3;
  case CellSelector::Selector::s2x2:
    return s_2x2;
  case CellSelector::Selector::SwissCross:
    return s_SwissCross;
  case CellSelector::Selector::Neighbour:
    return s_Neighbour;
  }
  return s_invalid;
}

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
#ifndef CALOFUTUREUTILS_CELLSELECTOR_H
#define CALOFUTUREUTILS_CELLSELECTOR_H 1
#include "CaloDet/DeCalorimeter.h"
#include "CaloFutureUtils/CellMatrix.h"
#include "CaloFutureUtils/CellMatrix2x2.h"
#include "CaloFutureUtils/CellMatrix3x3.h"
#include "CaloFutureUtils/CellNeighbour.h"
#include "CaloFutureUtils/CellSwissCross.h"

/** @class CellSelector CellSelector.h CaloFutureUtils/CellSelector.h
 *
 */

class CellSelector final : public CellMatrix {
public:
  CellSelector( const DeCalorimeter* det = 0, std::string selector = "" );

  void          setDet( const DeCalorimeter* det );
  inline void   setSelector( std::string selector ) { m_selector = selector; };
  inline double operator()( const LHCb::CaloCellID& seed, const LHCb::CaloCellID& cell ) const {
    if ( "3x3" == m_selector )
      return m_cell3x3( seed, cell );
    else if ( "2x2" == m_selector )
      return m_cell2x2( seed, cell );
    else if ( "SwissCross" == m_selector )
      return m_cellSwissCross( seed, cell );
    else if ( "Neighbour" == m_selector )
      return m_cellNeighbour( seed, cell );
    else
      return 1.;
  };

private:
  std::string    m_selector;
  CellMatrix3x3  m_cell3x3;
  CellMatrix2x2  m_cell2x2;
  CellSwissCross m_cellSwissCross;
  CellNeighbour  m_cellNeighbour;
};

#endif // CALOFUTUREUTILS_CELLSELECTOR_H

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
#ifndef CALOUTILS_CELLSELECTOR_H
#define CALOUTILS_CELLSELECTOR_H 1
#include "CaloDet/DeCalorimeter.h"
#include "CaloUtils/CellMatrix.h"
#include "CaloUtils/CellMatrix2x2.h"
#include "CaloUtils/CellMatrix3x3.h"
#include "CaloUtils/CellNeighbour.h"
#include "CaloUtils/CellSwissCross.h"
#include <iomanip>

/** @class CellSelector CellSelector.h CaloUtils/CellSelector.h
 *
 */

class CellSelector final : public CellMatrix {
public:
  enum class Selector { s3x3, s2x2, SwissCross, Neighbour };
  friend StatusCode         parse( Selector&, const std::string& );
  friend const std::string& toString( Selector );
  friend std::ostream& toStream( Selector s, std::ostream& os ) { return os << std::quoted( toString( s ), '\'' ); }
  friend std::ostream& operator<<( std::ostream& os, Selector s ) { return toStream( s, os ); }

  CellSelector( const DeCalorimeter* det = nullptr, Selector = Selector{-1} );

  void   setDet( const DeCalorimeter* det );
  void   setSelector( Selector selector ) { m_selector = selector; };
  double operator()( const LHCb::CaloCellID& seed, const LHCb::CaloCellID& cell ) const {
    switch ( m_selector ) {
    case Selector::s3x3:
      return m_cell3x3( seed, cell );
    case Selector::s2x2:
      return m_cell2x2( seed, cell );
    case Selector::SwissCross:
      return m_cellSwissCross( seed, cell );
    case Selector::Neighbour:
      return m_cellNeighbour( seed, cell );
    default:
      return 1.;
    }
  };

private:
  Selector       m_selector;
  CellMatrix3x3  m_cell3x3;
  CellMatrix2x2  m_cell2x2;
  CellSwissCross m_cellSwissCross;
  CellNeighbour  m_cellNeighbour;
};

#endif // CALOUTILS_CELLSELECTOR_H

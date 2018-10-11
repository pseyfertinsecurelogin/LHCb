/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
// Include files
#include <algorithm>
#include "CaloKernel/CaloException.h"
#include "CaloDet/DeCalorimeter.h"
#include "CaloUtils/CellNeighbour.h"

// ============================================================================
/** @file CellNeighbour.cpp
 *
 *  Implementation file for class : CellNeighbour
 *  
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date 25/11/2001 
 */
// ============================================================================



// ============================================================================
/** the major method. It returns "true" if the cell 
 *  is a neighbor for seed cell  
 *  @exeption CaloException if detector information is not available
 *  @param seed  the seed   cell
 *  @param cell  the cell to be tested
 *  @return fraction of area of the second cell which fits inside 
 *          "neighbour" aroud the seed cell, equivalently 0 or 1 
 */
// ============================================================================
double CellNeighbour::operator() 
  ( const LHCb::CaloCellID& seed , 
    const LHCb::CaloCellID& cell ) const
{
  /// trivial case 
  if ( seed == cell                                   ) { return 1 ; } 
  if ( 0 == det() ) { Exception("CellNeighbour:: Detector points to NULL!");}
  ///
  if ( !det()->valid( seed ) || !det()->valid( cell ) ) { return 0 ; }
  /// 
  const CaloNeighbors& neighbours = det()->neighborCells( seed ) ;
  auto it = std::find ( neighbours.begin () , neighbours.end () , cell ) ;
  return neighbours.end() == it ? 0.0 : 1.0 ;
}


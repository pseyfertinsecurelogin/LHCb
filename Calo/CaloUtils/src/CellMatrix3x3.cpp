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
// ============================================================================
// Include files
#include "CaloUtils/CellMatrix3x3.h"
#include "CaloDet/DeCalorimeter.h"

// ============================================================================
/** @file CellMatrix3x3.cpp
 *
 *  Implementation file for class : CellMatrix3x3
 *
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 * @date 07/11/2001
 */
// ============================================================================

// ============================================================================
/** if cells belong to different areas
 *  one need to treat them in a different way
 *  @param cell  the cell to be tested
 *  @return fraction of area of the second cell which fits inside
 *          3x3 matrix aroud the seed cell
 */
// ============================================================================
double CellMatrix3x3::treatDifferentAreas( const LHCb::CaloCellID& seed, const LHCb::CaloCellID& cell ) const {
  /// valid detector
  if ( 0 == det() ) { Exception( "CellMatrix3x3::treatDifferentAreas: DeCalo=0!" ); }
  /// both cells are valid?
  if ( !det()->valid( seed ) || !det()->valid( cell ) ) { return 0; }
  /// cell sizes
  const double sizeMtrx = 0.5 * 3 * det()->cellSize( seed );
  const double sizeCell = 0.5 * det()->cellSize( cell );
  /// cell area
  const double cellArea = sizeCell * sizeCell * 4.0;
  /// cell centers
  const Gaudi::XYZPoint& centerMtrx = det()->cellCenter( seed );
  const Gaudi::XYZPoint& centerCell = det()->cellCenter( cell );
  /// effective halfsize
  const double sizeMin = 0.01 * std::min( sizeMtrx, sizeCell );
  ///
  const double Area = area( centerMtrx, sizeMtrx, centerCell, sizeCell );
  ///
  if ( Area <= sizeMin ) { return 0; } ///< RETURN
  ///
  return Area / cellArea;
}

// ============================================================================
// The End
// ============================================================================

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
#ifndef CALOFUTUREUTILS_CELLMATRIX3X3_H
#define CALOFUTUREUTILS_CELLMATRIX3X3_H 1
// Include files
// CaloFutureUtils
#include "CaloFutureUtils/CellMatrix.h"
//

/** @class CellMatrix3x3 CellMatrix3x3.h CaloFutureUtils/CellMatrix3x3.h
 *
 *  The simple utility class to determine if 2 cells belongs
 *  to the same 3x3 matrix
 *
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   07/11/2001
 */

class CellMatrix3x3 final : public CellMatrix {
public:
  /** the standard constructor
   *  @param Det pointer to calorimeter detector
   */
  CellMatrix3x3( const DeCalorimeter* Det = 0 ) : CellMatrix( Det ){};

  /** the major method. It returns "true" if the cell
   *  belongs to 3x3 sub-matrix around the seed
   *  @param seed  the seed   cell
   *  @param cell  the cell to be tested
   *  @return fraction of area of the second cell which fits inside
   *          3x3 matrix aroud the seed cell
   */
  inline double operator()( const LHCb::CaloCellID& seed, const LHCb::CaloCellID& cell ) const {
    /// the same cells
    if ( seed == cell ) { return 1; } ///< RETURN
    /// the same calorimeter?
    if ( seed.calo() != cell.calo() ) { return 0; } ///< RETURN
    /// the same area ?
    if ( seed.area() != cell.area() ) { return treatDifferentAreas( seed, cell ); } ///< RETURN
    /// x ?
    if ( cell.col() > seed.col() + 1 ) { return 0; } ///< RETURN
    if ( cell.col() + 1 < seed.col() ) { return 0; } ///< RETURN
    /// y ?
    if ( cell.row() > seed.row() + 1 ) { return 0; } ///< RETURN
    if ( cell.row() + 1 < seed.row() ) { return 0; } ///< RETURN
    ///
    return 1;
  };

protected:
  /** if cells belong to different areas
   *  one need to treat them in a different way
   *  @exception CaloException if detector information is not available
   *  @param  seed  the seed cell
   *  @param  cell  the cell to be tested
   *  @return fraction of area of the second cell which fits inside
   *          3x3 matrix aroud the seed cell
   */
  double treatDifferentAreas( const LHCb::CaloCellID& seed, const LHCb::CaloCellID& cell ) const;
};

// ============================================================================
// The End
// ============================================================================
#endif // CALOFUTUREUTILS_CELLMATRIX3X3_H

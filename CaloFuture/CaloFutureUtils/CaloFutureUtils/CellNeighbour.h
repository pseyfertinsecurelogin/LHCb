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
#ifndef CALOFUTUREUTILS_CELLNEIGHBOUR_H
#define CALOFUTUREUTILS_CELLNEIGHBOUR_H 1
// Include files
// CaloFutureUtils
#include "CaloFutureUtils/CellMatrix.h"

/** @class CellNeighbour CellNeighbour.h CaloFutureUtils/CellNeighbour.h
 *
 *  The simple utility class to determine if 2 cells are neignbours
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   22/11/2001
 */

class CellNeighbour final : public CellMatrix {
public:
  /** the standard constructor
   *  @param Det pointer to calorimeter detector
   */
  using CellMatrix::CellMatrix;

  /** the major method. It returns "true" if the cell
   *  is a neighbor for seed cell
   *  @exception CaloException if detector information is not available
   *  @param seed  the seed   cell
   *  @param cell  the cell to be tested
   *  @return fraction of area of the second cell which fits inside
   *          "neighbour" aroud the seed cell, equivalently 0 or 1
   */
  double operator()( const LHCb::CaloCellID& seed, const LHCb::CaloCellID& cell ) const;
};

// ============================================================================
#endif // CALOFUTUREUTILS_CELLNEIGHBOUR_H

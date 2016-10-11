#ifndef CALOUTILS_CELLNEIGHBOUR_H 
#define CALOUTILS_CELLNEIGHBOUR_H 1
// Include files
// CaloUtils
#include "CaloUtils/CellMatrix.h"

/** @class CellNeighbour CellNeighbour.h CaloUtils/CellNeighbour.h
 *  
 *  The simple utility class to determine if 2 cells are neignbours 
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   22/11/2001
 */

class CellNeighbour final : public CellMatrix
{
public:

  /** the standard constructor
   *  @param Det pointer to calorimeter detector 
   */
  CellNeighbour( const DeCalorimeter* Det = 0 );
    
  /** the major method. It returns "true" if the cell 
   *  is a neighbor for seed cell  
   *  @exception CaloException if detector information is not available
   *  @param seed  the seed   cell
   *  @param cell  the cell to be tested
   *  @return fraction of area of the second cell which fits inside 
   *          "neighbour" aroud the seed cell, equivalently 0 or 1 
   */
  double operator() 
    ( const LHCb::CaloCellID& seed , 
      const LHCb::CaloCellID& cell ) const ;
};

// ============================================================================
#endif // CALOUTILS_CELLNEIGHBOUR_H
// ============================================================================

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
#ifndef CALOFUTUREUTILS_CELLMATRIX_H 
#define CALOFUTUREUTILS_CELLMATRIX_H 1
// Include files
#include <functional>
#include "GaudiKernel/Point3DTypes.h"
#include "Kernel/CaloCellID.h"
// forward declaration
class DeCalorimeter ;   

/** @class CellMatrix CellMatrix.h CaloFutureUtils/CellMatrix.h
 *  
 *  The base class for implementation of utilities 
 *  for determination of relative posiiton of 2 cells 
 *   e.g. 3x3 matrix, swiss-cross, 2x2 submatrix etc  
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru 
 *  @date   07/11/2001
 */
class CellMatrix
{
  
 public:
  
  /** set detector
   *  @param Det  new pointer to detector element
   */
  inline void setDet( const DeCalorimeter* Det ) 
    { m_det = Det; }
  
  virtual ~CellMatrix() = default;
 protected:
  
  /** Standard constructor
   *  @param Det pointer to calorimeter detector 
   */
  CellMatrix( const DeCalorimeter* Det = nullptr )
    : m_det ( Det )
    {};
  
 protected:
  
  /** accessor to calorimeter 
   *  @return pointer to detector element 
   */
  const DeCalorimeter* det() const { return m_det ;}
  
  /** calculate the intersection area for 2 squares on the plane
   *  @param   center1   center point of the first  square
   *  @param   halfsize1 half   size  of the first  square 
   *  @param   center2   center point of the second square
   *  @param   halfsize2 half   size  of the second square 
   *  @return the intersection area
   */ 
  inline double area ( const Gaudi::XYZPoint& center1   , 
                       const double           halfsize1 , 
                       const Gaudi::XYZPoint& center2   , 
                       const double           halfsize2 ) const 
    {
      /// check the sizes 
      if( halfsize1 <= 0 || halfsize2 <= 0 ) { return 0 ; } ///< RETURN 
      const double xSize = 
        std::min( center1.x() + halfsize1 , center2.x() + halfsize2 ) - 
        std::max( center1.x() - halfsize1 , center2.x() - halfsize2 ) ;
      if( xSize <= 0        )  { return 0 ; } ///< RETURN 
      const double ySize = 
        std::min( center1.y() + halfsize1 , center2.y() + halfsize2 ) - 
        std::max( center1.y() - halfsize1 , center2.y() - halfsize2 ) ;
      if( ySize <= 0        )  { return 0 ; } ///< RETURN 
      ///
      return xSize * ySize ;
    };
  
  /** throw the exception 
   *  @exception CaloException 
   *  @param message exception message 
   */
  void Exception
    ( const std::string& message ) const ;
  
  /** copy constructor is disabled!
   */
  CellMatrix( const CellMatrix& ) = delete;
  
  /** assignement operator is disabled!
   */
  CellMatrix& operator=( const CellMatrix& ) = delete;
  
 private:

  const DeCalorimeter* m_det = nullptr;
  
};

// ============================================================================
#endif // CALOFUTUREUTILS_CELLMATRIX_H

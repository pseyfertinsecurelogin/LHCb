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
#ifndef CALOUTILS_CALONEIGHBOURS_H 
#define CALOUTILS_CALONEIGHBOURS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL 
// ============================================================================
#include <set> 
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/CaloCellID.h"
// ============================================================================
// CaloDet
// ============================================================================
class DeCalorimeter ;
// ============================================================================
/** @file 
 *  Collection of utilitis for find neighbours 
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2009-08-05
 */
// ============================================================================
namespace LHCb
{
  // =========================================================================
  namespace CaloFunctors 
  {
    // ========================================================================
    /** find all neighbours for the given set of cells for the givel level 
     *  @param cells    (UPDATE) list of cells 
     *  @param level    (INPUT)  level
     *  @param detector (INPUT) the detector
     *  @return true if neighbours are added 
     */
    bool neighbours 
    ( LHCb::CaloCellID::Set&      cells    , 
      const unsigned int          level    , 
      const DeCalorimeter*        detector ) ;
    // ========================================================================
    /** find all neighbours for the given set of cells for the givel level 
     *  @param cells    (UPDATE) list of cells 
     *  @param level    (INPUT)  level
     *  @param detector (INPUT)  the detector
     *  @return true if neighbours are added 
     */
    bool neighbours 
    ( LHCb::CaloCellID::Vector&   cells    , 
      const unsigned int          level    , 
      const DeCalorimeter*        detector ) ;
    // ========================================================================
  } //                                      end of namespace LHCb::CaloFunctors 
  // ==========================================================================
} //                                                      end of namespace LHCb 
// ============================================================================
// The END 
// ============================================================================
#endif // CALOUTILS_CALONEIGHBOURS_H

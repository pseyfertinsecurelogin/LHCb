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
#ifndef CALOFUTUREUTILS_CALOFUTUREUTILS_H 
#define CALOFUTUREUTILS_CALOFUTUREUTILS_H 1
// ============================================================================
// Include files
// ============================================================================
// Event 
// ============================================================================
#include "Event/CaloDigit.h"
// ============================================================================
namespace LHCb
{
  // ==========================================================================
  /** @namespace LHCb::CaloFutureHelpers
   *  Helper namespace for dcoration of CaloFuture-objects 
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-11-28
   */  
  namespace CaloFutureHelpers
  {
    // ========================================================================
    /// vector -> set converter 
    LHCb::CaloCellID::Set toSet ( const LHCb::CaloCellID::Vector&    inp ) ;            
    /// vector -> set converter 
    LHCb::CaloDigit::Set  toSet ( const LHCb::CaloDigit::Vector&     inp ) ;
    /// set -> vector converter 
    LHCb::CaloCellID::Vector toVector ( const LHCb::CaloCellID::Set& inp ) ;
    /// set -> vector converter 
    LHCb::CaloDigit::Vector  toVector ( const LHCb::CaloDigit::Set&  inp ) ;
    // ========================================================================
    /// vector -> set converter 
    void toSet    ( const LHCb::CaloCellID::Vector&       inp , 
                    LHCb::CaloCellID::Set&                out ) ;
    /// vector -> set converter 
    void toSet    ( const LHCb::CaloDigit::Vector&        inp , 
                    LHCb::CaloDigit::Set&                 out ) ;
    /// set -> vector converter 
    void toVector ( const LHCb::CaloCellID::Set&          inp , 
                    LHCb::CaloCellID::Vector&             out ) ;
    /// set -> vector converter 
    void toVector ( const LHCb::CaloDigit::Set&           inp , 
                    LHCb::CaloDigit::Vector&              out ) ;
    // ========================================================================    
    /** get the cell form the set 
     *  @param cells (INPUT) the input set
     *  @param index (INPUT) the index 
     *  @return the cell
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     *  @date 2009-11-28
     */
    LHCb::CaloCellID       _get_at_ 
    ( const LHCb::CaloCellID::Set& cells ,
      const size_t                 index ) ;
    // ========================================================================
    /** get the digit from the set 
     *  @param cells (INPUT) the input set
     *  @param index (INPUT) the index 
     *  @return the digit
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     *  @date 2009-11-28
     */
    const LHCb::CaloDigit* _get_at_ 
    ( const LHCb::CaloDigit::Set& cells ,
      const size_t                index ) ;
    // ========================================================================
    /// insert the object to set 
    size_t    _insert_ 
    ( LHCb::CaloCellID::Set&  cells ,
      const LHCb::CaloCellID& cell  ) ;
    // ========================================================================
    /// insert the object to set 
    size_t    _insert_ 
    ( LHCb::CaloDigit::Set&  cells ,
      const LHCb::CaloDigit* digit ) ;
    // ========================================================================
  } //                                         end of namespace LHCb::CaloFutureUtils 
  // ==========================================================================
} //                                                      end of namespace LHCb
// ============================================================================
// The END 
// ============================================================================
#endif // CALOFUTUREUTILS_CALOFUTUREUTILS_H
// ============================================================================

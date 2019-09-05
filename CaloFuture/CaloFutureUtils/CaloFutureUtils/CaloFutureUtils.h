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
namespace LHCb {
  // ==========================================================================
  /** @namespace LHCb::CaloFutureHelpers
   *  Helper namespace for dcoration of CaloFuture-objects
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-11-28
   */
  namespace CaloFutureHelpers {
    // ========================================================================
    /// vector -> set converter
    CaloCellID::Set toSet( const CaloCellID::Vector& inp );
    /// vector -> set converter
    CaloDigit::Set toSet( const CaloDigit::Vector& inp );
    /// set -> vector converter
    CaloCellID::Vector toVector( const CaloCellID::Set& inp );
    /// set -> vector converter
    CaloDigit::Vector toVector( const CaloDigit::Set& inp );
    // ========================================================================
    /// vector -> set converter
    void toSet( const CaloCellID::Vector& inp, CaloCellID::Set& out );
    /// vector -> set converter
    void toSet( const CaloDigit::Vector& inp, CaloDigit::Set& out );
    /// set -> vector converter
    void toVector( const CaloCellID::Set& inp, CaloCellID::Vector& out );
    /// set -> vector converter
    void toVector( const CaloDigit::Set& inp, CaloDigit::Vector& out );
    // ========================================================================
    /** get the cell form the set
     *  @param cells (INPUT) the input set
     *  @param index (INPUT) the index
     *  @return the cell
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     *  @date 2009-11-28
     */
    CaloCellID _get_at_( const CaloCellID::Set& cells, const size_t index );
    // ========================================================================
    /** get the digit from the set
     *  @param cells (INPUT) the input set
     *  @param index (INPUT) the index
     *  @return the digit
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     *  @date 2009-11-28
     */
    const CaloDigit* _get_at_( const CaloDigit::Set& cells, const size_t index );
    // ========================================================================
    /// insert the object to set
    size_t _insert_( CaloCellID::Set& cells, const CaloCellID& cell );
    // ========================================================================
    /// insert the object to set
    size_t _insert_( CaloDigit::Set& cells, const CaloDigit* digit );
    // ========================================================================
  } // namespace CaloFutureHelpers
  // ==========================================================================
} //                                                      end of namespace LHCb
// ============================================================================
// The END
// ============================================================================
#endif // CALOFUTUREUTILS_CALOFUTUREUTILS_H

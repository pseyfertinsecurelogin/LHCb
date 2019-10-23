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
#ifndef LOKI_FIELD_H
#define LOKI_FIELD_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ConstReference.h"
#include "LoKi/Reference.h"
// ============================================================================
/** @file LoKi/Field.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
namespace LoKi {
  // ==========================================================================
  /** @struct Field
   *  Helper structure to define a proper data member type
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-06-08
   */
  template <class TYPE>
  struct Field {
    // ========================================================================
    /// in general case it is a "reference"
    using Type = LoKi::Reference<TYPE>; // in general case it is a "reference"
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<const TYPE> {
    // ========================================================================
    /// for const-type it is a const-reference
    using Type = LoKi::ConstReference<TYPE>; //                const-reference
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<TYPE*> {
    // ========================================================================
    /// in case of pointers it is a bare pointer
    using Type = TYPE*; // in case of pointers it is a bare pointer
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<TYPE* const> {
    // ========================================================================
    /// skip the constness of pointer
    using Type = typename LoKi::Field<TYPE*>::Type;
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<TYPE&> {
    // ========================================================================
    /// skip the reference
    using Type = typename Field<TYPE>::Type; // skip the reference
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE>
  struct Field<const TYPE&> {
    // ========================================================================
    /// skip the reference
    using Type = typename Field<const TYPE>::Type; // skip the reference
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_FIELD_H

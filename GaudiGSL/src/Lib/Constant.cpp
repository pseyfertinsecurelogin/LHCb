/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
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
// local
#include "GaudiMath/Constant.h"

/** @file
 *  Implementation file for class GSL_Constant
 *
 *  @date  2003-08-27
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */

namespace Genfun {
  namespace GaudiMathImplementation {
    // ========================================================================
    /// from CLHGEP/GenericFunctions
    // ========================================================================
    FUNCTION_OBJECT_IMP( Constant )
    // ========================================================================

    // ========================================================================
    /** Standard constructor
     *  @param value of the function
     *  @param dim   dimensionality
     */
    // ========================================================================
    Constant::Constant( const double value, const size_t dim ) : AbsFunction(), m_value( value ), m_DIM( dim ) {}
    // ========================================================================

    // ========================================================================
    /// Derivatives
    // ========================================================================
    Derivative Constant::partial( unsigned int /* index */ ) const {
      const AbsFunction& aux = Constant( 0.0, m_DIM );
      return FunctionNoop( &aux );
    }
    // ========================================================================

  } // end of namespace GaudiMathImplementation

} // namespace Genfun

// ============================================================================
// The END
// ============================================================================

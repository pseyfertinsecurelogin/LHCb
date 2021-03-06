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
// ============================================================================
// STD & STL
// ============================================================================
#include <iostream>
// ============================================================================
// AIDA
// ============================================================================
#include "AIDA/IFunction.h"
// ============================================================================
// local
// ============================================================================
#include "GaudiMath/Adapters.h"
// ============================================================================

/** @file
 *  @author Vanya BELYAEV
 *  @date 2004-03-06
 */

namespace GaudiMath {

  AIDAFunction adapter( const AIDA::IFunction& function ) { return AIDAFunction( function ); }

  Function2D adapter( Function2D::Function function ) { return Function2D( function ); }

  Function3D adapter( Function3D::Function function ) { return Function3D( function ); }

  SimpleFunction adapter( SimpleFunction::Function1 function ) { return SimpleFunction( function ); }

  SimpleFunction adapter( const size_t dim, SimpleFunction::Function2 function ) {
    return SimpleFunction( function, dim );
  }

  SimpleFunction adapter( const size_t dim, SimpleFunction::Function3 function ) {
    return SimpleFunction( function, dim );
  }

  GSLFunctionWithMode adapter( GSLFunctionWithMode::Function function, const gsl_mode_t& mode ) {
    return GSLFunctionWithMode( function, mode );
  }

  GSLFunctionWithError adapter( GSLFunctionWithError::Function function ) { return GSLFunctionWithError( function ); }

  GSLFunctionWithModeAndError adapter( GSLFunctionWithModeAndError::Function function, const gsl_mode_t& mode ) {
    return GSLFunctionWithModeAndError( function, mode );
  }

} // end of namespace GaudiMath
// ============================================================================
// The END
// ============================================================================

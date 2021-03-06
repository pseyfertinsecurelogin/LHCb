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
// CLHEP
// ============================================================================
#include "CLHEP/GenericFunctions/AbsFunction.hh"
// Handle CLHEP 2.0.x move to CLHEP namespace
namespace CLHEP {}
using namespace CLHEP;
// ============================================================================
// local
// ============================================================================
#include "Helpers.h"
// ============================================================================

namespace Genfun {
  namespace GaudiMathImplementation {

    // ========================================================================
    GSL_Helper::GSL_Helper( const Genfun::AbsFunction& function, Genfun::Argument& argument, const size_t index )
        : m_function( function ), m_argument( argument ), m_index( index ) {}
    // ========================================================================

    // ========================================================================
    double GSL_Adaptor( double x, void* params ) {
      // get parameters
      GSL_Helper* aux = (GSL_Helper*)params;
      // decode parameters
      const Genfun::AbsFunction& function = aux->m_function;
      const size_t               index    = aux->m_index;
      Genfun::Argument&          argument = aux->m_argument;
      // save the current value of argument
      const double x_prev = argument[index];
      // modify the argument
      argument[index] = x;
      // evaluate the function
      const double fun = function( argument );
      // restore the argument
      argument[index] = x_prev;
      return fun;
    }
    // ========================================================================

  } // end of namespace GaudiMathImplementation
} // end of namespace Genfun

// ============================================================================
// The END
// ============================================================================

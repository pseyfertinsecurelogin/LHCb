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
#include <cmath>
#include <cstdio>
// ============================================================================
// GAUDI
// ============================================================================
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiMath/GaudiMath.h"
// ============================================================================
// CLHEP
// ============================================================================
#include "CLHEP/GenericFunctions/Cos.hh"
#include "CLHEP/GenericFunctions/Sin.hh"
// ============================================================================
// Handle CLHEP 2.0.x move to CLHEP namespace
namespace CLHEP {}
using namespace CLHEP;

// ============================================================================
/** @file
 *
 *  Test file for the class NumericalIndefiniteIntregral
 *
 *  @date 2003-08-31
 *  @author Vanya  BELYAEV Ivan.Belyaev@itep.ru
 */
// ============================================================================

int main() {

  std::cout << " Test for numerical differentiation of Genfun::Cos(x) " << std::endl;

  const GaudiMath::IndIntegral& mysin = GaudiMath::IndIntegral( Genfun::Cos(), 0, 0.0 );

  for ( double x = -90 * Gaudi::Units::degree; x <= 180 * Gaudi::Units::degree; x += 10 * Gaudi::Units::degree ) {
    double value = mysin( x );
    double error = mysin.error();
    printf( "x=%8.3f deg; I(Cos)=%+.19f; ActErr=%+.19f; EstErr=%+.19f;\n", x / Gaudi::Units::degree, value,
            value - sin( x ), error );
  }
}

// ============================================================================
// The END
// ============================================================================

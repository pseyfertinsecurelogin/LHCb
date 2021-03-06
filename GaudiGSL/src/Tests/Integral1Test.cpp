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
// GaudiMath
// ============================================================================
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiMath/GaudiMath.h"
#include "GaudiMath/Integral.h"
// ============================================================================
// CLHEP
// ============================================================================
#include "CLHEP/GenericFunctions/Cos.hh"
#include "CLHEP/GenericFunctions/Gaussian.hh"
#include "CLHEP/GenericFunctions/Sin.hh"
// ============================================================================
// Handle CLHEP 2.0.x move to CLHEP namespace
namespace CLHEP {}
using namespace CLHEP;

// ============================================================================
/** @file
 *
 *  Test file for the class NumericalDefiniteIntregral  and Integral
 *
 *  @date 2003-08-31
 *  @author Vanya  BELYAEV Ivan.Belyaev@itep.ru
 */
// ============================================================================

int main() {

  std::cout << " Test for numerical integration of Genfun::Sin(x) " << std::endl;

  const GaudiMath::Function& mysin = Genfun::Sin();

  for ( double x = 0 * Gaudi::Units::degree; x <= 360 * Gaudi::Units::degree; x += 15 * Gaudi::Units::degree ) {
    double value = GaudiMath::Integral( mysin, 0., x );
    printf( "x=%8.3f deg; I(Sin)=%+.19f; ActErr=%+.19f;\n", x / Gaudi::Units::degree, value, value - ( 1 - cos( x ) ) );
  }

  const GaudiMath::Function& myfun = Genfun::Gaussian();

  std::cout << " integral from gauss1 =  " << GaudiMath::Integral( myfun, 0.0, GaudiMath::Integration::Infinity )
            << "\n integral from gauss2 =  " << GaudiMath::Integral( myfun, GaudiMath::Integration::Infinity, 0.0 )
            << "\n integral from gauss3 =  "
            << GaudiMath::Integral( myfun, GaudiMath::Integration::Infinity, GaudiMath::Integration::Infinity )
            << "\n integral from gauss4 =  " << GaudiMath::Integral( myfun, GaudiMath::Integration::Infinity, 1.0 )
            << "\n integral from gauss5 =  " << GaudiMath::Integral( myfun, GaudiMath::Integration::Infinity, 4.0 )
            << "\n integral from gauss6 =  " << GaudiMath::Integral( myfun, GaudiMath::Integration::Infinity, 9.0 )
            << "\n integral from gauss7 =  " << GaudiMath::Integral( myfun, GaudiMath::Integration::Infinity, 16.0 )
            << "\n integral from gauss8 =  " << GaudiMath::Integral( myfun, GaudiMath::Integration::Infinity, 25.0 )
            << "\n";
}

// ============================================================================
// The END
// ============================================================================

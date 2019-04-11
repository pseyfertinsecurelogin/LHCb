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
// Include file
// ============================================================================
// STD & STL
// ============================================================================
#include <cstdio>
#include <iostream>
// ============================================================================
// GaudiGSL/GaudiMath
// ============================================================================
#include "GaudiMath/GaudiMath.h"
// ============================================================================
// GSL
// ============================================================================
#include "gsl/gsl_sf_bessel.h"
// ============================================================================

/** @file
 *  test file for GSLFunAdapters
 *  @date 2004-03-06
 *  @authro Vanya BELYAEV Ivan.Belyaev@itep.ru
 */

int main() {

  /// regular bessel function I0 with simplest adaptor
  const GaudiMath::Function& I0 = GaudiMath::adapter( gsl_sf_bessel_I0 );
  /// regular bessel function I1 with 'result' frm of adaptor
  const GaudiMath::Function& I1 = GaudiMath::adapter( gsl_sf_bessel_I1_e );

  for ( int i = 0; i < 20; ++i ) {
    const double x  = double( i ) * 1 / 20.0;
    const double x0 = I0( x );
    const double x1 = I1( x );
    printf( " x = %+8.4f I0=%15.8f I1=%15.8f \n", x, x0, x1 );
  };
}

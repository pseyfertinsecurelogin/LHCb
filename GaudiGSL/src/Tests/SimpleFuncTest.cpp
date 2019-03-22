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
#include "GaudiMath/GaudiMath.h"
// ============================================================================

// ============================================================================
/** @file
 *
 *  Test file for the class SimpleFunction
 *
 *  @date 2003-08-31
 *  @author Vanya  BELYAEV Ivan.Belyaev@itep.ru
 */
// ============================================================================

int main() {

  const GaudiMath::Function& mysin = GaudiMath::SimpleFun( sin );

  for ( double x = 0; x < 10; x += 0.25 ) {
    printf( "x=%8.5f mysin=%+.10f sin=%+.10f sin-mysin=%+.19f \n", x, mysin( x ), sin( x ), sin( x ) - mysin( x ) );
  }
}

// ============================================================================
// The END
// ============================================================================

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
#include "GaudiMath/Adapters.h"
#include "GaudiMath/GaudiMath.h"
// ============================================================================

// ============================================================================
/** @file
 *
 *  Test file for the class Adapter3DoubleFunction
 *
 *  @date 2003-09-03
 *  @author Kirill Miklyaev kirillm@iris1.itep.ru
 */
// ============================================================================

double sum( double x, double y, double z ) { return 3 * x * x - 5 * y * y + 3 * z * z; }

int main() {
  const GaudiMath::Function& mysum = GaudiMath::adapter( sum );
  Genfun::Argument           arg( 3 );

  std::string format( "x=%6.5f y=%6.5f z=%6.5f mysum=%+.10f" );
  format += " sum=%+.10f sum-mysum=%+.10f \n";

  for ( double x = 0; x < 3; ++x ) {
    for ( double y = 0; y < 3; ++y ) {
      for ( double z = 0; z < 3; ++z ) {
        arg[0] = x;
        arg[1] = y;
        arg[2] = z;
        printf( format.c_str(), x, y, z, mysum( arg ), sum( x, y, z ), sum( x, y, z ) - mysum( arg ) );
      }
    }
  }

  exit( 0 );
}

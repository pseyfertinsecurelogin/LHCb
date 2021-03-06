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
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/Chi2Solution.h"
// ============================================================================
int main() {
  //
  typedef Gaudi::Math::Chi2Solution<4, 2> SOLUTION;
  //
  SOLUTION::DATA data;
  data[0] = 110;
  data[1] = 90;
  data[2] = 105;
  data[3] = 95;
  //
  SOLUTION::COV2 cov2;
  for ( unsigned short i = 0; i < 4; ++i ) { cov2( i, i ) = data[i]; }
  //
  // matrix of constraints
  SOLUTION::CMTRX1 D;
  D( 0, 0 ) = 1;
  D( 0, 1 ) = -1;
  D( 1, 2 ) = 1;
  D( 1, 3 ) = -1;
  //
  std::cout << " input  solution     " << data << std::endl;
  std::cout << " input  covariance \n" << cov2 << std::endl;
  std::cout << " constrain matrix  \n" << D << std::endl;
  //
  double     chi2 = -100;
  StatusCode sc   = SOLUTION::solve( data, cov2, D, SOLUTION::COFF(), chi2 );

  std::cout << " StatusCode " << sc << std::endl;
  std::cout << " output solution     " << data << std::endl;
  std::cout << " output covariance \n" << cov2 << std::endl;
  std::cout << " Chi2 " << chi2 << std::endl;

  exit( 0 );
}
// ============================================================================

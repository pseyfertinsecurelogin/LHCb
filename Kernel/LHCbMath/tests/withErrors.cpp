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
//  STD & STL
// ============================================================================
#include <iostream>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
// ============================================================================
// Local
// ============================================================================
#include "LHCbMath/SVectorWithError.h"
// ============================================================================
/** @file
 *  Simple file to test class Gaudi::Math::SVectorWithError
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2009-06-13
 */
// ============================================================================
int main ()
{

  Gaudi::Vector3      v1 ( 0 , 1 , 2 ) ;
  Gaudi::Vector3      v2 ( 1 , 1 , 1 ) ;
  Gaudi::SymMatrix3x3 m1 ;
  m1(0,0) = 1 ;
  m1(1,1) = 4 ;
  m1(2,2) = 9 ;

  Gaudi::Math::SVectorWithError<3,double> d1 ( v1 , m1    ) ;
  Gaudi::Math::SVectorWithError<3,double> d2 ( v1 + 10*v1 ) ;
  Gaudi::Math::SVectorWithError<3,double> d3 ( m1 + 100*m1 ) ;
  Gaudi::Math::SVectorWithError<3,double> d4 ( v1 + v1*2 , m1 + 4*m1 ) ;

  std::cout
    << " v1:     " <<  v1      << std::endl
    << " d1:     " <<  d1      << std::endl
    << " d2:     " <<  d2      << std::endl
    << " d3:     " <<  d3      << std::endl
    << " d4:     " <<  d4      << std::endl
    << " d4*=2:  " << (d4*=2)  << std::endl
    << " d1-d2:  " << (d1-d2)  << std::endl
    << " d1-v1:  " << (d1-v1)  << std::endl
    << " v1-d1:  " << (v1+d1)  << std::endl
    << " v1+d2:  " << (v1-d2)  << std::endl
    << " c21:    " << Gaudi::Math::chi2(v1,d4)  << std::endl
    << " c22:    " << Gaudi::Math::chi2(d4,v1)  << std::endl
    << " c23:    " << Gaudi::Math::chi2(d1,d4)  << std::endl ;


  m1(1,2) = 1 ;
  d1.setCov2 ( m1 ) ;

  /// get the correlation matrix :
  if ( d1.corr ( m1 ) )
  { std::cout << " correlation matrix : " << m1 << std::endl ; }
  else
  { std::cout << "no valid correlation matrix for " << d1.cov2() << std::endl ; }

}
// ============================================================================
// The END
// ============================================================================

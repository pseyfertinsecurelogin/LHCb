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
#include <iostream>
#include <iomanip>
#include <string>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "LHCbMath/Lomont.h"
// ============================================================================
/** @file
 *  The test file for "Lomont" comparison of doubles
 *  @author Vanya BELYAEV i
 *  @date   2009-05-23
 */
// ============================================================================
void _test ( double a , double diff )
{
  int n = 0 ;
  std::cout << "test with a = " << std::setprecision ( 15 ) << a
            << " \t diff "      << std::setprecision ( 15 ) << diff << std::endl ;
  do
  {
    diff /= 9.0 ;
    ++n  ;
    std::cout << " n=" << n
              << " \t diff=" << std::setprecision ( 15 ) << diff << std::endl ;
  }
  while ( !LHCb::Math::lomont_compare_double ( a , a +diff , 1000 ) ) ;
}
// ============================================================================
int main()
{
  _test (  1.01     , 3.01e-3 ) ;
  _test (  1.01e+10 , 3.01e-3 ) ;
  _test (  1.01e+50 , 3.01e-3 ) ;
  _test (  1.01e-10 , 3.01e-3 ) ;
  _test (  1.01e-50 , 3.01e-3 ) ;
  _test ( -1.01e-10 , 3.01e-3 ) ;
  _test ( -1.01e-50 , 3.01e-3 ) ;
  //
  return 0 ;
}
// ============================================================================
// The END
// ============================================================================

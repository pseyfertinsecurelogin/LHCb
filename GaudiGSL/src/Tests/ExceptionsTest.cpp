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
#include <exception>
#include <iostream>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/SystemOfUnits.h"
// ============================================================================
// GaudiMath
// ============================================================================
#include "GaudiMath/GaudiMath.h"
// ============================================================================
// CLHEP
// ============================================================================
#include "CLHEP/GenericFunctions/Sin.hh"
// ============================================================================
// Handle CLHEP 2.0.x move to CLHEP namespace
namespace CLHEP {}
using namespace CLHEP;

// ============================================================================
/** @file
 *
 *  Test file for the class NumericalDerivative
 *
 *  @date 2003-08-31
 *  @author Vanya  BELYAEV Ivan.Belyaev@itep.ru
 */
// ============================================================================

int main() {

  std::cout << " Test for embedded exceptions ( 2 exception shoudl be catched) " << std::endl;

  try {
    const GaudiMath::Function& mysin = Genfun::Sin();
    // the exception!!! showl be thrown!
    const GaudiMath::Function& prim = GaudiMath::Derivative( mysin, 5 );
    std::cout << "One should never see this line!! " << std::endl;
    mysin + prim;
  } catch ( const std::exception& e ) {
    std::cout << " OK 1) The exception is catched with 'what'='" << e.what() << "'" << std::endl;
  }

  try {
    const GaudiMath::Function& mysin = Genfun::Sin();
    // the exception!!! showl be thrown!
    const GaudiMath::Function& prim = GaudiMath::Derivative( mysin, 10 );
    std::cout << "One should never see this line!! " << std::endl;
    mysin + prim;
  } catch ( const GaudiException& e ) { std::cout << " OK 2) " << e << std::endl; }
}

// ============================================================================
// The END
// ============================================================================

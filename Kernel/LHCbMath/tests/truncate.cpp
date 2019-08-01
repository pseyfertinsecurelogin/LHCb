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

// STD & STL
#include <array>
#include <iostream>

// LHCbMath
#include "LHCbMath/Truncate.h"

template <typename TYPE>
inline void print( const TYPE x, const TYPE x_abs, const TYPE x_rel ) {
  std::cout << std::setprecision( 8 )                        //
            << std::fixed << std::setw( 16 ) << x << " "     //
            << std::fixed << std::setw( 16 ) << x_abs << " " //
            << std::fixed << std::setw( 16 ) << x_rel << "    | ";
  std::cout << std::hexfloat << std::setw( 24 ) << x << "   "     //
            << std::hexfloat << std::setw( 24 ) << x_abs << "   " //
            << std::hexfloat << std::setw( 24 ) << x_rel << std::endl;
}

template <std::size_t PREC_ABS, std::size_t PREC_REL>
void test() {
  using namespace LHCb::Math;

  std::cout << "test float PREC(ABS,REL) = (" << PREC_ABS << "," << PREC_REL << ")" << std::endl;
  for ( float x = 0.00876543; x < 1; x += 0.01 ) {
    // test scalar
    const auto x_abs = truncate<PrecisionMode::Absolute, PREC_ABS>( x );
    const auto x_rel = truncate<PrecisionMode::Relative, PREC_REL>( x );
    print( x, x_abs, x_rel );
    // test SIMD
    x += 0.01;
    const auto y     = LHCb::SIMD::FPF( x );
    const auto y_abs = truncate<PrecisionMode::Absolute, PREC_ABS>( y );
    const auto y_rel = truncate<PrecisionMode::Relative, PREC_REL>( y );
    print( y[0], y_abs[0], y_rel[0] );
  }

  std::cout << "test double PREC(ABS,REL) = (" << PREC_ABS << "," << PREC_REL << ")" << std::endl;
  for ( double x = 0.00876543; x < 1; x += 0.01 ) {
    // test scalar
    const auto x_abs = truncate<PrecisionMode::Absolute, PREC_ABS>( x );
    const auto x_rel = truncate<PrecisionMode::Relative, PREC_REL>( x );
    print( x, x_abs, x_rel );
    // test SIMD - Not yet supported by Vc
    // x += 0.01;
    // const auto y     = LHCb::SIMD::FPD( x );
    // const auto y_abs = truncate<PrecisionMode::Absolute, PREC_ABS>( y );
    // const auto y_rel = truncate<PrecisionMode::Relative, PREC_REL>( y );
    // print( y[0], y_abs[0], y_rel[0] );
  }
}

int main() {
  // test a number of different precisions
  test<1, 1>();
  test<2, 2>();
  test<3, 3>();
  test<4, 6>();
  test<4, 16>();
  // return
  exit( 0 );
}

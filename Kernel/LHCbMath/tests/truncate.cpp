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
#include <iostream>

// LHCbMath
#include "LHCbMath/Truncate.h"

template <typename TYPE>
inline void print( const TYPE x, const TYPE x_abs, const TYPE x_rel ) {
  std::cout << std::defaultfloat << std::setw( 10 ) << x << ' '        //
            << std::defaultfloat << std::setw( 10 ) << x_abs << ' '    //
            << std::defaultfloat << std::setw( 10 ) << x_rel << "    " //
            << std::hexfloat << std::setw( 18 ) << x << ' '            //
            << std::hexfloat << std::setw( 18 ) << x_abs << ' '        //
            << std::hexfloat << std::setw( 18 ) << x_rel << std::endl;
}

int main() {

  constexpr std::size_t PREC = 1u;

  using namespace LHCb::Math;

  for ( float x = 0; x < 1; x += 0.01 ) {
    // test scalar
    const auto x_abs = truncate<PrecisionMode::Absolute, PREC>( x );
    const auto x_rel = truncate<PrecisionMode::Relative, PREC>( x );
    print( x, x_abs, x_rel );
    // test SIMD
    x += 0.01;
    const auto y     = LHCb::SIMD::FPF( x );
    const auto y_abs = truncate<PrecisionMode::Absolute, PREC>( y );
    const auto y_rel = truncate<PrecisionMode::Relative, PREC>( y );
    print( y[0], y_abs[0], y_rel[0] );
  }

  exit( 0 );
}

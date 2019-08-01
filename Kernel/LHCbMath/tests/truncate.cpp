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

int main() {

  for ( float x = 0; x < 1; x += 0.01 ) {

    const auto x_abs = LHCb::Math::truncate<LHCb::Math::PrecisionMode::Absolute, 1>( x );
    const auto x_rel = LHCb::Math::truncate<LHCb::Math::PrecisionMode::Relative, 1>( x );

    std::cout << std::defaultfloat << std::setw( 10 ) << x << ' '        //
              << std::defaultfloat << std::setw( 10 ) << x_abs << ' '    //
              << std::defaultfloat << std::setw( 10 ) << x_rel << "    " //
              << std::hexfloat << std::setw( 18 ) << x << ' '            //
              << std::hexfloat << std::setw( 18 ) << x_abs << ' '        //
              << std::hexfloat << std::setw( 18 ) << x_rel << std::endl;
  }

  exit( 0 );
}

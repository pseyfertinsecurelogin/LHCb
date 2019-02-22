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
#include <iostream>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToStream.h"
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/Bernstein.h"
#include "LHCbMath/Polynomials.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/format.hpp"
// ============================================================================
/** @file
 *  Set of simple tests for various "polynomial" functions
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2016-12-14
 */
// ============================================================================
int main() {
  const double xmin = -1;
  const double xmax = 4;
  auto         xt   = [xmin, xmax]( double t ) { return 0.5 * ( ( xmax - xmin ) * t + xmax + xmin ); };

  std::cout << "compare Polynomial -> Chebyshev : transformation vs approximation" << std::endl;

  // regular polymonial
  Gaudi::Math::Polynomial p( 3, xmin, xmax );
  p.setPar( 0, 1 );
  p.setPar( 1, 10 );
  p.setPar( 2, -10 );
  p.setPar( 3, 5 );
  //
  std::cout << "Polynomial   (" << p.degree() << "," << p.xmin() << "," << p.xmax() << ") ";
  Gaudi::Utils::toStream( p.pars(), std::cout ) << std::endl;
  //
  // convert to chebyshev form
  Gaudi::Math::ChebyshevSum c( p );
  std::cout << "ChebyshevSum (" << c.degree() << "," << c.xmin() << "," << c.xmax() << ") ";
  Gaudi::Utils::toStream( c.pars(), std::cout ) << std::endl;
  //
  // make a chebyshev approximation  (template)
  Gaudi::Math::ChebyshevSum cc = Gaudi::Math::chebyshev_sum<10>( p, xmin, xmax );
  std::cout << "ChebyshevSum (" << cc.degree() << "," << cc.xmin() << "," << cc.xmax() << ") ";
  Gaudi::Utils::toStream( cc.pars(), std::cout ) << std::endl;
  //
  //
  const unsigned short N = 20;
  boost::format        format( " %|#6.3f| %|#12.4g| %|#12.4g| %|#12.4g|" );
  for ( unsigned int i = 0; i < N; ++i ) {
    const double  ti   = std::cos( M_PI / N * ( i + 0.5 ) );
    const double  xi   = xt( ti );
    boost::format line = format;
    std::cout << line % xi % p( xi ) % ( c( xi ) - p( xi ) ) % ( cc( xi ) - p( xi ) ) << std::endl;
  }
  //
  std::cout << "Chebyshev approximations and Gauss-Lobatto interpolation for sin and abs" << std::endl;

  // make a chebyshev approximation
  auto fun1 = []( double x ) { return std::sin( x ); };
  auto fun2 = []( double x ) { return std::abs( x ); };
  //  use     templated methods:
  Gaudi::Math::ChebyshevSum sf1 = Gaudi::Math::chebyshev_sum<16>( fun1 );
  Gaudi::Math::ChebyshevSum sf2 = Gaudi::Math::chebyshev_sum<16>( fun2 );
  //  use non-templated methods:
  Gaudi::Math::ChebyshevSum sf3 = Gaudi::Math::chebyshev_sum( fun1, 16 );
  Gaudi::Math::ChebyshevSum sf4 = Gaudi::Math::chebyshev_sum( fun2, 16 );
  //  use intrpolation with Gauss-Lobatto grid
  Gaudi::Math::Bernstein sf5 = Gaudi::Math::Interpolation::lobatto<16>( fun1, -1, 1 );
  Gaudi::Math::Bernstein sf6 = Gaudi::Math::Interpolation::lobatto( fun2, 16, -1, 1 );
  //
  boost::format format2(
      "x: %|#6.3f| sin:%|#+7.5f| %|#+14.5g| %|#+14.5g| %|#+14.5g|  abs:%|#+7.5f| %|#+14.5g| %|#+14.5g| %|#+14.5g|" );
  for ( unsigned int i = 0; i < N; ++i ) {
    const double  ti   = std::cos( M_PI / N * ( i + 0.5 ) );
    boost::format line = format2;
    std::cout << line % ti % fun1( ti ) % ( sf1( ti ) - fun1( ti ) ) % ( sf3( ti ) - fun1( ti ) ) %
                     ( sf5( ti ) - fun1( ti ) ) % fun2( ti ) % ( sf2( ti ) - fun2( ti ) ) % ( sf4( ti ) - fun2( ti ) ) %
                     ( sf6( ti ) - fun2( ti ) )
              << std::endl;
  }
  //

  exit( 0 );
}
// ============================================================================
// The END
// ============================================================================

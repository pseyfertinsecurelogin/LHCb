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
// GaudiKernel
// ============================================================================
#include "GaudiKernel/GaudiException.h"
// ============================================================================
// GaudiMath
// ============================================================================
#include "GaudiMath/Constant.h"
#include "GaudiMath/Integral.h"
#include "GaudiMath/NumericalDefiniteIntegral.h"
// ============================================================================

#ifdef __ICC
// disable icc remark #1572: floating-point equality and inequality comparisons are unreliable
//   The comparison are meant
#  pragma warning( disable : 1572 )
// disable icc remark #2259: non-pointer conversion from "double" to "float" may lose significant bits
#  pragma warning( disable : 2259 )
#endif

namespace GaudiMath {

  double Integral( const Genfun::AbsFunction& function, const double a, const double b,
                   const GaudiMath::Integration::Type type, const GaudiMath::Integration::KronrodRule rule,
                   const double epsabs, const double epsrel, const size_t size ) {
    if ( 1 != function.dimensionality() ) {
      throw GaudiException( "GaudiMath::Integral: illegal function dimension", "*GaudiMath*", StatusCode::FAILURE );
    }

    const Genfun::AbsFunction& one = Genfun::GaudiMathImplementation::Constant( 1.0, 1 );

    const Genfun::AbsFunction& cross = Genfun::FunctionDirectProduct( &one, &function );

    const Genfun::AbsFunction& result =
        Genfun::GaudiMathImplementation::NumericalDefiniteIntegral( cross, 1, a, b, type, rule, epsabs, epsrel, size );

    return result( Genfun::Argument( 1 ) );
  }

  double Integral( const Genfun::AbsFunction& function, const double a, const double b,
                   const std::vector<double>& points, const double epsabs, const double epsrel, const size_t size ) {

    if ( 1 != function.dimensionality() ) {
      throw GaudiException( "GaudiMath::Integral: illegal function dimension", "*GaudiMath*", StatusCode::FAILURE );
    }

    const Genfun::AbsFunction& one = Genfun::GaudiMathImplementation::Constant( 1.0, 1 );

    const Genfun::AbsFunction& cross = Genfun::FunctionDirectProduct( &one, &function );

    const Genfun::AbsFunction& result =
        Genfun::GaudiMathImplementation::NumericalDefiniteIntegral( cross, 1, a, b, points, epsabs, epsrel, size );

    return result( Genfun::Argument( 1 ) );
  }

  double Integral( const Genfun::AbsFunction& function, const double a, const GaudiMath::Integration::Inf b,
                   const double epsabs, const double epsrel, const size_t size ) {
    if ( 1 != function.dimensionality() ) {
      throw GaudiException( "GaudiMath::Integral: illegal function dimension", "*GaudiMath*", StatusCode::FAILURE );
    }

    const Genfun::AbsFunction& one = Genfun::GaudiMathImplementation::Constant( 1.0, 1 );

    const Genfun::AbsFunction& cross = Genfun::FunctionDirectProduct( &one, &function );

    const Genfun::AbsFunction& result =
        Genfun::GaudiMathImplementation::NumericalDefiniteIntegral( cross, 1, a, b, epsabs, epsrel, size );

    return result( Genfun::Argument( 1 ) );
  }

  double Integral( const Genfun::AbsFunction& function, const GaudiMath::Integration::Inf a, const double b,
                   const double epsabs, const double epsrel, const size_t size ) {
    if ( 1 != function.dimensionality() ) {
      throw GaudiException( "GaudiMath::Integral: illegal function dimension", "*GaudiMath*", StatusCode::FAILURE );
    }

    const Genfun::AbsFunction& one = Genfun::GaudiMathImplementation::Constant( 1.0, 1 );

    const Genfun::AbsFunction& cross = Genfun::FunctionDirectProduct( &one, &function );

    const Genfun::AbsFunction& result =
        Genfun::GaudiMathImplementation::NumericalDefiniteIntegral( cross, 1, a, b, epsabs, epsrel, size );

    return result( Genfun::Argument( 1 ) );
  }

  double Integral( const Genfun::AbsFunction& function, const GaudiMath::Integration::Inf /* a  */,
                   const GaudiMath::Integration::Inf /* b  */, const double epsabs, const double epsrel,
                   const size_t size ) {
    if ( 1 != function.dimensionality() ) {
      throw GaudiException( "GaudiMath::Integral: illegal function dimension", "*GaudiMath*", StatusCode::FAILURE );
    }

    const Genfun::AbsFunction& one = Genfun::GaudiMathImplementation::Constant( 1.0, 1 );

    const Genfun::AbsFunction& cross = Genfun::FunctionDirectProduct( &one, &function );

    // FIXME: (MCl) the static_cast below are needed to avoid warnings and to
    // match the signature in NumericalDefiniteIntegral.h (around line 288).
    const Genfun::AbsFunction& result = Genfun::GaudiMathImplementation::NumericalDefiniteIntegral(
        cross, 1, static_cast<float>( epsabs ), static_cast<float>( epsrel ), size );

    return result( Genfun::Argument( 1 ) );
  }
} // namespace GaudiMath

// ============================================================================
// The END
// ============================================================================

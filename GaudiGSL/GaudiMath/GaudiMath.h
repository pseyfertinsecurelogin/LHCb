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
#ifndef GAUDIMATH_GAUDIMATH_H
#define GAUDIMATH_GAUDIMATH_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiMath
// ============================================================================
#include "GaudiMath/Adapters.h"
#include "GaudiMath/Constant.h"
#include "GaudiMath/NumericalDefiniteIntegral.h"
#include "GaudiMath/NumericalDerivative.h"
#include "GaudiMath/NumericalIndefiniteIntegral.h"
#include "GaudiMath/Splines.h"
// ============================================================================

/** @namespace GaudiMath GaudiMath.h GaudiMath/GaudiMath.h
 *
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2003-08-31
 */
namespace GaudiMath {
  typedef Genfun::AbsFunction Function;

  typedef Genfun::GaudiMathImplementation::Constant                    Constant;
  typedef Genfun::GaudiMathImplementation::NumericalDerivative         Derivative;
  typedef Genfun::GaudiMathImplementation::SimpleFunction              SimpleFun;
  typedef Genfun::GaudiMathImplementation::NumericalIndefiniteIntegral IndIntegral;
  typedef Genfun::GaudiMathImplementation::NumericalDefiniteIntegral   DefIntegral;

  typedef Genfun::GaudiMathImplementation::SplineBase      SimpleSpline;
  typedef Genfun::GaudiMathImplementation::GSLSpline       Spline;
  typedef Genfun::GaudiMathImplementation::GSLSplineDeriv  SplineDeriv;
  typedef Genfun::GaudiMathImplementation::GSLSplineDeriv2 SplineDeriv2;
  typedef Genfun::GaudiMathImplementation::GSLSplineInteg  SplineInteg;

} // namespace GaudiMath

// ============================================================================
// The END
// ============================================================================
#endif // GAUDIMATH_GAUDIMATH_H

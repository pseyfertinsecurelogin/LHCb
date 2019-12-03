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
#ifndef LOKI_KINTYPES_H
#define LOKI_KINTYPES_H 1
// ============================================================================
// Include files
// ============================================================================
// STD&SLT
// ============================================================================
#include <cmath>
#include <complex>
// ============================================================================
// Math Definitions
// ============================================================================
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/Vector4DTypes.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  // the actual type of Lorentz vector
  using LorentzVector = Gaudi::LorentzVector; // the vector in 4D
  // the actual type of Lorentz vector
  using Vector4D = LorentzVector; // the vector in 4D
  // the actual tyep of 3D vector
  using ThreeVector = Gaudi::XYZVector; // the vector in 3D
  /// the actual type of 3D vector
  using Vector3D = ThreeVector; // the vector in 3D
  /// the actual type for  3D point
  using Point3D = Gaudi::XYZPoint; // the point  in 3D
  // ==========================================================================
  using ComplexLorentzVector = ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<std::complex<double>>>;
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_KINTYPES_H

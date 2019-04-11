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
// =============================================================================
#ifndef GAUDIGSL_IEQSOLVER_H
#define GAUDIGSL_IEQSOLVER_H 1
// Include files
// from STL
#include <string>
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// forward declarations
namespace Genfun /// CLHEP
{
  class AbsFunction;
  class Argument;
} // namespace Genfun

/** @class IEqSolver IEqSolver.h GaudiGSL/IEqSolver.h
 *  Interface file for multidimensional root-finding
 *  (solving nonlinear system with n equations in n unknowns)
 *  @author Kirill Miklyaev kirillm@iris1.itep.ru
 *  @date   2003-07-07
 */

struct GAUDI_API IEqSolver : extend_interfaces<IAlgTool> {
  /// InterfaceID
  DeclareInterfaceID( IEqSolver, 2, 0 );

  /// Function (@see CLHEP/GenericFunctions/AbsFunction.hh)
  typedef Genfun::AbsFunction GenFunc;
  /// Vector of the function "GenFunc" which we solver
  typedef std::vector<const GenFunc*> Equations;
  /// Argument of function "GenFunc" (@see CLHEP/GenericFunctions/Argument.hh)
  typedef Genfun::Argument Arg;

  /// Do Multidimensional Root-finding

  /** Solving nonlinear system with N equations in N unknowns
   *  of the function "GenFunc"
   *  @param equations   - vector of functions
   *  @see CLHEP/GenericFunctions/AbsFunction.hh
   *  @param solution    - argument
   *  @see CLHEP/GenericFunctions/Argument.hh
   *  @return StatusCode
   */
  virtual StatusCode solver( const Equations& equations, Arg& solution ) const = 0;
};

#endif // GAUDIGSL_IEQSOLVER_H

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
#ifndef LOKI_CORETYPES_H
#define LOKI_CORETYPES_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/BasicFunctors.h"
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
 *  @date 2006-02-10
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::CoreTypes LoKi/CoreTypes.h
   *  The core namespace for some "core"  types
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-02-10
   */
  namespace CoreTypes {
    // ====================================================================
    /// type of 'cuts' for  'void' (interface)
    using FCuts = LoKi::Predicate_t<void>;
    /// type of 'functions' 'void' (interface)
    using FFunc = LoKi::Function_t<void>;
    /// type of 'cuts' for  'void' (assignable)
    using FCut = LoKi::Assignable_t<FCuts>;
    /// type of 'functions' for LHCb::Particle  (assignable)
    using FFun = LoKi::Assignable_t<FFunc>;
    // ========================================================================
    /// type of 'cuts' for  'double' (interface)
    using XCuts = LoKi::Predicate_t<double>;
    /// type of 'functions' 'double' (interface)
    using XFunc = LoKi::Function_t<double>;
    /// type of 'cuts' for  'double' (assignable)
    using XCut = LoKi::Assignable_t<XCuts>;
    /// type of 'functions' for LHCb::Particle  (assignable)
    using XFun = LoKi::Assignable_t<XFunc>;
    // ========================================================================
    /// maping or filtering : std::vector<double> -> std::vector<double>
    using XMaps = LoKi::Map_t<double>;
    using XMap  = LoKi::Assignable_t<XMaps>;
    /// the same
    using XPipes = LoKi::Pipe_t<double>;
    using XPipe  = LoKi::Assignable_t<XPipes>;
    /// function or element selection: std::vector<double> -> double
    using XFunVals = LoKi::FunVal_t<double>;
    using XFunVal  = LoKi::Assignable_t<XFunVals>;
    /// source : void -> std::vector<double>
    using XSources = LoKi::Source_t<double>;
    using XSource  = LoKi::Assignable_t<XSources>;
    /// source : void -> std::vector<double>
    using FSources = LoKi::Source_t<double>;
    using FSource  = LoKi::Assignable_t<FSources>;
    // ========================================================================
  } // namespace CoreTypes
  // ==========================================================================
  /** @namespace LoKi::Types
   *  The namespace to collect all basic LoKi types for
   *  funtors and predicates
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-02-10
   */
  namespace Types {
    using namespace CoreTypes;
  } // namespace Types
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace LoKi {
  // ==========================================================================
  // the specialized printout
  // ==========================================================================
  template <>
  inline std::ostream& BooleanConstant_t<void>::fillStream( std::ostream& s ) const {
    return s << ( this->m_value ? "FALL" : "FNONE" );
  }
  // ==========================================================================
  template <>
  inline std::ostream& Constant_t<double>::fillStream( std::ostream& s ) const {
    return s << ( this->m_value ? "XALL" : "XNONE" );
  }
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_CORETYPES_H

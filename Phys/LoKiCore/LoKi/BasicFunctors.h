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
#ifndef LOKI_BASICFUNCTORS_H
#define LOKI_BASICFUNCTORS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functor.h"
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
 *  @date 2007-10-31
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @struct Assignable
   *  helper to define the corresponding assignable functors
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-10-31
   */
  template <typename Fun>
  using Assignable_t = LoKi::FunctorFromFunctor<LoKi::details::type1_t<Fun>,
                                                LoKi::details::type2_t<Fun>>;
  template <typename Fun> struct
  // TODO: uncomment deprecation warning...
  // [[deprecated("please use LoKi::Assignable_t<Fun> instead of typename LoKi::Assignable<Fun>::Type")]]
  Assignable { using Type = Assignable_t<Fun>; };

  // ==========================================================================
  /** @struct BasicFunctors
   *
   *  The helper structure to propagate the actual types for the basic functors
   *
   *  The idea comes from Gerhard Raven
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-10-31
   */

  template <typename T> using BooleanConstant_t = LoKi::Constant<T,bool>;
  template <typename T> using Constant_t        = LoKi::Constant<T,double>;
  template <typename T> using Function_t        = LoKi::Functor<T,double>;
  template <typename T> using Predicate_t       = LoKi::Functor<T,bool>;
  template <typename T> using AssignablePredicate_t = Assignable_t<Predicate_t<T>>;
  template <typename T> using Map_t             = LoKi::Functor<std::vector<T>, std::vector<double> >;
  template <typename T> using Pipe_t            = LoKi::Functor<std::vector<T>, std::vector<T>>;
  template <typename T> using FunVal_t          = Function_t<std::vector<T>>;
  template <typename T> using Source_t          = LoKi::Functor<void,std::vector<T>>;
  template <typename T> using CutVal_t          = Predicate_t<std::vector<T>>;

  template <class TYPE> struct BasicFunctors
  {
    using Function = Function_t<TYPE>;        // the (interface) type for "function"
    using Predicate = Predicate_t<TYPE>;      // the (interface) type for "predicate"
    using FunctionFromFunction = Assignable_t<Function>;    // the (assignable) type for "function"
    using PredicateFromPredicate = Assignable_t<Predicate>; // the (assignable) type for "predicate"
    //
    using Constant = Constant_t<TYPE>;
    using BooleanConstant = BooleanConstant_t<TYPE>;
    //
    // ========================================================================
    // for functional programing:
    // ========================================================================
    //
    using Map    = Map_t<TYPE>;      // map/yield:
    using Pipe   = Pipe_t<TYPE>;     // filter:
    using FunVal = FunVal_t<TYPE>;   // reduce
    using Source = Source_t<TYPE>;   // Source
    using CutVal = CutVal_t<TYPE>;   // reduce
  } ;
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_BASICFUNCTORS_H

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
#ifndef LOKI_FUNCOPS_H
#define LOKI_FUNCOPS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Dumper.h"
#include "LoKi/ExtraInfo.h"
#include "LoKi/Filters.h"
#include "LoKi/Funcs.h"
#include "LoKi/Functions.h"
#include "LoKi/Math.h"
#include "LoKi/Monitor.h"
#include "LoKi/Monitoring.h"
#include "LoKi/MoreFunctions.h"
#include "LoKi/Operators.h"
#include "LoKi/Param.h"
#include "LoKi/ParamFunctors.h"
#include "LoKi/Streamers.h"
#include "LoKi/compose.h"
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
namespace LoKi {
  // ==========================================================================
  namespace Dicts {
    // ========================================================================
    /** @class FuncOps__
     *  Wrapper class for basic operations with functions
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-10-07
     */
    template <class TYPE>
    class FuncOps__ {
    protected:
      // ======================================================================
      using Func = Function_t<TYPE>;
      using Cuts = Predicate_t<TYPE>;
      using Fun  = Assignable_t<Func>;
      using Cut  = Assignable_t<Cuts>;
      using PAR  = LoKi::Parameters::Parameter;
      using Para = LoKi::Param;
      // ======================================================================
    private:
      // ======================================================================
      using VOID = LoKi::BasicFunctors<void>::Function;
      using VAL  = LoKi::Value<TYPE, double>;
      // ======================================================================
    public:
      // ======================================================================
      static Cut __lt__( const Func& fun1, const Func& fun2 ) { return fun1 < fun2; }
      static Cut __lt__( const Func& fun1, double fun2 ) { return fun1 < fun2; }
      static Cut __lt__( const Func& fun1, const VOID& fun2 ) { return fun1 < fun2; }
      static Cut __lt__( const Func& fun1, const Para& fun2 ) { return fun1 < PAR( fun2 ); }
      //
      static Cut __le__( const Func& fun1, const Func& fun2 ) { return fun1 <= fun2; }
      static Cut __le__( const Func& fun1, double fun2 ) { return fun1 <= fun2; }
      static Cut __le__( const Func& fun1, const VOID& fun2 ) { return fun1 <= fun2; }
      static Cut __le__( const Func& fun1, const Para& fun2 ) { return fun1 <= PAR( fun2 ); }
      //
      static Cut __gt__( const Func& fun1, const Func& fun2 ) { return fun1 > fun2; }
      static Cut __gt__( const Func& fun1, double fun2 ) { return fun1 > fun2; }
      static Cut __gt__( const Func& fun1, const VOID& fun2 ) { return fun1 > fun2; }
      static Cut __gt__( const Func& fun1, const Para& fun2 ) { return fun1 > PAR( fun2 ); }
      //
      static Cut __ge__( const Func& fun1, const Func& fun2 ) { return fun1 >= fun2; }
      static Cut __ge__( const Func& fun1, double fun2 ) { return fun1 >= fun2; }
      static Cut __ge__( const Func& fun1, const VOID& fun2 ) { return fun1 >= fun2; }
      static Cut __ge__( const Func& fun1, const Para& fun2 ) { return fun1 >= PAR( fun2 ); }
      //
      static Cut __eq__( const Func& fun1, const Func& fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, double fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, int fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, unsigned int fun2 ) { return fun1 == fun2; }
      //
      static Cut __eq__( const Func& fun1, const std::vector<double>& fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, const std::vector<int>& fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, const std::vector<unsigned int>& fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, const VOID& fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, const Para& fun2 ) { return fun1 == PAR( fun2 ); }
      //
      static Cut __ne__( const Func& fun1, const Func& fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, double fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, int fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, unsigned int fun2 ) { return fun1 != fun2; }
      //
      static Cut __ne__( const Func& fun1, const std::vector<double>& fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, const std::vector<int>& fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, const std::vector<unsigned int>& fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, const VOID& fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, const Para& fun2 ) { return fun1 != PAR( fun2 ); }
      //
      // basic math
      //
      static Fun __add__( const Func& fun1, const Func& fun2 ) { return fun1 + fun2; }
      static Fun __sub__( const Func& fun1, const Func& fun2 ) { return fun1 - fun2; }
      static Fun __mul__( const Func& fun1, const Func& fun2 ) { return fun1 * fun2; }
      static Fun __div__( const Func& fun1, const Func& fun2 ) { return fun1 / fun2; }
      //
      static Fun __add__( const Func& fun1, double fun2 ) { return fun1 + fun2; }
      static Fun __sub__( const Func& fun1, double fun2 ) { return fun1 - fun2; }
      static Fun __mul__( const Func& fun1, double fun2 ) { return fun1 * fun2; }
      static Fun __div__( const Func& fun1, double fun2 ) { return fun1 / fun2; }
      //
      static Fun __add__( const Func& fun1, const VOID& fun2 ) { return fun1 + VAL( fun2 ); }
      static Fun __sub__( const Func& fun1, const VOID& fun2 ) { return fun1 - VAL( fun2 ); }
      static Fun __mul__( const Func& fun1, const VOID& fun2 ) { return fun1 * VAL( fun2 ); }
      static Fun __div__( const Func& fun1, const VOID& fun2 ) { return fun1 / VAL( fun2 ); }
      //
      static Fun __add__( const Func& fun1, const Para& fun2 ) { return fun1 + VAL( PAR( fun2 ) ); }
      static Fun __sub__( const Func& fun1, const Para& fun2 ) { return fun1 - VAL( PAR( fun2 ) ); }
      static Fun __mul__( const Func& fun1, const Para& fun2 ) { return fun1 * VAL( PAR( fun2 ) ); }
      static Fun __div__( const Func& fun1, const Para& fun2 ) { return fun1 / VAL( PAR( fun2 ) ); }
      //
      static Fun __pow__( const Func& fun1, const Func& fun2 ) { return LoKi::pow( fun1, fun2 ); }
      static Fun __pow__( const Func& fun1, double fun2 ) { return LoKi::pow( fun1, fun2 ); }
      static Fun __pow__( const Func& fun1, int fun2 ) { return LoKi::pow( fun1, fun2 ); }
      static Fun __pow__( const Func& fun1, const VOID& fun2 ) { return LoKi::pow( fun1, VAL( fun2 ) ); }
      static Fun __pow__( const Func& fun1, const Para& fun2 ) { return LoKi::pow( fun1, VAL( PAR( fun2 ) ) ); }

      //
      // "right" side math.
      //
      static Fun __radd__( const Func& fun1, double fun2 ) { return fun2 + fun1; }
      static Fun __rsub__( const Func& fun1, double fun2 ) { return fun2 - fun1; }
      static Fun __rmul__( const Func& fun1, double fun2 ) { return fun2 * fun1; }
      static Fun __rdiv__( const Func& fun1, double fun2 ) { return fun2 / fun1; }
      static Fun __rpow__( const Func& fun1, double fun2 ) { return LoKi::pow( fun2, fun1 ); }
      static Fun __rpow__( const Func& fun1, int fun2 ) { return LoKi::pow( fun2, fun1 ); }
      //
      static Fun __radd__( const Func& fun1, const Para& fun2 ) { return VAL( PAR( fun2 ) ) + fun1; }
      static Fun __rsub__( const Func& fun1, const Para& fun2 ) { return VAL( PAR( fun2 ) ) - fun1; }
      static Fun __rmul__( const Func& fun1, const Para& fun2 ) { return VAL( PAR( fun2 ) ) * fun1; }
      static Fun __rdiv__( const Func& fun1, const Para& fun2 ) { return VAL( PAR( fun2 ) ) / fun1; }
      static Fun __rpow__( const Func& fun1, const Para& fun2 ) { return LoKi::pow( VAL( PAR( fun2 ) ), fun1 ); }
      // ======================================================================

      // ======================================================================
      // min
      // ======================================================================
      static Fun __min__( const Func& f1, const Func& f2 ) { return LoKi::min( f1, f2 ); }
      // min
      static Fun __min__( const Func& f1, double f2 ) { return LoKi::min( f1, f2 ); }
      // min
      static Fun __min__( const Func& f1, const VOID& f2 ) { return LoKi::min( f1, VAL( f2 ) ); }
      // min
      static Fun __min__( const Func& f1, const Para& f2 ) { return LoKi::min( f1, VAL( PAR( f2 ) ) ); }
      // min
      static Fun __min__( const Func& f1, const Func& f2, const Func& f3 ) { return LoKi::min( f1, f2, f3 ); }
      // min
      static Fun __min__( const Func& f1, const Func& f2, const Func& f3, const Func& f4 ) {
        return LoKi::min( f1, f2, f3, f4 );
      }
      // ======================================================================
      // max
      // ======================================================================
      static Fun __max__( const Func& f1, const Func& f2 ) { return LoKi::max( f1, f2 ); }
      static Fun __max__( const Func& f1, double f2 ) { return LoKi::max( f1, f2 ); }
      static Fun __max__( const Func& f1, const VOID& f2 ) { return LoKi::max( f1, VAL( f2 ) ); }
      // max
      static Fun __max__( const Func& f1, const Para& f2 ) { return LoKi::max( f1, VAL( PAR( f2 ) ) ); }
      // max
      static Fun __max__( const Func& f1, const Func& f2, const Func& f3 ) { return LoKi::max( f1, f2, f3 ); }
      static Fun __max__( const Func& f1, const Func& f2, const Func& f3, const Func& f4 ) {
        return LoKi::max( f1, f2, f3, f4 );
      }
    };
    // ========================================================================
    /// speciailazation for void
    template <>
    class FuncOps__<void> {
    protected:
      // ======================================================================
      typedef LoKi::BasicFunctors<void>::Function               Func;
      typedef LoKi::BasicFunctors<void>::Predicate              Cuts;
      typedef LoKi::BasicFunctors<void>::FunctionFromFunction   Fun;
      typedef LoKi::BasicFunctors<void>::PredicateFromPredicate Cut;
      typedef LoKi::Parameters::Parameter                       PAR;
      typedef LoKi::Param                                       Para;
      // ======================================================================
    public:
      // ======================================================================
      static Cut __lt__( const Func& fun1, const Func& fun2 ) { return fun1 < fun2; }
      static Cut __lt__( const Func& fun1, double fun2 ) { return fun1 < fun2; }
      static Cut __lt__( const Func& fun1, const Para& fun2 ) { return fun1 < PAR( fun2 ); }
      //
      static Cut __le__( const Func& fun1, const Func& fun2 ) { return fun1 <= fun2; }
      static Cut __le__( const Func& fun1, double fun2 ) { return fun1 <= fun2; }
      static Cut __le__( const Func& fun1, const Para& fun2 ) { return fun1 <= PAR( fun2 ); }
      //
      static Cut __gt__( const Func& fun1, const Func& fun2 ) { return fun1 > fun2; }
      static Cut __gt__( const Func& fun1, double fun2 ) { return fun1 > fun2; }
      static Cut __gt__( const Func& fun1, const Para& fun2 ) { return fun1 > PAR( fun2 ); }
      //
      static Cut __ge__( const Func& fun1, const Func& fun2 ) { return fun1 >= fun2; }
      static Cut __ge__( const Func& fun1, double fun2 ) { return fun1 >= fun2; }
      static Cut __ge__( const Func& fun1, const Para& fun2 ) { return fun1 >= PAR( fun2 ); }
      //
      static Cut __eq__( const Func& fun1, const Func& fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, double fun2 ) { return fun1 == fun2; }
      //
      static Cut __eq__( const Func& fun1, int fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, unsigned int fun2 ) { return fun1 == fun2; }
      //
      static Cut __eq__( const Func& fun1, const std::vector<double>& fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, const std::vector<int>& fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, const std::vector<unsigned int>& fun2 ) { return fun1 == fun2; }
      static Cut __eq__( const Func& fun1, const Para& fun2 ) { return fun1 == PAR( fun2 ); }
      //
      static Cut __ne__( const Func& fun1, const Func& fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, double fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, int fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, unsigned int fun2 ) { return fun1 != fun2; }
      //
      static Cut __ne__( const Func& fun1, const std::vector<double>& fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, const std::vector<int>& fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, const std::vector<unsigned int>& fun2 ) { return fun1 != fun2; }
      static Cut __ne__( const Func& fun1, const Para& fun2 ) { return fun1 != PAR( fun2 ); }
      //
      static Fun __add__( const Func& fun1, const Func& fun2 ) { return fun1 + fun2; }
      static Fun __sub__( const Func& fun1, const Func& fun2 ) { return fun1 - fun2; }
      static Fun __mul__( const Func& fun1, const Func& fun2 ) { return fun1 * fun2; }
      static Fun __div__( const Func& fun1, const Func& fun2 ) { return fun1 / fun2; }
      //
      static Fun __add__( const Func& fun1, double fun2 ) { return fun1 + fun2; }
      static Fun __sub__( const Func& fun1, double fun2 ) { return fun1 - fun2; }
      static Fun __mul__( const Func& fun1, double fun2 ) { return fun1 * fun2; }
      static Fun __div__( const Func& fun1, double fun2 ) { return fun1 / fun2; }
      //
      static Fun __add__( const Func& fun1, const Para& fun2 ) { return fun1 + PAR( fun2 ); }
      static Fun __sub__( const Func& fun1, const Para& fun2 ) { return fun1 - PAR( fun2 ); }
      static Fun __mul__( const Func& fun1, const Para& fun2 ) { return fun1 * PAR( fun2 ); }
      static Fun __div__( const Func& fun1, const Para& fun2 ) { return fun1 / PAR( fun2 ); }
      //
      static Fun __pow__( const Func& fun1, const Func& fun2 ) { return LoKi::pow( fun1, fun2 ); }
      static Fun __pow__( const Func& fun1, double fun2 ) { return LoKi::pow( fun1, fun2 ); }
      static Fun __pow__( const Func& fun1, int fun2 ) { return LoKi::pow( fun1, fun2 ); }
      static Fun __pow__( const Func& fun1, const Para& fun2 ) { return LoKi::pow( fun1, PAR( fun2 ) ); }
      //
      // "right" side math.
      //
      static Fun __radd__( const Func& fun1, double fun2 ) { return fun2 + fun1; }
      static Fun __rsub__( const Func& fun1, double fun2 ) { return fun2 - fun1; }
      static Fun __rmul__( const Func& fun1, double fun2 ) { return fun2 * fun1; }
      static Fun __rdiv__( const Func& fun1, double fun2 ) { return fun2 / fun1; }
      static Fun __rpow__( const Func& fun1, double fun2 ) { return LoKi::pow( fun2, fun1 ); }
      static Fun __rpow__( const Func& fun1, int fun2 ) { return LoKi::pow( fun2, fun1 ); }
      //
      static Fun __radd__( const Func& fun1, const Para& fun2 ) { return PAR( fun2 ) + fun1; }
      static Fun __rsub__( const Func& fun1, const Para& fun2 ) { return PAR( fun2 ) - fun1; }
      static Fun __rmul__( const Func& fun1, const Para& fun2 ) { return PAR( fun2 ) * fun1; }
      static Fun __rdiv__( const Func& fun1, const Para& fun2 ) { return PAR( fun2 ) / fun1; }
      static Fun __rpow__( const Func& fun1, const Para& fun2 ) { return LoKi::pow( PAR( fun2 ), fun1 ); }
      // ======================================================================
      // min
      // ======================================================================
      static Fun __min__( const Func& f1, const Func& f2 ) { return LoKi::min( f1, f2 ); }
      // min
      static Fun __min__( const Func& f1, double f2 ) { return LoKi::min( f1, f2 ); }
      // min
      static Fun __min__( const Func& f1, const Para& f2 ) { return LoKi::min( f1, PAR( f2 ) ); }
      // min
      static Fun __min__( const Func& f1, const Func& f2, const Func& f3 ) { return LoKi::min( f1, f2, f3 ); }
      // min
      static Fun __min__( const Func& f1, const Func& f2, const Func& f3, const Func& f4 ) {
        return LoKi::min( f1, f2, f3, f4 );
      }
      // ======================================================================
      // max
      // ======================================================================
      static Fun __max__( const Func& f1, const Func& f2 ) { return LoKi::max( f1, f2 ); }
      static Fun __max__( const Func& f1, double f2 ) { return LoKi::max( f1, f2 ); }
      // max
      static Fun __max__( const Func& f1, const Para& f2 ) { return LoKi::max( f1, PAR( f2 ) ); }
      // max
      static Fun __max__( const Func& f1, const Func& f2, const Func& f3 ) { return LoKi::max( f1, f2, f3 ); }
      // max
      static Fun __max__( const Func& f1, const Func& f2, const Func& f3, const Func& f4 ) {
        return LoKi::max( f1, f2, f3, f4 );
      }
    };
    // ========================================================================
    /** @class FuncOps_
     *  Wrapper class for operations with functions
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-10-07
     */
    template <class TYPE>
    class FuncOps_ : public FuncOps__<TYPE> {
    protected:
      // ======================================================================
      typedef typename FuncOps__<TYPE>::Func Func;
      typedef typename FuncOps__<TYPE>::Cuts Cuts;
      typedef typename FuncOps__<TYPE>::Fun  Fun;
      typedef typename FuncOps__<TYPE>::Cut  Cut;
      // ======================================================================
    public:
      // ======================================================================
      //       // "right" side math. It can't be redefined in base class :-(
      //       static Fun __radd__   ( const Func&  fun1 ,
      //                               double fun2 ) { return fun2 + fun1  ; }
      //       static Fun __rsub__   ( const Func&  fun1 ,
      //                               double fun2 ) { return fun2 - fun1  ; }
      //       static Fun __rmul__   ( const Func&  fun1 ,
      //                               double fun2 ) { return fun2 * fun1  ; }
      //       static Fun __rdiv__   ( const Func&  fun1 ,
      //                               double fun2 ) { return fun2 / fun1  ; }
      //       static Fun __rpow__   ( const Func&  fun1 ,
      //                               double fun2 ) { return LoKi::pow ( fun2 , fun1 ) ; }
      //       static Fun __rpow__   ( const Func&  fun1 ,
      //                               int    fun2 ) { return LoKi::pow ( fun2 , fun1 ) ; }
      // ======================================================================
      static Fun __neg__( const Func& fun ) { return -1 * fun; }
      static Fun __abs__( const Func& fun ) { return LoKi::abs( fun ); }
      // hand-made pseudo-operators to simplify the treatment of math:
      static Fun __sin__( const Func& f ) { return LoKi::sin( f ); }
      static Fun __cos__( const Func& f ) { return LoKi::cos( f ); }
      static Fun __tan__( const Func& f ) { return LoKi::tan( f ); }
      static Fun __sinh__( const Func& f ) { return LoKi::sinh( f ); }
      static Fun __cosh__( const Func& f ) { return LoKi::cosh( f ); }
      static Fun __tanh__( const Func& f ) { return LoKi::tanh( f ); }
      static Fun __asin__( const Func& f ) { return LoKi::asin( f ); }
      static Fun __acos__( const Func& f ) { return LoKi::acos( f ); }
      static Fun __atan__( const Func& f ) { return LoKi::atan( f ); }
      static Fun __exp__( const Func& f ) { return LoKi::exp( f ); }
      static Fun __log__( const Func& f ) { return LoKi::log( f ); }
      static Fun __log10__( const Func& f ) { return LoKi::log10( f ); }
      static Fun __sqrt__( const Func& f ) { return LoKi::sqrt( f ); }
      static Fun __cbrt__( const Func& f ) { return LoKi::cbrt( f ); }
      static Fun __pow2__( const Func& f ) { return LoKi::pow2( f ); }
      static Fun __pow3__( const Func& f ) { return LoKi::pow3( f ); }
      static Fun __pow4__( const Func& f ) { return LoKi::pow4( f ); }
      //
      static Fun __atan__( const Func& f1, const Func& f2 ) { return LoKi::atan2( f1, f2 ); }
      static Fun __atan__( const Func& f1, double f2 ) { return LoKi::atan2( f1, f2 ); }
      static Fun __atan2__( const Func& f1, const Func& f2 ) { return LoKi::atan2( f1, f2 ); }
      static Fun __atan2__( const Func& f1, double f2 ) { return LoKi::atan2( f1, f2 ); }
      // ======================================================================
      // modulo
      // ======================================================================
      static Fun __mod__( const Func& f, unsigned int d ) { return LoKi::Modulo<TYPE>{f, d}; }
      // ======================================================================
      // round, jbit, jdigit, etc..
      // ======================================================================
      static Fun __round__( const Func& f ) { return LoKi::round( f ); }
      static Cut __jbit__( const Func& f, const unsigned short j ) { return LoKi::jbit( f, j ); }
      static Fun __jbits__( const Func& f, const unsigned short j1, const unsigned short j2 ) {
        return LoKi::jbits( f, j1, j2 );
      }
      static Fun __jdigit__( const Func& f, const unsigned short j ) { return LoKi::jdigit( f, j ); }
      static Fun __jdigits__( const Func& f, const unsigned short j1, const unsigned short j2 ) {
        return LoKi::jdigits( f, j1, j2 );
      }
      // ======================================================================
      // monitoring
      // ======================================================================
      // print-monitoring
      static Fun __monitor__( const Func& c, const std::string& s = "\n", const std::string& p = "" ) {
        return LoKi::print( c, std::cout, s, p );
      }
      // monitoring with counters:
      static Fun __monitor__( const Func& c, StatEntity* e ) { return LoKi::monitor( c, e ); }
      // use the services:
      static Fun __monitor__( const Func& c, IStatSvc* s, const std::string& n ) {
        return LoKi::monitor( c, LoKi::Monitoring::getCounter( s, n ) );
      }
      static Fun __monitor__( const Func& c, IAlgContextSvc* s, const std::string& n ) {
        return LoKi::monitor( c, LoKi::Monitoring::getCounter( s, n ) );
      }
      // use the flag:
      static Fun __monitor__( const Func& c, const std::string& n, const LoKi::Monitoring::Flag f ) {
        return LoKi::monitor( c, LoKi::CounterDef( n, f ) );
      }
      static Fun __monitor__( const Func& c, const std::string& g, const std::string& n,
                              const LoKi::Monitoring::Flag f ) {
        return LoKi::monitor( c, LoKi::CounterDef( g, n, f ) );
      }
      // monitoring with histograms:
      static Fun __monitor__( const Func& c, AIDA::IHistogram1D* h ) { return LoKi::monitor( c, h ); }
      // bookthe historgams using IHistogramSvc
      static Fun __monitor__( const Func& c, const std::string& p, const Gaudi::Histo1DDef& h ) {
        return LoKi::plot( c, LoKi::Histo( p, h, nullptr ) );
      }
      static Fun __monitor__( const Func& c, const std::string& d, const std::string& i, const Gaudi::Histo1DDef& h ) {
        return LoKi::plot( c, LoKi::Histo( d, i, h ) );
      }
      static Fun __monitor__( const Func& c, const std::string& d, int i, const Gaudi::Histo1DDef& h ) {
        return LoKi::plot( c, LoKi::Histo( d, i, h ) );
      }
      static Fun __monitor__( const Func& c, const std::string& d, const GaudiAlg::ID& i, const Gaudi::Histo1DDef& h ) {
        return LoKi::plot( c, LoKi::Histo( d, i, h ) );
      }
      // book the historgams using IAlgContextSvc
      static Fun __monitor__( const Func& c, const Gaudi::Histo1DDef& h, const GaudiAlg::ID& i ) {
        return LoKi::plot( c, LoKi::Histo( h, i ) );
      }
      static Fun __monitor__( const Func& c, const Gaudi::Histo1DDef& h, const std::string& i ) {
        return LoKi::plot( c, LoKi::Histo( h, i ) );
      }
      static Fun __monitor__( const Func& c, const Gaudi::Histo1DDef& h, int i ) {
        return LoKi::plot( c, LoKi::Histo( h, i ) );
      }
      // ======================================================================
      // timing
      // ======================================================================
      static Fun __timer__( const Func& c, ChronoEntity* t ) { return LoKi::timer( c, t ); }
      static Fun __timer__( const Func& c, IChronoSvc* s, const std::string& t ) { return LoKi::timer( c, s, t ); }
      static Fun __timer__( const Func& c, const std::string& t ) { return LoKi::timer( c, t ); }
      static Fun __rmod__( const Func& c, const LoKi::Timer& t ) { return t % c; }
      static Fun __rmod__( const Func& c, ChronoEntity* t ) { return t % c; }
      // ======================================================================
      // EqualTo
      // ======================================================================
      static Cut __equal_to__( const Func& fun, double val ) { return fun == val; }
      static Cut __equal_to__( const Func& fun, const Func& fun1 ) { return fun == fun1; }
      //
      static Cut __equal_to__( const Func& fun1, const std::vector<double>& fun2 ) { return fun1 == fun2; }
      static Cut __equal_to__( const Func& fun1, const std::vector<int>& fun2 ) { return fun1 == fun2; }
      static Cut __equal_to__( const Func& fun1, const std::vector<unsigned int>& fun2 ) { return fun1 == fun2; }
      // ======================================================================
      // InRange
      static Cut __in_range__( const Func& fun, double low, double high ) { return LoKi::inRange( low, fun, high ); }
      // ======================================================================
      // InRange
      static Cut __in_range__( const Func& fun, const Func& low, double high ) {
        return LoKi::inRange( low, fun, high );
      }
      // ======================================================================
      // InRange
      static Cut __in_range__( const Func& fun, double low, const Func& high ) {
        return LoKi::inRange( low, fun, high );
      }
      // ======================================================================
      // InRange
      static Cut __in_range__( const Func& fun, const Func& low, const Func& high ) {
        return LoKi::inRange( low, fun, high );
      }
      // ======================================================================
      // InList
      static Cut __in_list__( const Func& fun, const std::vector<double>& fun2 ) { return fun == fun2; }
      // InList
      static Cut __in_list__( const Func& fun, const std::vector<int>& fun2 ) { return fun == fun2; }
      // InList
      static Cut __in_list__( const Func& fun, const std::vector<unsigned int>& fun2 ) { return fun == fun2; }
      // ======================================================================
    public: // simple dumpers
      // ======================================================================
      static Fun __dump1__( const Func& fun, const bool right = true, const LoKi::Dump& dump = LoKi::Dump() ) {
        return LoKi::dump1( fun, right, dump );
      }
    };
    // ========================================================================
    /** @class FuncOps
     *  Wrapper class for operations with functions
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-10-07
     */
    template <class TYPE>
    class FuncOps : public FuncOps_<TYPE> {
    protected:
      // ======================================================================
      typedef typename FuncOps_<TYPE>::Func Func;
      typedef typename FuncOps_<TYPE>::Cuts Cuts;
      typedef typename FuncOps_<TYPE>::Fun  Fun;
      typedef typename FuncOps_<TYPE>::Cut  Cut;
      using TYPE2 = TYPE;
      // ======================================================================
    public:
      // ======================================================================
      // functional part
      // ======================================================================
      // yields/map
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<double>> __yields__( const Func& fun ) {
        return LoKi::yields( fun );
      }
      // process__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __process__( const Func& fun ) {
        return LoKi::process<TYPE2>( fun );
      }
      // min value
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __min_value__( const Func& fun ) {
        return LoKi::min_value( fun );
      }
      // min value
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __min_value__( const Func& fun, double val ) {
        return LoKi::min_value( fun, val );
      }
      // abs min value
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __min_abs_value__( const Func& fun ) {
        return LoKi::min_abs_value( fun );
      }
      // abs min value
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __min_abs_value__( const Func& fun, double val ) {
        return LoKi::min_abs_value( fun, val );
      }
      // maximal value
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __max_value__( const Func& fun ) {
        return LoKi::max_value( fun );
      }
      // maximal value
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __max_value__( const Func& fun, double val ) {
        return LoKi::max_value( fun, val );
      }
      // abs max value
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __max_abs_value__( const Func& fun ) {
        return LoKi::max_abs_value( fun );
      }
      // abs max value
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __max_abs_value__( const Func& fun, double val ) {
        return LoKi::max_abs_value( fun, val );
      }
      // min element
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __min_element__( const Func& fun ) {
        return LoKi::min_element( fun );
      }
      // abs min element
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __min_abs_element__( const Func& fun ) {
        return LoKi::min_abs_element( fun );
      }
      // max element
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __max_element__( const Func& fun ) {
        return LoKi::max_element( fun );
      }
      // abs min element
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __max_abs_element__( const Func& fun ) {
        return LoKi::max_abs_element( fun );
      }
      // ======================================================================
      // sum over the stream
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __sum__( const Func& fun, double init = 0 ) {
        return LoKi::sum( fun, init );
      }
      // sum over the stream
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __sum__( const Func& fun, const Cuts& cut,
                                                                           double init = 0 ) {
        return LoKi::sum( fun, cut, init );
      }
      // ======================================================================
      // product over the stream
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __product__( const Func& fun, double init = 1 ) {
        return LoKi::product( fun, init );
      }
      // ======================================================================
      // sum over the stream
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __product__( const Func& fun, const Cuts& cut,
                                                                               double init = 1 ) {
        return LoKi::product( fun, cut, init );
      }
      // ======================================================================
      // fetch from the stream
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __fetch__( const Func& fun, unsigned int index,
                                                                             double bad ) {
        return LoKi::fetch( fun, index, bad );
      }
      // ======================================================================
      // sort the stream
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __sort__( const Func& fun, int N = -1,
                                                                                        const bool ascending = true ) {
        return ascending ? FunctorFromFunctor{LoKi::Functors::Sort_<TYPE2, double, true>( fun, N )}
                         : FunctorFromFunctor{LoKi::Functors::Sort_<TYPE2, double, false>( fun, N )};
      }
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __sort__( const Func&       fun,
                                                                                        const LoKi::Sort& s ) {
        return __sort__( fun, s.N(), s.ascending() );
      }
      // ======================================================================
    public: // statistics
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __mean__( const Func& fun ) {
        return LoKi::mean( fun );
      }
      // statistics
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __mean__( const Func& fun, const Cuts& cut ) {
        return LoKi::mean( fun, cut );
      }
      // statistics
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __rms__( const Func& fun ) {
        return LoKi::rms( fun );
      }
      // statistics
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __rms__( const Func& fun, const Cuts& cut ) {
        return LoKi::rms( fun, cut );
      }
      // statistics
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __meanErr__( const Func& fun ) {
        return LoKi::meanErr( fun );
      }
      // statistics
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __meanErr__( const Func& fun, const Cuts& cut ) {
        return LoKi::meanErr( fun, cut );
      }
      // ======================================================================
    };
    // ========================================================================
    /** @class CutsOps_
     *  Wrapper class for operations with predicates
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-10-07
     */
    template <class TYPE>
    class CutsOps_ {
    private:
      // ======================================================================
      typedef typename LoKi::BasicFunctors<TYPE>::Function               Func;
      typedef typename LoKi::BasicFunctors<TYPE>::Predicate              Cuts;
      typedef typename LoKi::BasicFunctors<TYPE>::FunctionFromFunction   Fun;
      typedef typename LoKi::BasicFunctors<TYPE>::PredicateFromPredicate Cut;
      // ======================================================================
    public:
      // ======================================================================
      static Cut __or__( const Cuts& cut1, const Cuts& cut2 ) { return cut1 || cut2; }
      static Cut __and__( const Cuts& cut1, const Cuts& cut2 ) { return cut1 && cut2; }
      static Cut __invert__( const Cuts& cut ) { return !cut; }
      // ======================================================================
      // monitoring
      // ======================================================================
      // monitor with printout
      static Cut __monitor__( const Cuts& c, const std::string& s = "\n", const std::string& p = "" ) {
        return LoKi::print( c, std::cout, s, p );
      }
      // monitor with counters:
      static Cut __monitor__( const Cuts& c, StatEntity* e ) { return LoKi::monitor( c, e ); }
      // use the services:
      static Cut __monitor__( const Cuts& c, IStatSvc* s, const std::string& n ) {
        return LoKi::monitor( c, LoKi::Monitoring::getCounter( s, n ) );
      }
      static Cut __monitor__( const Cuts& c, IAlgContextSvc* s, const std::string& n ) {
        return LoKi::monitor( c, LoKi::Monitoring::getCounter( s, n ) );
      }
      // use the flag:
      static Cut __monitor__( const Cuts& c, const std::string& n, const LoKi::Monitoring::Flag f ) {
        return LoKi::monitor( c, LoKi::CounterDef( n, f ) );
      }
      static Cut __monitor__( const Cuts& c, const std::string& g, const std::string& n,
                              const LoKi::Monitoring::Flag f ) {
        return LoKi::monitor( c, LoKi::CounterDef( g, n, f ) );
      }
      // ======================================================================
      // timing
      // ======================================================================
      static Cut __timer__( const Cuts& c, ChronoEntity* t ) { return LoKi::timer( c, t ); }
      static Cut __timer__( const Cuts& c, IChronoSvc* s, const std::string& t ) { return LoKi::timer( c, s, t ); }
      static Cut __timer__( const Cuts& c, const std::string& t ) { return LoKi::timer( c, t ); }
      static Cut __rmod__( const Cuts& c, const LoKi::Timer& t ) { return t % c; }
      static Cut __rmod__( const Cuts& c, ChronoEntity* t ) { return t % c; }
      // ======================================================================
    public: //  switch
      // ======================================================================
      static Fun __switch__( const Cuts& cut, const Func& fun1, const Func& fun2 ) {
        return LoKi::Switch<TYPE>( cut, fun1, fun2 );
      }
      static Fun __switch__( const Cuts& cut, const Func& fun1, double fun2 ) {
        return LoKi::Switch<TYPE>( cut, fun1, fun2 );
      }
      static Fun __switch__( const Cuts& cut, double fun1, const Func& fun2 ) {
        return Switch<TYPE>( cut, fun1, fun2 );
      }
      // switch
      static Fun __switch__( const Cuts& cut, double fun1, double fun2 ) {
        return LoKi::SimpleSwitch<TYPE>( cut, fun1, fun2 );
      }
      // switch
      static Cut __switch__( const Cuts& cut, const Cuts& cut1, const Cuts& cut2 ) {
        return LoKi::Switch<TYPE, bool>( cut, cut1, cut2 );
      }
      // ======================================================================
    public: // scale
      // ======================================================================
      static Cut __scale__( const Cuts& cuts, const LoKi::Functor<void, bool>& sf ) { return LoKi::scale( cuts, sf ); }
      // ======================================================================
    public: // simple dumpers
      // ======================================================================
      static Cut __dump1__( const Cuts& fun, const bool right = true, const LoKi::Dump& dump = LoKi::Dump() ) {
        return LoKi::dump1( fun, right, dump );
      }
      // ======================================================================
    };
    // ========================================================================
    /** @class CutsOps
     *  Wrapper class for operations with predicates
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-10-07
     */
    template <class TYPE, class TYPE2 = TYPE>
    class CutsOps : public CutsOps_<TYPE> {
    private:
      // ======================================================================
      typedef typename LoKi::BasicFunctors<TYPE>::Function               Func;
      typedef typename LoKi::BasicFunctors<TYPE>::Predicate              Cuts;
      typedef typename LoKi::BasicFunctors<TYPE>::FunctionFromFunction   Fun;
      typedef typename LoKi::BasicFunctors<TYPE>::PredicateFromPredicate Cut;
      typedef typename LoKi::BasicFunctors<TYPE2>::Pipe                  Pipe;
      // ======================================================================
    public:
      // ======================================================================
      // functional part
      // ======================================================================
      // filter/select
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __select__( const Cuts& cut ) {
        return LoKi::select( cut );
      }
      // process
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __process__( const Cuts& cut ) {
        return LoKi::process<TYPE2>( cut );
      }
      // count
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __count__( const Cuts& cut ) {
        return LoKi::count<TYPE2>( cut );
      }
      // has
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, bool> __has__( const Cuts& cut ) {
        return LoKi::has<TYPE2>( cut );
      }
      // ======================================================================
      // fetch from the stream
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, bool> __fetch__( const Cuts& fun, unsigned int index,
                                                                           const bool bad = false ) {
        return LoKi::fetch( fun, index, bad );
      }
      // ======================================================================
    public:
      // ======================================================================
      // __union__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __union__( const Cuts& fun,
                                                                                         const Pipe& fun2 ) {
        return LoKi::union_( Functors::Select{fun}, fun2 );
      }
      // __union__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __union__( const Cuts& fun,
                                                                                         const Cuts& fun2 ) {
        return LoKi::union_( Functors::Select{fun}, Functors::Select{fun2} );
      }
      // __intersection__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __intersection__( const Cuts& fun,
                                                                                                const Pipe& fun2 ) {
        return LoKi::Functors::Intersection<std::vector<TYPE2>, TYPE2>( Functors::Select{fun}, fun2 );
      }
      // __intersection__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __intersection__( const Cuts& fun,
                                                                                                const Cuts& fun2 ) {
        return LoKi::Functors::Intersection<std::vector<TYPE2>, TYPE2>( Functors::Select{fun}, Functors::Select{fun2} );
      }
      // __difference__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __difference__( const Cuts& fun,
                                                                                              const Pipe& fun2 ) {
        return LoKi::Functors::Difference<std::vector<TYPE2>, TYPE2>( Functors::Select{fun}, fun2 );
      }
      // __difference__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __difference__( const Cuts& fun,
                                                                                              const Cuts& fun2 ) {
        return LoKi::Functors::Difference<std::vector<TYPE2>, TYPE2>( Functors::Select{fun}, Functors::Select{fun2} );
      }
      // __sym_difference__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __sym_difference__( const Cuts& fun,
                                                                                                  const Pipe& fun2 ) {
        return LoKi::Functors::SymDifference<std::vector<TYPE2>, TYPE2>( Functors::Select{fun}, fun2 );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, std::vector<TYPE2>> __sym_difference__( const Cuts& fun,
                                                                                                  const Cuts& fun2 ) {
        return LoKi::Functors::SymDifference<std::vector<TYPE2>, TYPE2>( Functors::Select{fun},
                                                                         Functors::Select{fun2} );
      }
      // __includes__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, bool> __includes__( const Cuts& fun, const Pipe& fun2 ) {
        return LoKi::Functors::Includes{Functors::Select{fun}, fun2};
      }
      // __includes__
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, bool> __includes__( const Cuts& fun, const Cuts& fun2 ) {
        return LoKi::Functors::Includes{Functors::Select{fun}, Functors::Select{fun2}};
      }
      // ======================================================================
      // statistics
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __eff__( const Cuts& cut ) {
        return LoKi::eff( cut );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __eff__( const Cuts& cut, const Cuts& cut2 ) {
        return LoKi::eff( cut, cut2 );
      }
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __effErr__( const Cuts& cut ) {
        return LoKi::effErr( cut );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE2>, double> __effErr__( const Cuts& cut, const Cuts& cut2 ) {
        return LoKi::effErr( cut, cut2 );
      }
      // ======================================================================
    };
    // ========================================================================
    /** @class MapsOps
     *  Wrapper class for operations with mapping functors
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-11-30
     */
    template <class TYPE>
    class MapsOps {
    private:
      // ======================================================================
      typedef typename LoKi::BasicFunctors<TYPE>::Map Map;
      typedef LoKi::details::result_t<Map>            result_type;
      // ======================================================================
    public:
      // ======================================================================
      static result_type __call__( const Map& fun, typename Map::argument a ) { return fun( a ); }
      // ======================================================================
    public:
      // ======================================================================
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>>
      __rshift__( const Map& fun, const LoKi::BasicFunctors<double>::Map& fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double>
      __rshift__( const Map& fun, const LoKi::BasicFunctors<double>::FunVal& fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool>
      __rshift__( const Map& fun, const LoKi::BasicFunctors<double>::CutVal& fun2 ) {
        return fun >> fun2;
      }
      //
      // ======================================================================
    public: // add the dumps
      // ======================================================================
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __rshift__( const Map&        fun,
                                                                                          const LoKi::Dump& dump ) {
        return fun >> LoKi::Functors::Dump_<double>( dump );
      }
      // ======================================================================
    public: // get the first N
      // ======================================================================
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __rshift__( const Map&          fun,
                                                                                          const LoKi::FirstN& fn ) {
        return fun >> LoKi::Functors::FirstN_<double>( fn );
      }
      // ======================================================================
    public: // reverse
      // ======================================================================
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>>
      __rshift__( const Map& fun, const LoKi::Reverse& /* fn */ ) {
        return fun >> LoKi::Functors::Reverse_<double>();
      }
      // ======================================================================
    public: // gate-2
      // ======================================================================
      // __rrshift__  : gate
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>>
      __rrshift__( const Map& fun, const LoKi::Functor<void, bool>& gate ) {
        return LoKi::gate<TYPE>( gate ) >> fun;
      }
      // __rmul__  : gate
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>>
      __rmul__( const Map& fun, const LoKi::Functor<void, bool>& gate ) {
        return LoKi::gate<TYPE>( gate ) >> fun;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static std::vector<double> __rrshift__( const Map& fun, const std::vector<TYPE>& val ) { return fun( val ); }
      // __rrshift__
      static std::vector<double> __rrshift__( const Map& fun, const typename std::vector<TYPE>::value_type& val ) {
        return fun( std::vector<TYPE>( 1, val ) );
      }
      // __rrshift__
      static std::vector<double> __rrshift__( const Map& fun, const Gaudi::Range_<std::vector<TYPE>>& val ) {
        return val >> fun;
      }
      // __rrshift__
      static std::vector<double> __rrshift__( const Map& fun, const Gaudi::NamedRange_<std::vector<TYPE>>& val ) {
        return val >> fun;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __tee__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __tee__( const Map& fun ) {
        return LoKi::tee( fun );
      }
      // ======================================================================
    public:
      // ======================================================================
      // timing
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __timer__( const Map&    c,
                                                                                         ChronoEntity* t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __timer__( const Map& c, IChronoSvc* s,
                                                                                         const std::string& t ) {
        return LoKi::timer( c, s, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __timer__( const Map&         c,
                                                                                         const std::string& t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __rmod__( const Map&         c,
                                                                                        const LoKi::Timer& t ) {
        return t % c;
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __rmod__( const Map&    c,
                                                                                        ChronoEntity* t ) {
        return t % c;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __union__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __union__( const Map& fun,
                                                                                         const Map& fun2 ) {
        return LoKi::union_( fun, fun2 );
      }
      // __intersection__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __intersection__( const Map& fun,
                                                                                                const Map& fun2 ) {
        return LoKi::Functors::Intersection<std::vector<TYPE>, double>( fun, fun2 );
      }
      // __difference__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __difference__( const Map& fun,
                                                                                              const Map& fun2 ) {
        return LoKi::Functors::Difference<std::vector<TYPE>, double>( fun, fun2 );
      }
      // __sym_difference__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __sym_difference__( const Map& fun,
                                                                                                  const Map& fun2 ) {
        return LoKi::Functors::SymDifference<std::vector<TYPE>, double>( fun, fun2 );
      }
      // __includes__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __includes__( const Map& fun, const Map& fun2 ) {
        return LoKi::Functors::Includes{fun, fun2};
      }
      // ======================================================================
    };
    // ========================================================================
    /** @class PipeOps
     *  Wrapper class for operations with mapping functors
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-11-30
     */
    template <class TYPE>
    class PipeOps {
    private:
      // ======================================================================
      using TYPE2 = TYPE;
      typedef typename LoKi::BasicFunctors<TYPE>::Pipe       Pipe;
      typedef typename LoKi::BasicFunctors<TYPE>::Map        Map;
      typedef typename LoKi::BasicFunctors<TYPE>::FunVal     FunVal;
      typedef typename LoKi::BasicFunctors<TYPE>::CutVal     CutVal;
      typedef typename LoKi::BasicFunctors<TYPE2>::Function  Func;
      typedef typename LoKi::BasicFunctors<TYPE2>::Predicate Cuts;
      typedef LoKi::details::result_t<Pipe>                  result_type;
      // ======================================================================
    public:
      // ======================================================================
      static result_type __call__( const Pipe& fun, typename Pipe::argument a ) { return fun( a ); }
      // ======================================================================
    public:
      // ======================================================================
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __rshift__( const Pipe& fun,
                                                                                          const Map&  fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __rshift__( const Pipe& fun,
                                                                                        const Pipe& fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double> __rshift__( const Pipe& fun, const FunVal& fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __rshift__( const Pipe& fun, const CutVal& fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __rshift__( const Pipe& fun,
                                                                                        const Cuts& fun2 ) {
        return fun >> LoKi::filter( fun2 );
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>> __rshift__( const Pipe& fun,
                                                                                          const Func& fun2 ) {
        return fun >> LoKi::yields( fun2 );
      }
      //
      // ======================================================================
    public: // gate & dump
      // ======================================================================
      // __rshift__  : gate
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>>
      __rshift__( const Pipe& fun, const LoKi::Functor<void, bool>& gate ) {
        return fun >> LoKi::gate<TYPE>( gate );
      }
      // __rshift__  : dump
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __rshift__( const Pipe&       fun,
                                                                                        const LoKi::Dump& dump ) {
        return fun >> LoKi::Functors::Dump_<TYPE>( dump );
      }
      // ======================================================================
    public: // the first N elements
      // ======================================================================
      // __rshift__  : dump
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __rshift__( const Pipe&         fun,
                                                                                        const LoKi::FirstN& fn ) {
        return fun >> LoKi::Functors::FirstN_<TYPE>( fn );
      }
      // ======================================================================
    public: // reverse
      // ======================================================================
      // __rshift__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>>
      __rshift__( const Pipe& fun, const LoKi::Reverse& /* fn */ ) {
        return fun >> LoKi::Functors::Reverse_<TYPE>();
      }
      // ======================================================================
    public: // gate-2
      // ======================================================================
      // __rrshift__  : gate
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>>
      __rrshift__( const Pipe& fun, const LoKi::Functor<void, bool>& gate ) {
        return LoKi::gate<TYPE>( gate ) >> fun;
      }
      // __rmul__  : gate
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>>
      __rmul__( const Pipe& fun, const LoKi::Functor<void, bool>& gate ) {
        return LoKi::gate<TYPE>( gate ) >> fun;
      }
      // ======================================================================
    public: // timing
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __timer__( const Pipe&   c,
                                                                                       ChronoEntity* t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __timer__( const Pipe& c, IChronoSvc* s,
                                                                                       const std::string& t ) {
        return LoKi::timer( c, s, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __timer__( const Pipe&        c,
                                                                                       const std::string& t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __rmod__( const Pipe&        c,
                                                                                      const LoKi::Timer& t ) {
        return t % c;
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __rmod__( const Pipe& c, ChronoEntity* t ) {
        return t % c;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static std::vector<TYPE> __rrshift__( const Pipe& fun, const std::vector<TYPE>& val ) { return fun( val ); }
      // __rrshift__
      static std::vector<TYPE> __rrshift__( const Pipe& fun, const typename std::vector<TYPE>::value_type& val ) {
        return fun( std::vector<TYPE>( 1, val ) );
      }
      // __rrshift__
      static std::vector<TYPE> __rrshift__( const Pipe& fun, const Gaudi::Range_<std::vector<TYPE>>& val ) {
        return val >> fun;
      }
      // __rrshift__
      static std::vector<TYPE> __rrshift__( const Pipe& fun, const Gaudi::NamedRange_<std::vector<TYPE>>& val ) {
        return val >> fun;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __tee__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __tee__( const Pipe& fun ) {
        return LoKi::tee( fun );
      }
      // ======================================================================
    public:
      // ======================================================================
      // __union__
      static LoKi::Assignable_t<Pipe> __union__( const Pipe& fun, const Pipe& fun2 ) {
        return LoKi::union_( fun, fun2 );
      }
      // __union__
      static LoKi::Assignable_t<Pipe> __union__( const Pipe& fun, const Cuts& fun2 ) {
        return LoKi::union_( fun, Functors::Select{fun2} );
      }
      // __intersection__
      static LoKi::Assignable_t<Pipe> __intersection__( const Pipe& fun, const Pipe& fun2 ) {
        return LoKi::Functors::Intersection<std::vector<TYPE>, TYPE>( fun, fun2 );
      }
      // __intersection__
      static LoKi::Assignable_t<Pipe> __intersection__( const Pipe& fun, const Cuts& fun2 ) {
        return LoKi::Functors::Intersection<std::vector<TYPE>, TYPE>( fun, Functors::Select{fun2} );
      }
      // __difference__
      static LoKi::Assignable_t<Pipe> __difference__( const Pipe& fun, const Pipe& fun2 ) {
        return LoKi::Functors::Difference<std::vector<TYPE>, TYPE>( fun, fun2 );
      }
      // __difference__
      static LoKi::Assignable_t<Pipe> __difference__( const Pipe& fun, const Cuts& fun2 ) {
        return LoKi::Functors::Difference<std::vector<TYPE>, TYPE>( fun, Functors::Select{fun2} );
      }
      // __sym_difference__
      static LoKi::Assignable_t<Pipe> __sym_difference__( const Pipe& fun, const Pipe& fun2 ) {
        return LoKi::Functors::SymDifference<std::vector<TYPE>, TYPE>( fun, fun2 );
      }
      // __sym_difference__
      static LoKi::Assignable_t<Pipe> __sym_difference__( const Pipe& fun, const Cuts& fun2 ) {
        return LoKi::Functors::SymDifference<std::vector<TYPE>, TYPE>( fun, Functors::Select{fun2} );
      }
      // __includes__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __includes__( const Pipe& fun, const Pipe& fun2 ) {
        return LoKi::Functors::Includes{fun, fun2};
      }
      // __includes__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __includes__( const Pipe& fun, const Cuts& fun2 ) {
        return LoKi::Functors::Includes{fun, Functors::Select{fun2}};
      }
      // ======================================================================
    };
    // ========================================================================
    /** @class FunValOps
     *  Wrapper class for operations with 'funcion-value'-functors
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-11-30
     */
    template <class TYPE>
    class FunValOps : public FuncOps_<std::vector<TYPE>> {
    private:
      // ======================================================================
      typedef typename LoKi::BasicFunctors<TYPE>::FunVal      FunVal;
      typedef typename LoKi::BasicFunctors<double>::Function  Func;
      typedef typename LoKi::BasicFunctors<double>::Predicate Cuts;
      typedef LoKi::details::result_t<FunVal>                 result_type;
      // ======================================================================
    public:
      // ======================================================================
      static result_type __call__( const FunVal& fun, typename FunVal::argument a ) { return fun( a ); }
      // ======================================================================
    public:
      // ======================================================================
      // __rshift__
      static LoKi::Assignable_t<FunVal> __rshift__( const FunVal& fun, const Func& fun2 ) { return fun >> fun2; }
      // __rshift__
      static LoKi::Assignable_t<LoKi::CutVal_t<TYPE>> __rshift__( const FunVal& fun, const Cuts& fun2 ) {
        return fun >> fun2;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static double __rrshift__( const FunVal& fun, const std::vector<TYPE>& val ) { return fun( val ); }
      // __rrshift__
      static double __rrshift__( const FunVal& fun, const typename std::vector<TYPE>::value_type& val ) {
        return fun( std::vector<TYPE>( 1, val ) );
      }
      // __rrshift__
      static double __rrshift__( const FunVal& fun, const Gaudi::Range_<std::vector<TYPE>>& val ) { return val >> fun; }
      // __rrshift__
      static double __rrshift__( const FunVal& fun, const Gaudi::NamedRange_<std::vector<TYPE>>& val ) {
        return val >> fun;
      }
      // ======================================================================
    public: // gate-2
      // ======================================================================
      // __rrshift__  : gate
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double> __rrshift__( const FunVal&                    fun,
                                                                              const LoKi::Functor<void, bool>& gate ) {
        return LoKi::gate<TYPE>( gate ) >> fun;
      }
      // __rmul__  : gate
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double> __rmul__( const FunVal&                    fun,
                                                                           const LoKi::Functor<void, bool>& gate ) {
        return LoKi::gate<TYPE>( gate ) >> fun;
      }
      // ======================================================================
    public:
      // ======================================================================
      // timing
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double> __timer__( const FunVal& c, ChronoEntity* t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double> __timer__( const FunVal& c, IChronoSvc* s,
                                                                            const std::string& t ) {
        return LoKi::timer( c, s, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double> __timer__( const FunVal& c, const std::string& t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double> __rmod__( const FunVal& c, const LoKi::Timer& t ) {
        return t % c;
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double> __rmod__( const FunVal& c, ChronoEntity* t ) {
        return t % c;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __tee__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __tee__( const FunVal& fun ) {
        return LoKi::tee( fun );
      }
      // ======================================================================
    };
    // ========================================================================
    /** @class SourceOps
     *  Wrapper class for operations with 'source'-functors
     *  @see LoKi::BasicFunctors
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-11-30
     */
    template <class TYPE>
    class SourceOps {
    private:
      using TYPE2 = TYPE;
      // ======================================================================
      typedef typename LoKi::BasicFunctors<TYPE>::Source     Source;
      typedef typename LoKi::BasicFunctors<TYPE>::Pipe       Pipe;
      typedef typename LoKi::BasicFunctors<TYPE>::Map        Map;
      typedef typename LoKi::BasicFunctors<TYPE>::FunVal     FunVal;
      typedef typename LoKi::BasicFunctors<TYPE>::CutVal     CutVal;
      typedef typename LoKi::BasicFunctors<TYPE2>::Function  Func;
      typedef typename LoKi::BasicFunctors<TYPE2>::Predicate Cuts;
      typedef LoKi::details::result_t<Source>                result_type;
      // ======================================================================
    public:
      // ======================================================================
      // __call__
      static result_type __call__( const Source& fun ) { return fun(); }
      // ======================================================================
    public:
      // ======================================================================
      // __rshift__
      static result_type __rshift__( const Source& fun, std::vector<TYPE>& res ) {
        res = fun();
        return res;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __rshift__( const Source& fun, const Pipe& fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<void, std::vector<double>> __rshift__( const Source& fun, const Map& fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<void, double> __rshift__( const Source& fun, const FunVal& fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<void, bool> __rshift__( const Source& fun, const CutVal& fun2 ) {
        return fun >> fun2;
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __rshift__( const Source& fun, const Cuts& fun2 ) {
        return fun >> Functors::Select{fun2};
      }
      // __rshift__
      static LoKi::FunctorFromFunctor<void, std::vector<double>> __rshift__( const Source& fun, const Func& fun2 ) {
        return fun >> Functors::Yields{fun2};
      }
      //
      // ======================================================================
    public: // gate & dump
      // ======================================================================
      // __rshift__ : gate
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __rshift__( const Source&                    fun,
                                                                           const LoKi::Functor<void, bool>& gate ) {
        return fun >> LoKi::gate<TYPE>( gate );
      }
      // __rshift__ : dump
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __rshift__( const Source& fun, const LoKi::Dump& dump ) {
        return fun >> LoKi::Functors::Dump_<TYPE>( dump );
      }
      // ======================================================================
    public: // the first N elements
      // ======================================================================
      // __rshift__  : dump
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __rshift__( const Source& fun, const LoKi::FirstN& fn ) {
        return fun >> LoKi::Functors::FirstN_<TYPE>( fn );
      }
      // ======================================================================
    public: // reverse
      // ======================================================================
      // __rshift__
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __rshift__( const Source& fun,
                                                                           const LoKi::Reverse& /* fn */ ) {
        return fun >> LoKi::Functors::Reverse_<TYPE>();
      }
      // ======================================================================
    public: // cause
      // ======================================================================
      // __rmul__
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __rmul__( const Source&                    fun,
                                                                         const LoKi::Functor<void, bool>& fun2 ) {
        return LoKi::cause( fun, fun2 );
      }
      // __cause__
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __cause__( const Source&                    fun,
                                                                          const LoKi::Functor<void, bool>& fun2 ) {
        return LoKi::cause( fun, fun2 );
      }
      // ======================================================================
    public: // timing
      // ======================================================================
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __timer__( const Source& c, ChronoEntity* t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __timer__( const Source& c, IChronoSvc* s,
                                                                          const std::string& t ) {
        return LoKi::timer( c, s, t );
      }
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __timer__( const Source& c, const std::string& t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __rmod__( const Source& c, const LoKi::Timer& t ) {
        return t % c;
      }
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __rmod__( const Source& c, ChronoEntity* t ) {
        return t % c;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __union__
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __union__( const Source& fun, const Source& fun2 ) {
        return LoKi::union_( fun, fun2 );
      }
      // __intersection__
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __intersection__( const Source& fun,
                                                                                 const Source& fun2 ) {
        return LoKi::Functors::Intersection<void, TYPE>( fun, fun2 );
      }
      // __difference__
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __difference__( const Source& fun, const Source& fun2 ) {
        return Functors::Difference<void, TYPE>( fun, fun2 );
      }
      // __sym_difference__
      static LoKi::FunctorFromFunctor<void, std::vector<TYPE>> __sym_difference__( const Source& fun,
                                                                                   const Source& fun2 ) {
        return Functors::SymDifference<void, TYPE>( fun, fun2 );
      }
      // __includes__
      static LoKi::FunctorFromFunctor<void, bool> __includes__( const Source& fun, const Source& fun2 ) {
        return Functors::Includes{fun, fun2};
      }
      // ======================================================================
    };
    // ========================================================================
    /** @class CutValOps
     *  Wrapper class for operations with 'source'-functors
     *  @see LoKi::BasicFunctors
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-11-30
     */
    template <class TYPE>
    class CutValOps : public CutsOps_<std::vector<TYPE>> {
    public:
      // ======================================================================
      typedef typename LoKi::BasicFunctors<TYPE>::CutVal CutVal;
      typedef typename LoKi::BasicFunctors<TYPE>::Pipe   Pipe;
      typedef typename LoKi::BasicFunctors<TYPE>::Map    Map;
      typedef typename LoKi::BasicFunctors<TYPE>::FunVal FunVal;
      typedef LoKi::details::result_t<CutVal>            result_type;
      // ======================================================================
    public:
      // ======================================================================
      // __call__
      static result_type __call__( const CutVal& fun, typename CutVal::argument a ) { return fun( a ); }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static bool __rrshift__( const CutVal& fun, const std::vector<TYPE>& val ) { return fun( val ); }
      // __rrshift__
      static bool __rrshift__( const CutVal& fun, const typename std::vector<TYPE>::value_type& val ) {
        return fun( std::vector<TYPE>( 1, val ) );
      }
      // __rrshift__
      static bool __rrshift__( const CutVal& fun, const Gaudi::Range_<std::vector<TYPE>>& val ) { return val >> fun; }
      // __rrshift__
      static bool __rrshift__( const CutVal& fun, const Gaudi::NamedRange_<std::vector<TYPE>>& val ) {
        return val >> fun;
      }
      // ======================================================================
    public: // gate-2
      // ======================================================================
      // __rrshift__  : gate
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __rrshift__( const CutVal&                    fun,
                                                                            const LoKi::Functor<void, bool>& gate ) {
        return LoKi::gate<TYPE>( gate ) >> fun;
      }
      // __rmul__  : gate
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __rmul__( const CutVal&                    fun,
                                                                         const LoKi::Functor<void, bool>& gate ) {
        return LoKi::gate<TYPE>( gate ) >> fun;
      }
      // ======================================================================
    public:
      // ======================================================================
      // timing
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __timer__( const CutVal& c, ChronoEntity* t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __timer__( const CutVal& c, IChronoSvc* s,
                                                                          const std::string& t ) {
        return LoKi::timer( c, s, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __timer__( const CutVal& c, const std::string& t ) {
        return LoKi::timer( c, t );
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __rmod__( const CutVal& c, const LoKi::Timer& t ) {
        return t % c;
      }
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __rmod__( const CutVal& c, ChronoEntity* t ) {
        return t % c;
      }
      // ======================================================================
    public: //   switches
      // ======================================================================
      using CutsOps_<std::vector<TYPE>>::__switch__;
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, bool> __switch__( const CutVal& cut, const CutVal& fun1,
                                                                           const CutVal& fun2 ) {
        return LoKi::Switch<std::vector<TYPE>, bool>( cut, fun1, fun2 );
      }
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, double> __switch__( const CutVal& cut, const FunVal& fun1,
                                                                             const FunVal& fun2 ) {
        return LoKi::Switch<std::vector<TYPE>, double>( cut, fun1, fun2 );
      }
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>>
      __switch__( const CutVal& cut, const Pipe& fun1, const Pipe& fun2 ) {
        return LoKi::Switch<std::vector<TYPE>, std::vector<TYPE>>( cut, fun1, fun2 );
      }
      // ======================================================================
      // need to be removed for TYPE==double
      template <class T2 = TYPE, typename = std::enable_if_t<!std::is_same_v<T2, double>>>
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>>
      __switch__( const CutVal& cut, const Map& fun1, const Map& fun2 ) {
        return LoKi::Switch<std::vector<TYPE>, std::vector<double>>( cut, fun1, fun2 );
      }
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>>
      __switch__( const CutVal& cut, const LoKi::Predicate_t<TYPE>& fun1, const LoKi::Predicate_t<TYPE>& fun2 ) {
        return __switch__( cut, Functors::Select{fun1}, Functors::Select{fun2} );
      }
      // ======================================================================
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<double>>
      __switch__( const CutVal& cut, const LoKi::Function_t<TYPE>& fun1, const LoKi::Function_t<TYPE>& fun2 ) {
        return __switch__( cut, Functors::Yields{fun1}, Functors::Yields{fun2} );
      }
      // ======================================================================
    public:
      // ======================================================================
      // __tee__
      static LoKi::FunctorFromFunctor<std::vector<TYPE>, std::vector<TYPE>> __tee__( const CutVal& fun ) {
        return LoKi::tee( fun );
      }
      // ======================================================================
    };
    // ========================================================================
    template <class TYPE>
    struct InfoOps {
      // ======================================================================
      // info
      // ======================================================================
      // __info__
      template <typename F, typename = details::require_signature<F, TYPE, double>>
      static LoKi::Assignable_t<F> __info__( F&& fun, int index, const bool update ) {
        return LoKi::info( index, std::forward<F>( fun ), update );
      }
      // __info__
      template <typename F, typename = details::require_signature<F, TYPE, double>>
      static LoKi::Assignable_t<F> __info__( F&& fun, int index ) {
        return LoKi::info( index, std::forward<F>( fun ) );
      }
      // ======================================================================
      // logging
      // ======================================================================
      // __info__
      template <typename F, typename = details::require_signature<F, TYPE, double>>
      static LoKi::Assignable_t<F> __logging__( F&& fun, int index ) {
        return LoKi::ExtraInfo2::LogInfo<TYPE, double>( std::forward<F>( fun ), index );
      }
      // ======================================================================
    };
    // ========================================================================
  } // namespace Dicts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_FUNCOPS_H

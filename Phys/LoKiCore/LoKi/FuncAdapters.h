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
#ifndef LOKI_FUNCADAPTERS_H
#define LOKI_FUNCADAPTERS_H 1
// ============================================================================
//  include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functions.h"
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
namespace LoKi
{
  // ===========================================================================
  /** @namespace LoKi::Adapters LoKi/FuncAdapters.h
   *  Simple namespace with "function adapters"
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-11-25
   */
  namespace Adapters
  {
    // ========================================================================
    /** @class FunAdapter
     *  The generic templated adapter for the functions
     *
     *  @code
     *
     *  // FUNCTION
     *  double myFun ( const Particle* p ) ;
     *  ...
     *
     *  typedef LoKi::Adapters::FunAdapter<const Particle*> AFun ;
     *
     *  // create the function itself
     *  Fun fun = AFun( &myFun )
     *
     *  const Particle* p = ... ;
     *  // use the function/functor
     *  const double result = fun( p ) ;
     *
     *  @endcode
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-27
     */
    template <class TYPE, class TYPE2=double>
    class FunAdapter : public LoKi::Functor<TYPE,TYPE2>
    {
      // ======================================================================
    public:
      // ======================================================================
      /// the actual type of the function
      typedef TYPE2 (*function)( TYPE ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// contructor from the function
      explicit FunAdapter ( function fun )
        : m_fun  ( fun )
      {}
      /// MANDATORY: clone method ("virtual constructor")
      FunAdapter* clone() const override { return new FunAdapter( *this ) ; }
      /// MANDATORY: the only one essential method
      TYPE2 operator()( typename LoKi::Functor<TYPE,TYPE2>::argument arg ) const override
      { return m_fun( arg ) ; }
      // assignement operator is disabled
      FunAdapter& operator=( const FunAdapter& right ) = delete;
      // ======================================================================
    private:
      // ======================================================================
      function m_fun ;
      // ======================================================================
    };
    // ========================================================================
  } // end of namespace Adapters
  // ==========================================================================
  /** helper templated function to make easier the
   *  creation of adapter-functors:
   *
   *  @code
   *
   *  // FUNCTION
   *  double myCun ( const Particle* p ) ;
   *  ...
   *
   *  // create the function itself
   *  Fun fun = aFun( &myFun )
   *
   *  const Particle* p = ... ;
   *  // use the function/functor
   *  const double result = fun( p ) ;
   *
   *  @endcode
   *
   *  @see LoKi::Adapters::FunAdapter
   *
   *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
   *  @date 2005-03-27
   */
  template <class TYPE>
  inline LoKi::Adapters::FunAdapter<TYPE,double> aFun ( double (*fun) ( TYPE ) )
  { return LoKi::Adapters::FunAdapter<TYPE,double> ( fun ) ; }
  // ==========================================================================
  /** helper templated function to make easier the
   *  creation of adapter-functors:
   *
   *  @code
   *
   *  // PREDICATE
   *  double myCut ( const Particle* p ) ;
   *  ...
   *
   *  // create the function itself
   *  Cut cut = aCut( &myCut )
   *
   *  const Particle* p = ... ;
   *  // use the predicate/functor
   *  const bool result = cut( p ) ;
   *
   *  @endcode
   *
   *  @see LoKi::Adapters::CutAdapter
   *
   *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
   *  @date 2005-03-27
   */
  template <class TYPE>
  inline LoKi::Adapters::FunAdapter<TYPE,bool> aCut ( bool (*cut) ( TYPE ) )
  { return LoKi::Adapters::FunAdapter<TYPE,bool> ( cut ) ; }
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_FUNCADAPTERS_H
// ============================================================================

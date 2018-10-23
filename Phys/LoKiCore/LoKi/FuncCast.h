/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
#ifndef LOKI_FUNCCAST_H
#define LOKI_FUNCCAST_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKiCore
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
 *  @date 2006-03-07
 */
// ============================================================================
namespace LoKi
{
  namespace Adapters
  {
    // ========================================================================
    /** @class FuncCast FuncCast.h LoKi/FuncCast.h
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-03-07
     */
    template <typename TYPE1,typename TYPE2,typename TYPE3=double>
    class FuncCast : public LoKi::Functor<TYPE1,TYPE3>
    {
    public:
      // ======================================================================
      /// Standard constructor
      explicit FuncCast ( LoKi::FunctorFromFunctor<TYPE2,TYPE3> fun )
        : LoKi::AuxFunBase ( std::tie ( fun ) )
        , m_fun ( std::move(fun) )
      {}
      /// MANDATORY: clone method ("virtual constructor")
      FuncCast* clone() const override { return new FuncCast(*this); }
      /// MANDATORY: the only one essential method
      TYPE3
      operator() ( typename LoKi::Functor<TYPE1,TYPE3>::argument a ) const override
      { return m_fun( dynamic_cast<TYPE2>( a )  ) ; }
      /// OPTIONAL: the specific printout
      std::ostream& fillStream ( std::ostream& stream ) const override
      { return stream << m_fun ; }
      /// OPTIONAL: helper method
      TYPE3
      operator() ( typename LoKi::Functor<TYPE2,TYPE3>::argument a ) const override
      { return m_fun ( a ) ; } ;
    private:
      // ======================================================================
      LoKi::FunctorFromFunctor<TYPE2,TYPE3> m_fun ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class FuncStaticCast FuncCast.h LoKi/FuncCast.h
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-03-07
     */
    template <class TYPE1,class TYPE2, class TYPE3=double>
    class FuncStaticCast : public LoKi::Functor<TYPE1,TYPE3>
    {
    public:
      // ======================================================================
      /// Standard constructor
      explicit FuncStaticCast ( LoKi::FunctorFromFunctor<TYPE2,TYPE3> fun )
        : LoKi::AuxFunBase ( std::tie ( fun ) )
        , m_fun ( std::move(fun) )
      {}
      /// MANDATORY: clone method ("virtual constructor")
      FuncStaticCast* clone() const override
      { return new FuncStaticCast(*this); }
      /// MANDATORY: the only one essential method
      TYPE3
      operator() ( typename LoKi::Functor<TYPE1,TYPE3>::argument a ) const override
      { return m_fun.fun ( static_cast<TYPE2>( a ) ) ; } ;
      /// OPTIONAL: the specific printout
      virtual std::ostream& fillStream ( std::ostream& stream ) const
      { return stream << m_fun ; }
      /// OPTIONAL: helper method
      TYPE3
      operator() ( typename LoKi::Functor<TYPE2,TYPE3>::argument a ) const override
      { return m_fun.fun ( a ) ; }
      // ======================================================================
    private:
      // ======================================================================
      LoKi::FunctorFromFunctor<TYPE2,TYPE3> m_fun ;
      // ======================================================================
    } ;
    // ========================================================================
  } // end of the namespace LoKi::Adapters
} // end of the namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_FUNCCAST_H
// ============================================================================

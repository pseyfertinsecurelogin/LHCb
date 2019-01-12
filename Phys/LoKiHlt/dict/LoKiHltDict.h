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
#ifndef DICT_LOKIHLTDICT_H
#define DICT_LOKIHLTDICT_H 1
// ============================================================================
// Include files
// ============================================================================
// Event
// ============================================================================
#include "Event/ODIN.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Dicts.h"
#include "LoKi/FuncOps.h"
#include "LoKi/BasicFunctors.h"
// ============================================================================
#include "LoKi/Odin.h"
#include "LoKi/L0.h"
#include "LoKi/HLT.h"
#include "LoKi/Prescale.h"
// ============================================================================
#include "LoKi/AlgCuts.h" // TEMPORARY
// ============================================================================
#include "LoKi/IHltFactory.h"
#include "LoKi/IHltAntiFactory.h"
#include "LoKi/HltEngine.h"
#include "LoKi/EvtNum.h"
#include "LoKi/TESGet.h"
// ============================================================================
#include "LoKi/IHDRFilterTool.h"
// ============================================================================
/** @file
 *  Simpe file to build the dictionaries for LoKi's functors, which
 *  deal only with DAQ&trigger  values
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2008-09-17
 */
// ============================================================================
namespace LoKi
{
  namespace Dicts
  {
    // ========================================================================
    // O1
    // ========================================================================
    template <>
    class FunCalls<LHCb::ODIN>
    {
    private:
      // ======================================================================
      typedef LHCb::ODIN                                 Type ;
      typedef LoKi::BasicFunctors<const Type*>::Function Fun  ;
      typedef LoKi::details::result_t<Fun>               result_type;
      // ======================================================================
    public:
      // ======================================================================
      static result_type __call__ ( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static result_type __rrshift__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // __rshift__
      static LoKi::Assignable_t<Fun>
      __rshift__ ( const Fun&                          fun  ,
                   const LoKi::Functor<double,double>& o    )
      { return fun >> o  ; }
      // __rshift__
      static LoKi::FunctorFromFunctor<const Type*,bool>
      __rshift__( const Fun&                          fun  ,
                  const LoKi::Functor<double,bool>&   o    )
      { return fun >> o  ; }
      // ======================================================================
    } ;
    // ========================================================================
   template <>
   class CutCalls<LHCb::ODIN>
    {
    private:
      // ======================================================================
      typedef LHCb::ODIN                                  Type ;
      typedef LoKi::BasicFunctors<const Type*>::Predicate Fun  ;
      typedef LoKi::details::result_t<Fun>                result_type;
      // ======================================================================
    public:
      // ======================================================================
      static result_type __call__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      //
      static result_type  __rrshift__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      //
      static LoKi::Assignable_t<Fun> __rshift__( const Fun& fun, const Fun& o )
      { return fun >> o  ; }
      // ======================================================================
   } ;
    // ========================================================================
    // L0
    // ========================================================================
    template <>
    class FunCalls<LHCb::L0DUReport>
    {
    private:
      typedef LHCb::L0DUReport                           Type ;
      typedef LoKi::BasicFunctors<const Type*>::Function Fun  ;
      typedef LoKi::details::result_t<Fun>               result_type;
    public:
      //
      static result_type __call__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      // ======================================================================
      // __rrshift__
      static result_type __rrshift__ ( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      // ======================================================================
      // __rshift__
      static LoKi::Assignable_t<Fun>
      __rshift__( const Fun& fun,
                  const LoKi::Functor<double,double>& o )
      { return fun >> o  ; }
      // __rshift__
      static LoKi::FunctorFromFunctor<const Type*,bool>
      __rshift__( const Fun& fun,
                  const LoKi::Functor<double,bool>& o )
      { return fun >> o  ; }
      // ======================================================================
    } ;
    // ========================================================================
   template <>
   class CutCalls<LHCb::L0DUReport>
    {
    private:
      // ======================================================================
      typedef LHCb::L0DUReport                            Type ;
      typedef LoKi::BasicFunctors<const Type*>::Predicate Fun  ;
      typedef LoKi::details::result_t<Fun>                result_type;
      // ======================================================================
    public:
      // ======================================================================
      static result_type __call__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      // ======================================================================
      static result_type __rrshift__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      // ======================================================================
      static LoKi::Assignable_t<Fun>
      __rshift__( const Fun& fun, const Fun& o )
      { return fun >> o  ; }
      // ======================================================================
   } ;
    // ========================================================================
    // HDR
    // ========================================================================
    template <>
    class FunCalls<LHCb::HltDecReports>
    {
    private:
      typedef LHCb::HltDecReports                        Type ;
      typedef LoKi::BasicFunctors<const Type*>::Function Fun  ;
      typedef LoKi::details::result_t<Fun>               result_type;
    public:
      //
      static result_type __call__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static result_type __rrshift__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // __rshift__
      static LoKi::Assignable_t<Fun>
      __rshift__( const Fun& fun  ,
                  const LoKi::Functor<double,double>& o )
      { return fun >> o  ; }
      // __rshift__
      static LoKi::FunctorFromFunctor<const Type*,bool>
      __rshift__( const Fun& fun  ,
                  const LoKi::Functor<double,bool>& o )
      { return fun >> o  ; }
      // ======================================================================
    } ;
    // ========================================================================
   template <>
   class CutCalls<LHCb::HltDecReports>
    {
    private:
      // ======================================================================
      typedef LHCb::HltDecReports                         Type ;
      typedef LoKi::BasicFunctors<const Type*>::Predicate Fun  ;
      typedef LoKi::details::result_t<Fun>                result_type;
      // ======================================================================
    public:
      // ======================================================================
      static result_type __call__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      //
      static result_type  __rrshift__( const Fun& fun, const Type* o )
      { return fun ( o ) ; }
      //
      static LoKi::Assignable_t<Fun> __rshift__( const Fun& fun, const Fun& o )
      { return fun >> o  ; }
      //
      // ======================================================================
   } ;
    // ========================================================================
  } //                                             end of namespace LoKi::Dicts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace
{
  struct _Instantiations
  {
    // ========================================================================
    /// O1
    /// function
    LoKi::FunctorFromFunctor<const LHCb::ODIN*,double>                 m_001  ;
    /// predicate
    LoKi::FunctorFromFunctor<const LHCb::ODIN*,bool>                   m_002  ;
    ///
    LoKi::Dicts::FuncOps<const LHCb::ODIN*>                            m_op01 ;
    LoKi::Dicts::CutsOps<const LHCb::ODIN*>                            m_op02 ;
    ///
    LoKi::Dicts::FunCalls<LHCb::ODIN>                                  m_cs01 ;
    LoKi::Dicts::CutCalls<LHCb::ODIN>                                  m_cs02 ;
    ///
    LoKi::Constant<const LHCb::ODIN*,double>                           m_co01 ;
    LoKi::Constant<const LHCb::ODIN*,bool>                             m_co02 ;
    // ========================================================================
    /// L0
    /// function
    LoKi::FunctorFromFunctor<const LHCb::L0DUReport*,double>   m_0011  ;
    /// predicate
    LoKi::FunctorFromFunctor<const LHCb::L0DUReport*,bool>     m_0021  ;
    ///
    LoKi::Dicts::FuncOps<const LHCb::L0DUReport*>              m_op011 ;
    LoKi::Dicts::CutsOps<const LHCb::L0DUReport*>              m_op021 ;
    ///
    LoKi::Dicts::FunCalls<LHCb::L0DUReport>                    m_cs011 ;
    LoKi::Dicts::CutCalls<LHCb::L0DUReport>                    m_cs021 ;
    ///
    LoKi::Constant<const LHCb::L0DUReport*,double>             m_co011 ;
    LoKi::Constant<const LHCb::L0DUReport*,bool>               m_co021 ;
    // ========================================================================
    /// HDR
    /// function
    LoKi::FunctorFromFunctor<const LHCb::HltDecReports*,double>   m_0012  ;
    /// predicate
    LoKi::FunctorFromFunctor<const LHCb::HltDecReports*,bool>     m_0022  ;
    ///
    LoKi::Dicts::FuncOps<const LHCb::HltDecReports*>              m_op012 ;
    LoKi::Dicts::CutsOps<const LHCb::HltDecReports*>              m_op022 ;
    ///
    LoKi::Dicts::FunCalls<LHCb::HltDecReports>                    m_cs012 ;
    LoKi::Dicts::CutCalls<LHCb::HltDecReports>                    m_cs022 ;
    ///
    LoKi::Constant<const LHCb::HltDecReports*,double>             m_co012 ;
    LoKi::Constant<const LHCb::HltDecReports*,bool>               m_co022 ;
    /// fictive non-existent constructor
    _Instantiations() ;
    // ========================================================================
    std::vector<unsigned long long>  m_l1 ;
    std::vector<         long long>  m_l2 ;
    std::vector<unsigned      long>  m_l3 ;
    std::vector<              long>  m_l4 ;
    // ========================================================================
  };
  // ==========================================================================
} //                                                 end of anonymous namespace
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // DICT_LOKINUMBERSDICT_H

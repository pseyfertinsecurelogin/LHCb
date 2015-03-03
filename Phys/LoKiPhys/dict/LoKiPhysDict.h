// $Id: LoKiPhysDict.h,v 1.24 2009-11-28 19:29:45 ibelyaev Exp $
// ============================================================================
#ifndef LOKI_LOKIPHYSDICT_H 
#define LOKI_LOKIPHYSDICT_H 1
// ============================================================================
// Include files
// ============================================================================
// Event
// ============================================================================
#include "Event/Track.h"
#include "Event/Particle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/LoKiPhys.h"
#include "LoKi/Keeper.h"
#include "LoKi/UniqueKeeper.h"
#include "LoKi/Monitoring.h"
#include "LoKi/ExtraInfo.h"
// ============================================================================
#include "LoKi/Child.h"
#include "LoKi/Dicts.h"
#include "LoKi/PhysAlgsDicts.h"
#include "LoKi/PhysExtractDicts.h"
#include "LoKi/PhysMoniDicts.h"
#include "LoKi/Filters.h"
#include "LoKi/Sources.h"
#include "LoKi/SelectVertex.h"
// ============================================================================
#include "LoKi/ParticleContextCuts.h"
#include "LoKi/VertexContextCuts.h"
#include "LoKi/Legacy1.h"
#include "LoKi/Particles26.h"
#include "LoKi/Particles27.h"
#include "LoKi/Particles28.h"
#include "LoKi/Particles29.h"
// ============================================================================
#include "LoKi/IDecay.h"
#include "LoKi/FinderDict.h"
#include "LoKi/Decays.h"
#include "LoKi/TreeOps.h"
// ============================================================================
#include "LoKi/Photons.h"
// ============================================================================
/** @file
 *  The dictionaries for the package Phys/LoKiPhys
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
 *  @date 2007-12-01
 */
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  namespace Functors 
  {
    // ========================================================================
    // the specific printout
    // ========================================================================
    template <>
    std::ostream& Empty<const LHCb::Particle*>::fillStream
    ( std::ostream& s ) const { return s << "EMPTY" ; }
    // ========================================================================
    // the specific printpout
    // ========================================================================
    template <>
    std::ostream& 
    Size<const LHCb::Particle*>::fillStream
    ( std::ostream& s ) const { return s << "SIZE" ; }
    // ========================================================================
    // the specific printout
    // ========================================================================
    template <>
    std::ostream& 
    Empty<const LHCb::VertexBase*>::fillStream
    ( std::ostream& s ) const { return s << "VEMPTY" ; }
    // ========================================================================
    // the specific printpout
    // ========================================================================
    template <>
    std::ostream& 
    Size<const LHCb::VertexBase*>::fillStream
    ( std::ostream& s ) const { return s << "VSIZE" ; }
    // ========================================================================
  } //                                          end of namespace LoKi::Functors 
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  namespace Dicts
  {
    // ========================================================================
    template <>
    class FunCalls<LHCb::VertexBase> 
    {
    private:
      // ======================================================================
      typedef LHCb::VertexBase            Type ;
      typedef LoKi::BasicFunctors<const Type*>::Function Fun  ;
      // ======================================================================
    public:
      // ======================================================================
      static Fun::result_type __call__ 
      ( const Fun& fun  , const LHCb::VertexBase*           o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const LHCb::Vertex*               o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const LHCb::RecVertex*            o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const SmartRef<LHCb::Vertex>&     o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const SmartRef<LHCb::RecVertex>&  o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const SmartRef<LHCb::VertexBase>& o ) { return fun ( o ) ; }
      // ======================================================================
    public:
      // ======================================================================
      static std::vector<Fun::result_type> __rrshift__ 
      ( const Fun& fun  , const LHCb::VertexBase::ConstVector&    o ) 
      { return o >> fun  ; }
      static std::vector<Fun::result_type> __rrshift__ 
      ( const Fun& fun  , const LHCb::RecVertex::ConstVector&     o ) 
      { return o >> fun  ; }
      static std::vector<Fun::result_type> __rrshift__ 
      ( const Fun& fun  , const LHCb::Vertex::ConstVector&        o ) 
      { return o >> fun  ; }
      // ======================================================================
    public:
      // ======================================================================
      // __rshift__ 
      static LoKi::FunctorFromFunctor<const Type*,double> __rshift__            
      ( const Fun&                          fun  , 
        const LoKi::Functor<double,double>& o    ) { return fun >> o  ; }
      // __rshift__ 
      static LoKi::FunctorFromFunctor<const Type*,bool>   __rshift__            
      ( const Fun&                          fun  , 
        const LoKi::Functor<double,bool>&   o    ) { return fun >> o  ; }
      //
      // ======================================================================
    } ;
    // ========================================================================
    template <>
    class CutCalls<LHCb::VertexBase> 
    {
    private:
      // ======================================================================
      typedef LHCb::VertexBase                            Type ;
      typedef LoKi::BasicFunctors<const Type*>::Predicate Fun  ;
      // ======================================================================
    public:
      // ======================================================================
      static Fun::result_type __call__ 
      ( const Fun& fun  , const LHCb::VertexBase*           o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const LHCb::Vertex*               o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const LHCb::RecVertex*            o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const SmartRef<LHCb::Vertex>&     o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const SmartRef<LHCb::RecVertex>&  o ) { return fun ( o ) ; }
      static Fun::result_type __call__ 
      ( const Fun& fun  , const SmartRef<LHCb::VertexBase>& o ) { return fun ( o ) ; }
      // ======================================================================
      static LHCb::VertexBase::ConstVector  __rrshift__ 
      ( const Fun& fun  , const LHCb::VertexBase::ConstVector& o ) 
      { return o >> fun  ; }
      static LHCb::RecVertex::ConstVector   __rrshift__ 
      ( const Fun& fun  , const LHCb::RecVertex::ConstVector&  o ) 
      { return o >> fun  ; }
      static LHCb::Vertex::ConstVector      __rrshift__ 
      ( const Fun& fun  , const LHCb::Vertex::ConstVector&     o ) 
      { return o >> fun  ; }
      // ======================================================================
      static LoKi::FunctorFromFunctor<const Type*,bool> __rshift__            
      ( const Fun& fun  , const Fun&                         o ) { return fun >> o  ; }
      // ======================================================================
    } ;
    // ========================================================================
  }
  // ==========================================================================
}
// ============================================================================
namespace
{
  // ==========================================================================
  struct _Instantiations 
  {
    // fictive constructor 
    _Instantiations () ;
    // the basic types
    LoKi::Types::Range                              m_r1 ;
    LoKi::Types::VRange                             m_r2 ;
    // rangelists:
    LoKi::Types::RangeL                             m_l1 ;
    LoKi::Types::VRangeL                            m_l2 ;
    // keepers:
    LoKi::Keeper<LHCb::Particle>                    m_k1 ;
    LoKi::Keeper<LHCb::VertexBase>                  m_k2 ;
    LoKi::Keeper<LHCb::Vertex>                      m_k3 ;
    LoKi::Keeper<LHCb::RecVertex>                   m_k4 ;
   // unique keepers
    LoKi::UniqueKeeper<LHCb::Particle>              m_u1 ;
    LoKi::UniqueKeeper<LHCb::VertexBase>            m_u2 ;
    LoKi::UniqueKeeper<LHCb::Vertex>                m_u3 ;
    LoKi::UniqueKeeper<LHCb::RecVertex>             m_u4 ;
    // operators 
    LoKi::Dicts::FuncOps<const LHCb::Particle*>     m_o1 ;
    LoKi::Dicts::FuncOps<const LHCb::VertexBase*>   m_o2 ;
    LoKi::Dicts::CutsOps<const LHCb::Particle*>     m_o3 ;
    LoKi::Dicts::CutsOps<const LHCb::VertexBase*>   m_o4 ;
    // functionals:
    LoKi::Dicts::MapsOps<const LHCb::Particle*>      m_fo1 ;
    LoKi::Dicts::MapsOps<const LHCb::VertexBase*>    m_fo2 ;
    LoKi::Dicts::PipeOps<const LHCb::Particle*>      m_fo3 ;
    LoKi::Dicts::PipeOps<const LHCb::VertexBase*>    m_fo4 ;
    LoKi::Dicts::FunValOps<const LHCb::Particle*>    m_fo5 ;
    LoKi::Dicts::FunValOps<const LHCb::VertexBase*>  m_fo6 ;
    LoKi::Dicts::CutValOps<const LHCb::Particle*>    m_fo51;
    LoKi::Dicts::CutValOps<const LHCb::VertexBase*>  m_fo61;
    LoKi::Dicts::ElementOps<const LHCb::Particle*>   m_fo7 ;
    LoKi::Dicts::ElementOps<const LHCb::VertexBase*> m_fo8 ;
    LoKi::Dicts::SourceOps<const LHCb::Particle*>    m_fo9 ;
    LoKi::Dicts::SourceOps<const LHCb::VertexBase*>  m_f10 ;
    // infor
    LoKi::Dicts::InfoOps<const LHCb::Particle*>      m_io1 ;
    LoKi::Dicts::InfoOps<const LHCb::VertexBase*>    m_io2 ;
    // calls
    LoKi::Dicts::FunCalls<LHCb::Particle>           m_c1 ;
    LoKi::Dicts::CutCalls<LHCb::Particle>           m_c2 ;
    LoKi::Dicts::FunCalls<LHCb::VertexBase>         m_c3 ;
    LoKi::Dicts::CutCalls<LHCb::VertexBase>         m_c4 ;
    // the basic functions 
    LoKi::Dicts::Funcs<const LHCb::Particle*>       m_f1 ;
    LoKi::Dicts::Funcs<const LHCb::VertexBase*>     m_f2 ;
    LoKi::Dicts::VFuncs<const LHCb::Particle*>      m_f3 ;
    LoKi::Dicts::VFuncs<const LHCb::VertexBase*>    m_f4 ;
    // special operators for identifiers 
    LoKi::Dicts::PIDOps<LoKi::Particles::Identifier>    m_i1 ;
    LoKi::Dicts::PIDOps<LoKi::Particles::AbsIdentifier> m_21 ;    
    // Monitoring & Extra Info    
    LoKi::ExtraInfo::GetInfo<const LHCb::Particle*>     m_53 ;
    LoKi::ExtraInfo::CheckInfo<const LHCb::Particle*>   m_54 ;
    LoKi::ExtraInfo::GetInfo<const LHCb::VertexBase*>   m_63 ;
    LoKi::ExtraInfo::CheckInfo<const LHCb::VertexBase*> m_64 ;
    // Monitoring 
    LoKi::Monitoring::Counter<const LHCb::Particle*,bool>   m_50 ;
    LoKi::Monitoring::Counter<const LHCb::Particle*,double> m_51 ;
    LoKi::Monitoring::Plot<const LHCb::Particle*,double>    m_52 ;
    LoKi::Monitoring::Counter<const LHCb::VertexBase*,bool> m_60 ;
    LoKi::Monitoring::Counter<const LHCb::VertexBase*,double> m_61 ;
    LoKi::Monitoring::Plot<const LHCb::VertexBase*,double>  m_62 ;
    // additional 
    LoKi::TheSame<const LHCb::Particle*>                m_71 ;
    LoKi::TheSame<const LHCb::VertexBase*>              m_72 ;
    LoKi::Functors::Empty<const LHCb::Particle*>        m_ee1 ;
    LoKi::Functors::Empty<const LHCb::VertexBase*>      m_ee2 ;
    LoKi::Functors::Size<const LHCb::Particle*>         m_es1 ;
    LoKi::Functors::Size<const LHCb::VertexBase*>       m_es2 ;
    // decay funders:
    Decays::Tree_<const LHCb::Particle*>                m_tree1 ;
    Decays::Trees::Any_<const LHCb::Particle*>          m_tree2 ;
    LoKi::Dicts::TreeOps<const LHCb::Particle*>         m_trops ;
    // =======================================================================
    Decays::IDecay::Finder m_finder ;
    Decays::IDecay::Tree   m_tree   ;
    LoKi::Dicts::FinderDicts_<const LHCb::Particle*> m_finderDicts ;
    // =======================================================================    
    LHCb::CaloDigit::Vector  m_calo_1 ;
    LHCb::CaloDigit::Set     m_calo_2 ;
    LHCb::CaloCellID::Vector m_calo_3 ;
    LHCb::CaloCellID::Set    m_calo_4 ;    
    // =======================================================================    
  } ;  
  // ==========================================================================
}
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_LOKIPHYSDICT_H
// ============================================================================

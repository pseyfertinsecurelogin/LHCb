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
#ifndef LOKI_LOKIGEN_DCT_H
#define LOKI_LOKIGEN_DCT_H 1
// ============================================================================
// Include files
// ============================================================================
// HepMC
// ============================================================================
#ifdef __INTEL_COMPILER
#  pragma warning( disable : 1572 ) // floating-point equality and inequality comparisons are unreliable
#  pragma warning( push )
#endif
// ============================================================================
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
// ============================================================================
#ifdef __INTEL_COMPILER
#  pragma warning( pop )
#endif
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Keeper.h"
#include "LoKi/LoKiGen.h"
#include "LoKi/Monitoring.h"
#include "LoKi/Operators.h"
#include "LoKi/TreeOps.h"
#include "LoKi/Trees.h"
#include "LoKi/UniqueKeeper.h"
// ============================================================================
#include "LoKi/Dicts.h"
#include "LoKi/GenAlgsDicts.h"
#include "LoKi/GenChildSelector.h"
#include "LoKi/GenDecayChain.h"
#include "LoKi/GenDecays.h"
#include "LoKi/GenDump.h"
#include "LoKi/GenExtractDicts.h"
#include "LoKi/GenMoniDicts.h"
#include "LoKi/GenParticles.h"
#include "LoKi/GenParticles2.h"
#include "LoKi/GenParticles3.h"
#include "LoKi/GenParticles4.h"
#include "LoKi/GenParticles5.h"
// ============================================================================
#include "LoKi/FinderDict.h"
#include "LoKi/IGenDecay.h"
// ============================================================================
#include "LoKi/GenHybridEngine.h"
#include "LoKi/IGenHybridTool.h"
// ============================================================================
/** @file
 *  The dictionaries for the package Phys/LoKiGen
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
namespace LoKi {
  // ==========================================================================
  namespace Functors {
    // ========================================================================
    // the specialized printout
    // ========================================================================
    template <>
    inline std::ostream& Empty<const HepMC::GenVertex*>::fillStream( std::ostream& s ) const {
      return s << "GVEMPTY";
    }
    // ========================================================================
    // the specialized printout
    // ========================================================================
    template <>
    inline std::ostream& Empty<const HepMC::GenParticle*>::fillStream( std::ostream& s ) const {
      return s << "GEMPTY ";
    }
    // ========================================================================
    // the specialized printout
    // ========================================================================
    template <>
    inline std::ostream& Size<const HepMC::GenVertex*>::fillStream( std::ostream& s ) const {
      return s << "GVSIZE";
    }
    // ========================================================================
    // the specialized printout
    // ========================================================================
    template <>
    inline std::ostream& Size<const HepMC::GenParticle*>::fillStream( std::ostream& s ) const {
      return s << "GSIZE ";
    }
    // ========================================================================
  } // namespace Functors
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Dicts {
    // ========================================================================
    template <>
    class FunCalls<HepMC::GenParticle> {
    private:
      // ======================================================================
      typedef HepMC::GenParticle                         Type;
      typedef LoKi::BasicFunctors<const Type*>::Function Fun;
      typedef LoKi::details::result_t<Fun>               result_type;
      // ======================================================================
    public:
      // ======================================================================
      // __call__
      static result_type __call__( const Fun& fun, const Type* o ) { return fun( o ); }
      // ======================================================================
      // __rrshift__
      static result_type __rrshift__( const Fun& fun, const Type* o ) { return o >> fun; }
      // __rrshift__
      static std::vector<result_type> __rrshift__( const Fun& fun, const LoKi::GenTypes::GenContainer& o ) {
        return o >> fun;
      }
      // ======================================================================
      // _rshift_
      static LoKi::FunctorFromFunctor<const Type*, double> __rshift__( const Fun&                           fun,
                                                                       const LoKi::Functor<double, double>& o ) {
        return fun >> o;
      }
      // _rshift_
      static LoKi::FunctorFromFunctor<const Type*, bool> __rshift__( const Fun&                         fun,
                                                                     const LoKi::Functor<double, bool>& o ) {
        return fun >> o;
      }
      // ======================================================================
    };
    // ========================================================================
    template <>
    class CutCalls<HepMC::GenParticle> {
    private:
      // ======================================================================
      typedef HepMC::GenParticle                          Type;
      typedef LoKi::BasicFunctors<const Type*>::Predicate Fun;
      typedef LoKi::details::result_t<Fun>                result_type;
      // ======================================================================
    public:
      // ======================================================================
      // __call__
      static result_type __call__( const Fun& fun, const Type* o ) { return fun( o ); }
      //
      // __call__ as filter
      //
      // __call__
      static LoKi::GenTypes::GenContainer __call__( const Fun& fun, const LoKi::GenTypes::GenContainer& o ) {
        return o >> fun;
      }
      // __call__
      static LoKi::GenTypes::GenContainer __call__( const Fun& fun, const LoKi::GenTypes::GRange& o ) {
        return o >> fun;
      }
      // __call__
      static LoKi::GenTypes::GenContainer __call__( const Fun& fun, const HepMC::GenEvent* e ) { return e >> fun; }
      // __call__
      static LoKi::GenTypes::GenContainer __call__( const Fun& fun, const LHCb::HepMCEvent* e ) { return e >> fun; }
      // __call__
      static LoKi::GenTypes::GenContainer __call__( const Fun& fun, const LHCb::HepMCEvent::Container* e ) {
        return e >> fun;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static result_type __rrshift__( const Fun& fun, const Type* o ) { return o >> fun; }
      //
      // rrshift as "filter"
      //
      // __rrshift__
      static LoKi::GenTypes::GenContainer __rrshift__( const Fun& fun, const LoKi::GenTypes::GenContainer& o ) {
        return o >> fun;
      }
      // __rrshift__
      static LoKi::GenTypes::GenContainer __rrshift__( const Fun& fun, const LoKi::GenTypes::GRange& o ) {
        return o >> fun;
      }
      // __rrshift__
      static LoKi::GenTypes::GenContainer __rrshift__( const Fun& fun, const HepMC::GenEvent* e ) { return e >> fun; }
      // __rrshift__
      static LoKi::GenTypes::GenContainer __rrshift__( const Fun& fun, const LHCb::HepMCEvent* e ) { return e >> fun; }
      // __rrshift__
      static LoKi::GenTypes::GenContainer __rrshift__( const Fun& fun, const LHCb::HepMCEvent::Container* e ) {
        return e >> fun;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __rshift__
      static LoKi::FunctorFromFunctor<const Type*, bool> __rshift__( const Fun& fun, const Fun& o ) { return fun >> o; }
      // ======================================================================
    };
    // ========================================================================
    template <>
    class FunCalls<HepMC::GenVertex> {
    private:
      // ======================================================================
      typedef HepMC::GenVertex                           Type;
      typedef LoKi::BasicFunctors<const Type*>::Function Fun;
      typedef LoKi::details::result_t<Fun>               result_type;
      // ======================================================================
    public:
      // ======================================================================
      // __call__
      static result_type __call__( const Fun& fun, const Type* o ) { return fun( o ); }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static result_type __rrshift__( const Fun& fun, const Type* o ) { return o >> fun; }
      // __rrshift__
      static std::vector<result_type> __rrshift__( const Fun& fun, const LoKi::GenTypes::GenVContainer& o ) {
        return o >> fun;
      }
      // ======================================================================
    public:
      // ======================================================================
      // _rshift_
      static LoKi::FunctorFromFunctor<const Type*, double> __rshift__( const Fun&                           fun,
                                                                       const LoKi::Functor<double, double>& o ) {
        return fun >> o;
      }
      // _rshift_
      static LoKi::FunctorFromFunctor<const Type*, bool> __rshift__( const Fun&                         fun,
                                                                     const LoKi::Functor<double, bool>& o ) {
        return fun >> o;
      }
      // ======================================================================
    };
    // ========================================================================
    template <>
    class CutCalls<HepMC::GenVertex> {
    private:
      // ======================================================================
      typedef HepMC::GenVertex                            Type;
      typedef LoKi::BasicFunctors<const Type*>::Predicate Fun;
      typedef LoKi::details::result_t<Fun>                result_type;
      // ======================================================================
    public:
      // ======================================================================
      // __call__
      static result_type __call__( const Fun& fun, const Type* o ) { return fun( o ); }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static result_type __rrshift__( const Fun& fun, const Type* o ) { return o >> fun; }
      // __rrshift__
      static const LoKi::GenTypes::GenVContainer __rrshift__( const Fun& fun, const LoKi::GenTypes::GenVContainer& o ) {
        return o >> fun;
      }
      // ======================================================================
    public:
      // ======================================================================
      // __rrshift__
      static LoKi::FunctorFromFunctor<const Type*, bool> __rshift__( const Fun& fun, const Fun& o ) { return fun >> o; }
      // ======================================================================
    };
    // ========================================================================
  } // namespace Dicts
  // ==========================================================================
} // namespace LoKi
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Dicts {
    // ========================================================================
    template <>
    struct FinderDicts_<const HepMC::GenParticle*> {
      // ======================================================================
      /// get the actual "stripped" type
      typedef const HepMC::GenParticle*              TYPE;
      typedef const HepMC::GenParticle*              Type;
      typedef std::vector<const HepMC::GenParticle*> ConstVector;
      // ======================================================================
    public:
      // ======================================================================
      static bool __hasDecay__( const Decays::Finder_<TYPE>& finder, const ConstVector& input ) {
        return finder.hasDecay( input.begin(), input.end() );
      }
      // ======================================================================
      static bool __hasDecay__( const Decays::Finder_<TYPE>& finder, const HepMC::GenEvent* event ) {
        return event && finder.hasDecay( event->particles_begin(), event->particles_end() );
      }
      // ======================================================================
      static bool __hasDecay__( const Decays::Finder_<TYPE>& finder, const LHCb::HepMCEvent* event ) {
        return event && __hasDecay__( finder, event->pGenEvt() );
      }
      // ======================================================================
      static bool __hasDecay__( const Decays::Finder_<TYPE>& finder, const LHCb::HepMCEvent::Container* events ) {
        return events && std::any_of( events->begin(), events->end(),
                                      [&]( const LHCb::HepMCEvent* e ) { return __hasDecay__( finder, e ); } );
      }
      // ======================================================================
      static bool __hasDecay__( const Decays::Finder_<TYPE>& finder, const HepMC::GenVertex* vertex,
                                HepMC::IteratorRange range ) {
        if ( !vertex ) { return false; }
        HepMC::GenVertex* _vx = const_cast<HepMC::GenVertex*>( vertex );
        return finder.hasDecay( _vx->particles_begin( range ), _vx->particles_end( range ) );
      }
      // ======================================================================
    public:
      // ======================================================================
      static size_t __findDecay__( const Decays::Finder_<TYPE>& finder, const ConstVector& input,
                                   ConstVector& output ) {
        return finder.findDecay( input.begin(), input.end(), output );
      }
      // ======================================================================
      static size_t __findDecay__( const Decays::Finder_<TYPE>& finder, const HepMC::GenEvent* event,
                                   ConstVector& output ) {
        return event ? finder.findDecay( event->particles_begin(), event->particles_end(), output ) : 0;
      }
      // ======================================================================
      static size_t __findDecay__( const Decays::Finder_<TYPE>& finder, const LHCb::HepMCEvent* event,
                                   ConstVector& output ) {
        return event ? __findDecay__( finder, event->pGenEvt(), output ) : 0;
      }
      // ======================================================================
      static size_t __findDecay__( const Decays::Finder_<TYPE>& finder, const LHCb::HepMCEvent::Container* events,
                                   ConstVector& output ) {
        if ( !events ) { return 0; }
        return std::accumulate( events->begin(), events->end(), size_t{0},
                                [&]( size_t found, const LHCb::HepMCEvent* e ) {
                                  if ( e ) found += __findDecay__( finder, e, output );
                                  return found;
                                } );
      }
      // ======================================================================
      static size_t __findDecay__( const Decays::Finder_<TYPE>& finder, const HepMC::GenVertex* vertex,
                                   HepMC::IteratorRange range, ConstVector& output ) {
        if ( !vertex ) { return false; }
        HepMC::GenVertex* _vx = const_cast<HepMC::GenVertex*>( vertex );
        return finder.findDecay( _vx->particles_begin( range ), _vx->particles_end( range ), output );
      }
      // ======================================================================
    public:
      // ======================================================================
      static ConstVector __findDecay__( const Decays::Finder_<TYPE>& finder, const ConstVector& input ) {
        ConstVector output;
        __findDecay__( finder, input, output );
        return output;
      }
      // ======================================================================
      static ConstVector __findDecay__( const Decays::Finder_<TYPE>& finder, const HepMC::GenEvent* event ) {
        ConstVector output;
        __findDecay__( finder, event, output );
        return output;
      }
      // ======================================================================
      static ConstVector __findDecay__( const Decays::Finder_<TYPE>& finder, const LHCb::HepMCEvent* event ) {
        ConstVector output;
        __findDecay__( finder, event, output );
        return output;
      }
      // ======================================================================
      static ConstVector __findDecay__( const Decays::Finder_<TYPE>&       finder,
                                        const LHCb::HepMCEvent::Container* events ) {
        ConstVector output;
        __findDecay__( finder, events, output );
        return output;
      }
      // ======================================================================
      static ConstVector __findDecay__( const Decays::Finder_<TYPE>& finder, const HepMC::GenVertex* vertex,
                                        HepMC::IteratorRange range ) {
        ConstVector output;
        __findDecay__( finder, vertex, range, output );
        return output;
      }
      // ======================================================================
    };
    // ========================================================================
  } // namespace Dicts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace {
  // ==========================================================================
  struct _Instantiations {
    // ========================================================================
    /// fictive constructor
    _Instantiations();
    // ========================================================================
    // the basic types
    LoKi::Types::GRange  m_r1;
    LoKi::Types::GVRange m_r2;
    // rangelists:
    LoKi::Types::GRangeL  m_l1;
    LoKi::Types::GVRangeL m_l2;
    // keepers:
    LoKi::Keeper<HepMC::GenParticle> m_k1;
    LoKi::Keeper<HepMC::GenVertex>   m_k2;
    // unique keepers
    LoKi::UniqueKeeper<HepMC::GenParticle> m_u1;
    LoKi::UniqueKeeper<HepMC::GenVertex>   m_u2;
    // the basic functions
    LoKi::Dicts::Funcs<const HepMC::GenParticle*>  m_f1;
    LoKi::Dicts::Funcs<const HepMC::GenVertex*>    m_f2;
    LoKi::Dicts::VFuncs<const HepMC::GenParticle*> m_f3;
    LoKi::Dicts::VFuncs<const HepMC::GenVertex*>   m_f4;
    // operators
    LoKi::Dicts::FuncOps<const HepMC::GenParticle*> m_o1;
    LoKi::Dicts::FuncOps<const HepMC::GenVertex*>   m_o2;
    LoKi::Dicts::CutsOps<const HepMC::GenParticle*> m_o3;
    LoKi::Dicts::CutsOps<const HepMC::GenVertex*>   m_o4;
    // functional parts:
    LoKi::Dicts::MapsOps<const HepMC::GenParticle*>   m_fo1;
    LoKi::Dicts::PipeOps<const HepMC::GenParticle*>   m_fo2;
    LoKi::Dicts::FunValOps<const HepMC::GenParticle*> m_fo3;
    LoKi::Dicts::CutValOps<const HepMC::GenParticle*> m_fo31;
    LoKi::Dicts::MapsOps<const HepMC::GenVertex*>     m_fo5;
    LoKi::Dicts::PipeOps<const HepMC::GenVertex*>     m_fo6;
    LoKi::Dicts::FunValOps<const HepMC::GenVertex*>   m_fo7;
    LoKi::Dicts::CutValOps<const HepMC::GenVertex*>   m_fo71;
    LoKi::Dicts::SourceOps<const HepMC::GenVertex*>   m_fo9;
    LoKi::Dicts::SourceOps<const HepMC::GenParticle*> m_fo10;
    /// mathematics:
    LoKi::Dicts::FunCalls<HepMC::GenParticle> m_c1;
    LoKi::Dicts::FunCalls<HepMC::GenVertex>   m_c2;
    LoKi::Dicts::CutCalls<HepMC::GenParticle> m_c3;
    LoKi::Dicts::CutCalls<HepMC::GenVertex>   m_c4;
    /// the special operators for identifiers
    LoKi::Dicts::PIDOps<LoKi::GenParticles::Identifier>    m_i1;
    LoKi::Dicts::PIDOps<LoKi::GenParticles::AbsIdentifier> m_i2;
    /// same ?
    LoKi::TheSame<const HepMC::GenParticle*> m_s1;
    LoKi::TheSame<const HepMC::GenVertex*>   m_s2;
    /// trivia
    LoKi::Functors::Empty<const HepMC::GenParticle*> m_e1;
    LoKi::Functors::Size<const HepMC::GenParticle*>  m_si1;
    LoKi::Functors::Empty<const HepMC::GenVertex*>   m_e2;
    LoKi::Functors::Size<const HepMC::GenVertex*>    m_si2;
    // decay funders:
    Decays::Tree_<const HepMC::GenParticle*>          m_tree1;
    Decays::Trees::Any_<const HepMC::GenParticle*>    m_tree2;
    Decays::Trees::None_<const HepMC::GenParticle*>   m_tree3;
    Decays::Trees::Stable_<const HepMC::GenParticle*> m_tree4;
    LoKi::Dicts::TreeOps<const HepMC::GenParticle*>   m_trops;
    // ========================================================================
    // Decay Finder
    // ========================================================================
    Decays::IGenDecay::Finder                            m_finder;
    Decays::IGenDecay::Tree                              m_tree;
    LoKi::Dicts::FinderDicts_<const HepMC::GenParticle*> m_finderDicts;
    // ========================================================================
  };
  // ==========================================================================
} // end of anonymous namespace
// ============================================================================

// ============================================================================
// The END
// ============================================================================
#endif // LOKI_LOKIGEN_DCT_H

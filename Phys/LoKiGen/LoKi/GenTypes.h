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
#ifndef LOKI_GENTYPES_H
#define LOKI_GENTYPES_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/CoreTypes.h"
#include "LoKi/Functions.h"
#include "LoKi/RangeList.h"
#include "LoKi/Selected.h"
// ============================================================================
// GenEvent
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
// forward declarations
// ============================================================================
namespace HepMC {
  class GenParticle;
}
namespace HepMC {
  class GenVertex;
}
// ============================================================================
/** @file LoKi/GenTypes.h
 *
 *  Collection of useful function to extract the information from  HepMC
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
 *
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::GenTypes GenTypes.h LoKi/GenTypes.h
   *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
   *  @date   2005-03-27
   */
  namespace GenTypes {
    // ========================================================================
    /// the basic imported type for the sequence of HepMC-particles
    typedef std::vector<const HepMC::GenParticle*> GenContainer;
    /// the basic imported type for the sequence of HepMC-particles
    typedef GenContainer ConstVector;
    /// the basic imported type for the sequence of HepMC-vertices
    typedef std::vector<const HepMC::GenVertex*> GenVContainer;
    //
    typedef LoKi::Selected_<GenContainer>  GenSelected;
    typedef LoKi::Selected_<GenVContainer> GenVSelected;
    //
    /// the actual type of the range of HepMC-particles
    typedef GenSelected::Range GRange;
    /// the actual type of the range  of HepMC-vertices
    typedef GenVSelected::Range GVRange;
    /// type of 'cuts' for HepMC::GenParticle        (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::Predicate GCuts;
    /// type of 'functions' for HEpMC::GenParticle   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::Function GFunc;
    /// type of 'cuts' for HepMC::GenParticle        (assignable)
    typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate GCut;
    /// type of 'functions' for HepMC::GenParticle   (assignable)
    typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::FunctionFromFunction GFun;
    //
    /// type of 'cuts' for HepMC::GenVertex          (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenVertex*>::Predicate GVCuts;
    /// type of 'functions' for HepMC::GenVertex     (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenVertex*>::Function GVFunc;
    /// type of 'cuts' for HepMC::GenVertex          (assignable)
    typedef LoKi::BasicFunctors<const HepMC::GenVertex*>::PredicateFromPredicate GVCut;
    /// type of 'functions' for HepMC::GenVertex     (assignable)
    typedef LoKi::BasicFunctors<const HepMC::GenVertex*>::FunctionFromFunction GVFun;
    //
    // functional types for HepMC::GenParticle
    //
    // type of "map" for HepMC::GenParticle   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::Map GMaps;
    // type of "map" for HepMC::GenParticle   (assignable)
    typedef LoKi::Assignable_t<GMaps> GMap;
    // type of "pipe" for HepMC::GenParticle   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::Pipe GPipes;
    // type of "pipe" for HepMC::GenParticle   (assignable)
    typedef LoKi::Assignable_t<GPipes> GPipe;
    // type of "fun-val" for HepMC::GenParticle   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::FunVal GFunVals;
    // type of "fun-val" for HepMC::GenParticle   (assignable)
    typedef LoKi::Assignable_t<GFunVals> GFunVal;
    // type of "cut-val" for HepMC::GenParticle   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::CutVal GCutVals;
    // type of "cut-val" for HepMC::GenParticle   (assignable)
    typedef LoKi::Assignable_t<GCutVals> GCutVal;
    // type of "source" for HepMC::GenParticle   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::Source GSources;
    // type of "source" for HepMC::GenParticle   (assignable)
    typedef LoKi::Assignable_t<GSources> GSource;
    //
    // functional types for HepMC::GenVertex
    //
    // type of "map" for HepMCGenVertex   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenVertex*>::Map GVMaps;
    // type of "map" for HepMCGenVertex   (assignable)
    typedef LoKi::Assignable_t<GVMaps> GVMap;
    // type of "pipe" for HepMCGenVertex   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenVertex*>::Pipe GVPipes;
    // type of "pipe" for HepMCGenVertex   (assignable)
    typedef LoKi::Assignable_t<GVPipes> GVPipe;
    // type of "fun-val" for HepMCGenVertex   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenVertex*>::FunVal GVFunVals;
    // type of "fun-val" for HepMCGenVertex   (assignable)
    typedef LoKi::Assignable_t<GVFunVals> GVFunVal;
    // type of "cut-val" for HepMCGenVertex   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenVertex*>::CutVal GVCutVals;
    // type of "cut-val" for HepMCGenVertex   (assignable)
    typedef LoKi::Assignable_t<GVCutVals> GVCutVal;
    // type of "source" for HepMCGenVertex   (interface)
    typedef LoKi::BasicFunctors<const HepMC::GenVertex*>::Source GVSources;
    // type of "source" for HepMCGenVertex   (assignable)
    typedef LoKi::Assignable_t<GVSources> GVSource;
    //
    // ========================================================================
  } // end of namespace GenTypes
  // ==========================================================================
  namespace Types {
    // ========================================================================
    /// type of 'cuts' for HepMC::GenParticle       (interface)
    typedef LoKi::GenTypes::GCuts GCuts;
    /// type of 'functions' for HepMC::GenParticle  (interface)
    typedef LoKi::GenTypes::GFunc GFunc;
    /// type of 'cuts' for HepMC::GenParticle       (assignable)
    typedef LoKi::GenTypes::GCut GCut;
    /// type of 'functions' for HepMC::GenParticle  (assignable)
    typedef LoKi::GenTypes::GFun GFun;
    /// type of 'cuts' for HepMC::GenVertex       (interface)
    typedef LoKi::GenTypes::GVCuts GVCuts;
    /// type of 'functions' for HepMC::GenVertex  (interface)
    typedef LoKi::GenTypes::GVFunc GVFunc;
    /// type of 'cuts' for HepMC::GenVertex       (assignable)
    typedef LoKi::GenTypes::GVCut GVCut;
    /// type of 'functions' for HepMC::GenVertex  (assignable)
    typedef LoKi::GenTypes::GVFun GVFun;
    /// the actual type of the range of HepMC-particles
    typedef LoKi::GenTypes::GRange GRange;
    /// the actual type of the range of HepMC-vertices
    typedef LoKi::GenTypes::GVRange GVRange;
    /// Range list for HepMC-particles:
    typedef LoKi::RangeList_<GRange> GRangeL;
    /// Range list for HepMC-vertices
    typedef LoKi::RangeList_<GVRange> GVRangeL;
    //

    // functional types for HepMC::GenParticle

    // type of "map" for HepMC::GenParticle  (interface)
    typedef LoKi::GenTypes::GMaps GMaps;
    // type of "map" for HepMC::GenParticle  (assignable)
    typedef LoKi::GenTypes::GMap GMap;
    // type of "pipe" for HepMC::GenParticle  (interface)
    typedef LoKi::GenTypes::GPipes GPipes;
    // type of "pipe" for HepMC::GenParticle  (assignable)
    typedef LoKi::GenTypes::GPipe GPipe;
    // type of "fun-val" for HepMC::GenParticle  (interface)
    typedef LoKi::GenTypes::GFunVals GFunVals;
    // type of "fun-val" for HepMC::GenParticle  (assignable)
    typedef LoKi::GenTypes::GFunVal GFunVal;
    // type of "cut-val" for HepMC::GenParticle  (interface)
    typedef LoKi::GenTypes::GCutVals GCutVals;
    // type of "cut-val" for HepMC::GenParticle  (assignable)
    typedef LoKi::GenTypes::GCutVal GCutVal;
    // type of "source" for HepMC::GenParticle  (interface)
    typedef LoKi::GenTypes::GSources GSources;
    // type of "source" for HepMC::GenParticle  (assignable)
    typedef LoKi::GenTypes::GSource GSource;

    // functional types for HepMC::GenVertex

    // type of "map" for HepMC::GenVertex  (interface)
    typedef LoKi::GenTypes::GVMaps GVMaps;
    // type of "map" for HepMC::GenVertex  (assignable)
    typedef LoKi::GenTypes::GVMap GVMap;
    // type of "pipe" for HepMC::GenVertex  (interface)
    typedef LoKi::GenTypes::GVPipes GVPipes;
    // type of "pipe" for HepMC::GenVertex  (assignable)
    typedef LoKi::GenTypes::GVPipe GVPipe;
    // type of "fun-val" for HepMC::GenVertex  (interface)
    typedef LoKi::GenTypes::GVFunVals GVFunVals;
    // type of "fun-val" for HepMC::GenVertex  (assignable)
    typedef LoKi::GenTypes::GVFunVal GVFunVal;
    // type of "cut-val" for HepMC::GenVertex  (interface)
    typedef LoKi::GenTypes::GVCutVals GVCutVals;
    // type of "cut-val" for HepMC::GenVertex  (assignable)
    typedef LoKi::GenTypes::GVCutVal GVCutVal;
    // type of "source" for HepMC::GenVertex  (interface)
    typedef LoKi::GenTypes::GVSources GVSources;
    // type of "source" for HepMC::GenVertex  (assignable)
    typedef LoKi::GenTypes::GVSource GVSource;
    // ========================================================================
  } //                                                   end of namespace Types
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace LoKi {
  // ==========================================================================
  // the specialized printout
  // ==========================================================================
  template <>
  inline std::ostream& Constant<const HepMC::GenParticle*, bool>::fillStream( std::ostream& s ) const {
    return s << ( this->m_value ? "GALL" : "GNONE" );
  }
  // ==========================================================================
  // the specialized printout
  // ==========================================================================
  template <>
  inline std::ostream& Constant<const HepMC::GenVertex*, bool>::fillStream( std::ostream& s ) const {
    return s << ( this->m_value ? "GVALL" : "GVNONE" );
  }
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// a little bit of streamers
// ============================================================================
GAUDI_API
LoKi::GenTypes::GenContainer operator>>( const LoKi::GenTypes::GenContainer& input, const LoKi::Types::GCuts& cuts );
// ============================================================================
GAUDI_API
LoKi::GenTypes::GenContainer operator>>( const LoKi::GenTypes::GRange& input, const LoKi::Types::GCuts& cuts );
// ============================================================================
GAUDI_API
LoKi::GenTypes::GenContainer operator>>( const LHCb::HepMCEvent* event, const LoKi::Types::GCuts& cuts );
// ============================================================================
GAUDI_API
LoKi::GenTypes::GenContainer operator>>( const LHCb::HepMCEvent::Container* event, const LoKi::Types::GCuts& cuts );
// ============================================================================
GAUDI_API
LoKi::GenTypes::GenContainer operator>>( const HepMC::GenEvent* event, const LoKi::Types::GCuts& cuts );
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENTYPES_H

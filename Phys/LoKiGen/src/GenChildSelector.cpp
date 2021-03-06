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
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/GenAlgs.h"
#include "LoKi/GenChild.h"
#include "LoKi/GenChildSelector.h"
#include "LoKi/GenExtract.h"
#include "LoKi/GenParticles.h"
#include "LoKi/ILoKiSvc.h"
#include "LoKi/Services.h"
#include "LoKi/Trees.h"
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
 *  @author Vanya Belyaev  Ivan.Belyaev@cern.ch
 *  @date 2011-12-11
 *
 */
// ============================================================================
namespace {
  // ==========================================================================
  /// invalid decay
  const Decays::Trees::Types_<const HepMC::GenParticle*>::Invalid s_INVALID =
      Decays::Trees::Types_<const HepMC::GenParticle*>::Invalid();
  /// "None"-selector
  const LoKi::BasicFunctors<const HepMC::GenParticle*>::BooleanConstant s_NONE( false );
  /// "Factory"
  const std::string s_FACTORY = "LoKi::GenDecay";
  // ==========================================================================
} // namespace
// ============================================================================
// constructor from the index
// ============================================================================
LoKi::GenChild::Selector::Selector( const std::vector<unsigned int>& indices )
    : LoKi::AuxFunBase( std::tie( indices ) )
    , m_indices( indices )
    , m_finder( s_INVALID )
    , m_setCut( false )
    , m_cut( s_NONE ) {
  Assert( valid(), "The child selector is invalid" );
}
// ============================================================================
// constructor from decay tree
// ============================================================================
LoKi::GenChild::Selector::Selector( const Decays::IGenDecay::iTree& child )
    : LoKi::AuxFunBase(), m_finder( child ), m_setCut( false ), m_cut( s_NONE ) {
  //
  if ( !m_finder ) {
    LoKi::ILoKiSvc*                     ls = lokiSvc();
    SmartIF<LHCb::IParticlePropertySvc> pp( ls );
    const LHCb::IParticlePropertySvc*   ppsvc = pp;
    if ( 0 == ppsvc ) {
      const LoKi::Services& services = LoKi::Services::instance();
      ppsvc                          = services.ppSvc();
    }
    StatusCode sc = m_finder.validate( ppsvc );
    Assert( sc.isSuccess(), " Unable to validate the decay tree!" );
  }
  //
  Assert( valid(), "The child selector is invalid" );
}
// ============================================================================
// constructor from decay onde
// ============================================================================
LoKi::GenChild::Selector::Selector( const Decays::iNode& node )
    : LoKi::AuxFunBase(), m_finder( s_INVALID ), m_setCut( false ), m_cut( s_NONE ) {
  //
  m_cut    = LoKi::GenParticles::DecNode( node );
  m_setCut = true;
  //
  Assert( valid(), "The child selector is invalid" );
}
// ============================================================================
// constructor from decay tree
// ============================================================================
LoKi::GenChild::Selector::Selector( const Decays::IGenDecay::Finder& child )
    : LoKi::AuxFunBase(), m_finder( child ), m_setCut( false ), m_cut( s_NONE ) {
  //
  if ( !m_finder ) {
    LoKi::ILoKiSvc*                     ls = lokiSvc();
    SmartIF<LHCb::IParticlePropertySvc> pp( ls );
    const LHCb::IParticlePropertySvc*   ppsvc = pp;
    if ( 0 == ppsvc ) {
      const LoKi::Services& services = LoKi::Services::instance();
      ppsvc                          = services.ppSvc();
    }
    StatusCode sc = m_finder.validate( ppsvc );
    Assert( sc.isSuccess(), " Unable to validate the decay tree!" );
  }
  //
  Assert( valid(), "The child selector is invalid" );
}
// ============================================================================
// constructor from the cut
// ============================================================================
LoKi::GenChild::Selector::Selector( const LoKi::Types::GCuts& cut )
    : LoKi::AuxFunBase( std::tie( cut ) ), m_indices(), m_finder( s_INVALID ), m_setCut( true ), m_cut( cut ) {}
// ============================================================================
// constructor from decay desctriptor
// ============================================================================
LoKi::GenChild::Selector::Selector( const std::string& child )
    : LoKi::AuxFunBase( std::tie( child ) ), m_indices(), m_finder( s_INVALID ), m_setCut( false ), m_cut( s_NONE ) {
  StatusCode sc = buildTree( child, s_FACTORY );
  Assert( sc.isSuccess(), "Unable to decode descriptor '" + child + "'", sc );
  Assert( valid(), "The child selector is invalid" );
}
// ============================================================================
// constructor from decay desctriptor & factory
// ============================================================================
LoKi::GenChild::Selector::Selector( const std::string& child, const std::string& factory )
    : LoKi::AuxFunBase( std::tie( child, factory ) )
    , m_indices()
    , m_finder( s_INVALID )
    , m_setCut( false )
    , m_cut( s_NONE ) {
  StatusCode sc = buildTree( child, factory );
  Assert( sc.isSuccess(), "Unable to decode descriptor '" + child + "'", sc );
  Assert( valid(), "The child selector is invalid" );
}
// ============================================================================
// build the decay tree form the descriptor
// ============================================================================
StatusCode LoKi::GenChild::Selector::buildTree( const std::string& descriptor, const std::string& factory ) {
  LoKi::ILoKiSvc*   ls = lokiSvc();
  SmartIF<IToolSvc> toolSvc( ls );
  if ( !toolSvc ) { return Error( "getTree: unable to locate tool service" ); }

  Decays::IGenDecay* tool = nullptr;
  StatusCode         sc   = toolSvc->retrieveTool( factory, tool );
  if ( sc.isFailure() ) { return Error( "getTree: Unable to retrieve '" + factory + "'", sc ); }
  if ( 0 == tool ) { return Error( "getTree: Decays::IGenDecay* points to NULL" ); }
  //
  m_finder = tool->tree( descriptor );
  //
  toolSvc->releaseTool( tool ); // do not need the tool anymore
  //
  if ( !m_finder ) { return Error( "getTree: the tree is invalid:  '" + descriptor + "'" ); }
  if ( !m_finder.marked() ) { Warning( "getTree: the tree is unmarked: '" + descriptor + "'" ); }
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
// check the validity the child-selector
// ============================================================================
bool LoKi::GenChild::Selector::valid() const { return !m_indices.empty() || m_finder.valid() || m_setCut; }
// ============================================================================

// ============================================================================
// try to validate it!
// ============================================================================
StatusCode LoKi::GenChild::Selector::validate( const LHCb::IParticlePropertySvc* svc ) const {
  if ( !m_indices.empty() || m_setCut ) { return StatusCode::SUCCESS; }
  //
  StatusCode sc = m_finder.validate( svc );
  if ( sc.isFailure() ) { return Error( "Unable to validate the tree/decay descriptor", sc ); }
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::GenChild::Selector::fillStream( std::ostream& s ) const {
  //
  if ( !m_indices.empty() ) { return s << " " << Gaudi::Utils::toString( m_indices ) << " "; }
  //
  if ( m_setCut ) { return s << " " << m_cut << " "; } // REUTRN
  //
  return s << " " << m_finder.tree() << " ";
}
// ============================================================================
/*  get the child according to the selector
 *  @param mother    (INPUT) the head fo the decay tree
 *  @param daughetrs (UPDATE)
 *  @return unmber of particles
 */
// ============================================================================
unsigned int LoKi::GenChild::Selector::children( const HepMC::GenParticle*    head,
                                                 LoKi::GenTypes::ConstVector& daughters ) const {
  if ( 0 == head ) {
    Error( "children: HepMC::GenParticle* points to NULL" );
    return 0; // RETURN
  }
  //
  Assert( valid(), "Selector is invalid!" );
  //
  if ( !m_indices.empty() ) {
    const HepMC::GenParticle* c = LoKi::GenChild::child( head, m_indices );
    if ( 0 == c ) { return 0; } // RETURN
    //
    daughters.push_back( c );
    return daughters.size(); // RETURN
  } else if ( m_setCut ) {
    LoKi::Extract::genParticles( head, std::back_inserter( daughters ), m_cut );
    return daughters.size(); // RETURN
  } else {
    LoKi::GenTypes::ConstVector input( 1, head );
    return m_finder.findDecay( input, daughters ); // RETURN
  }
}
// ============================================================================
/*  get the child according to the selector
 *  @param  head (INPUT) the head fo the decay tree
 *  @return child particle
 */
// ============================================================================
const HepMC::GenParticle* LoKi::GenChild::Selector::child( const HepMC::GenParticle* head ) const {
  //
  if ( 0 == head ) {
    Error( "child: HepMC::GenParticle points to NULL" );
    return 0;
  }
  //
  Assert( valid(), "Selector is invalid!" );
  //
  if ( !m_indices.empty() ) { return LoKi::GenChild::child( head, m_indices ); }
  //
  if ( m_setCut ) { return LoKi::GenAlgs::foundFirst( head, m_cut ); }
  //
  LoKi::GenTypes::ConstVector daughters;
  LoKi::GenTypes::ConstVector input( 1, head );
  //
  m_finder.findDecay( input, daughters );
  //
  if ( daughters.empty() ) { return 0; } // REUTRN
  //
  if ( 1 < daughters.size() ) { Warning( "child: >1 daughter particles are found, return the first" ); }
  //
  return daughters[0];
}
// ============================================================================
/* Trivial accessor to the daughter particles for the given particle.
 *  @param  particle (const) pointer to mother particle
 *  @param  selector the selector
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2012-01-26
 */
// ============================================================================
const HepMC::GenParticle* LoKi::GenChild::child( const HepMC::GenParticle*       particle,
                                                 const LoKi::GenChild::Selector& selector ) {
  if ( 0 == particle ) { return 0; } // RETURN
  return selector.child( particle );
}
// ===========================================================================
/*  accessor to certain children particles for the given particle
 *  @param  particle (INPUT) pointer to mother particle
 *  @param  selector (INPUT) the selector
 *  @param  result   (OUTPUT) the container of found particles
 *  @return number of found particles
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2012-01-26
 */
// ===========================================================================
unsigned int LoKi::GenChild::children( const HepMC::GenParticle* particle, const LoKi::GenChild::Selector& selector,
                                       LoKi::GenTypes::ConstVector& daughters ) {
  if ( 0 == particle ) { return 0; } // RETURN
  return selector.children( particle, daughters );
}
// ============================================================================
/*  accessor to certain children particles for the given particle
 *  @param  particle (INPUT) pointer to mother particle
 *  @param  selector (INPUT) the selector
 *  @return the container of found particles
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2010-05-29
 */
// ============================================================================
LoKi::GenTypes::ConstVector LoKi::GenChild::children( const HepMC::GenParticle*       particle,
                                                      const LoKi::GenChild::Selector& selector ) {
  //
  LoKi::GenTypes::ConstVector result;
  if ( 0 == particle ) { return result; }
  //
  selector.children( particle, result );
  //
  return result;
}
// ===========================================================================
/** Trivial accessor to the daughter particles for the given particle.
 *  @param  particle (const) pointer to mother particle
 *  @param  dscriptor
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2016-06-21
 */
// ===========================================================================
const HepMC::GenParticle* LoKi::GenChild::child( const HepMC::GenParticle* particle, const std::string& descriptor ) {
  return nullptr == particle ? nullptr : child( particle, LoKi::GenChild::Selector( descriptor ) );
}
// ===========================================================================
/* Trivial accessor to the daughter particles for the given particle.
 *  @param  particle (const) pointer to mother particle
 *  @param  cuts     (INPUT) cuts to be used
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2016-06-21
 */
const HepMC::GenParticle* LoKi::GenChild::child( const HepMC::GenParticle* particle, const LoKi::Types::GCuts& cuts ) {
  return nullptr == particle ? nullptr : child( particle, LoKi::GenChild::Selector( cuts ) );
}
// ========================================================================
/* accessor to certain children particles for the given particle
 *  @param  particle   (INPUT) pointer to mother particle
 *  @param  descriptor (INPUT) the descriptor
 *  @return the container of found particles
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2016-06-21
 */
// ========================================================================
LoKi::GenTypes::ConstVector LoKi::GenChild::children( const HepMC::GenParticle* particle,
                                                      const std::string&        descriptor ) {
  return nullptr == particle ? LoKi::GenTypes::ConstVector()
                             : children( particle, LoKi::GenChild::Selector( descriptor ) );
}
// ========================================================================
/* accessor to certain children particles for the given particle
 *  @param  particle (INPUT) pointer to mother particle
 *  @param  cuts     (INPUT) cuts to be used
 *  @return the container of found particles
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2016-06-21
 */
// ========================================================================
LoKi::GenTypes::ConstVector LoKi::GenChild::children( const HepMC::GenParticle* particle,
                                                      const LoKi::Types::GCuts& cuts ) {
  return nullptr == particle ? LoKi::GenTypes::ConstVector() : children( particle, LoKi::GenChild::Selector( cuts ) );
}
// ========================================================================

// ============================================================================
// The END
// ============================================================================

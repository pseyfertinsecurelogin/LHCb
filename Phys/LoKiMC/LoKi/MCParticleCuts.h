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
#ifndef LOKI_MCPARTICLECUTS_H
#define LOKI_MCPARTICLECUTS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Monitoring.h"
#include "LoKi/MCTypes.h"
#include "LoKi/MCParticles.h"
#include "LoKi/MCParticles1.h"
#include "LoKi/MCParticles2.h"
#include "LoKi/MCParticles3.h"
#include "LoKi/MCPIDOperators.h"
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
  // ==========================================================================
  namespace Cuts
  {
    // ========================================================================
    /** @var BARYON
     *  Check for particle type
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool baryon  = BARYON  ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::IsBaryon
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto BARYON = LoKi::MCParticles::IsBaryon{};
    // ========================================================================
    /** @var BEAUTY
     *  Check for bottom quark
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool beauty  = BEAUTY ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::HasQuark
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto BEAUTY = LoKi::MCParticles::HasQuark{ LHCb::ParticleID::bottom } ;
    // ========================================================================
    /** @var CHARGED
     *  Check for particle charge
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool charged = CHARGED ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::IsCharged
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto CHARGED = LoKi::MCParticles::IsCharged{};
    // ========================================================================
    /** @var CHARM
     *  Check for charm quark
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool charm  = CHARM ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::HasQuark
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const  LoKi::MCParticles::HasQuark     CHARM ( LHCb::ParticleID::charm  ) ;
    // ========================================================================
    /** @typedef FROMMCTREE
     *  siple predicate wich evaluates if MC particle comes from decay tree of
     *  (an)other MC particle(s)
     *
     * @code
     *
     * const MCParticle* p = .... ;
     * const MCParticle* parent = ... ;
     *
     *  MCFun fun = FROMMCTREE( parent ) ;
     *
     *  const bool fromTree = fun( p ) ;
     *
     * @endcode
     *
     * @see MCParticle
     * @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     * @date 2004-07-07
     */
    typedef LoKi::MCParticles::FromMCDecayTree                     FROMMCTREE ;
    // ========================================================================
    /** @var HADRON
     *  Check for particle type
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool hadron  = HADRON  ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::IsHadron
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto HADRON = LoKi::MCParticles::IsHadron{};
    // ========================================================================
    /** @typedef ISMCCO
     *  Trivial predicate to check the identity of a particle
     *  and ContainedObject
     *
     *  @see LoKi::MCParticles::IsContainedObject
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-02-02
     */
    typedef LoKi::MCParticles::IsContainedObject                       ISMCCO ;
    // ========================================================================
    /** @typedef ISMCPART
     *  Trivial predicate to check the identity of a particle
     *  and another particle
     *
     *  @see LoKi::MCParticles::IsParticle
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-02-02
     */
    typedef LoKi::MCParticles::IsParticle                            ISMCPART ;
    // ========================================================================
    /** @var LEPTON
     *  Check for particle type
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool lepton  = LEPTON  ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::IsLepton
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto LEPTON = LoKi::MCParticles::IsLepton{};
    // ========================================================================
    /** @var MC3Q
     *  MCParticle three charge
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      q  = MC3Q (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::ThreeCharge
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MC3Q = LoKi::MCParticles::ThreeCharge{};
    // ========================================================================
    /** @var MCABSID
     *  abs( MCParticle ID )
     *
     *  @code
     *
     *   const MCParticle* p  = ...            ;
     *   const double      id = MCABSID (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::AbsIdentifier
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCABSID = LoKi::MCParticles::AbsIdentifier{};
    // ========================================================================
    /** @var MCALL
     *  primitive predicate, "always true"
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-27
     */
    const auto MCALL = LoKi::Constant<const LHCb::MCParticle*,bool>{ true } ;
    // ========================================================================
    /** @typedef MCCHILD
     *  Simple function which delegates the evaluation of
     *  another function to a daughter "decay" MC-particle
     *
     *  @code
     *
     *   const LHCb::MCParticle* phi = ... ;
     *
     *   // construct the fuction, which evaluates to
     *       PT of the first decy daughter:
     *   MCFun pt1 = MCCHILD ( MCPT , 1 ) ;
     *
     *   // evaluate the PT of the first decay daughter:
     *
     *   const double value = pt1 ( phi ) ;
     *
     *  @endcode
     *
     *  @attention index starts form 1,
     *   null-value corresponds to the particle itself
     *
     *  @see LoKi::Cuts::MCPT
     *  @see LoKi::Child::child
     *  @see LoKi::MCParticles::ChildFunction
     *  @see LoKi::Cuts::MCCHILDFUN
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::ChildFunction                          MCCHILD ;
    // ========================================================================
    /** @typedef MCCHILDCUT
     *  Simple predicate which delegates the evaluation of
     *  another function to a daughter "decay" MC-particle
     *
     *  @code
     *
     *   const LHCb::MCParticle* phi = ... ;
     *
     *   // construct the function, which checks PT of the first child
     *   MCCut pt1 = MCCHILDFUN ( MCPT > 1 * GeV  , 1 ) ;
     *
     *   // check pt of th efirst daughter particle:
     *
     *   const bool large  = pt1 ( phi ) ;
     *
     *  @endcode
     *
     *  @attention index starts form 1,
     *   null-value corresponds to the particle itself
     *
     *  @see LoKi::Cuts::MCPT
     *  @see LoKi::Child::child
     *  @see LoKi::MCParticles::ChildPredicate
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::ChildPredicate                      MCCHILDCUT ;
    // ========================================================================
    /** @typedef MCCHILDFUN
     *  Simple function which delegates the evaluation of
     *  another function to a daughter "decay" MC-particle
     *
     *  @code
     *
     *   const LHCb::MCParticle* phi = ... ;
     *
     *   // construct the fuction, which evaluates to
     *   // PT of the first decy daughter:
     *   MCFun pt1 = MCCHILDFUN ( MCPT , 1 ) ;
     *
     *   // evaluate the PT of th efirst decay daughter:
     *
     *   const double value = pt1 ( phi ) ;
     *
     *  @endcode
     *
     *  @attention index starts form 1,
     *   null-value corresponds to the particle itself
     *
     *  @see LoKi::Cuts::MCPT
     *  @see LoKi::Child::child
     *  @see LoKi::MCParticles::ChildFunction
     *  @see LoKi::Cuts::MCCHILD
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::ChildFunction                       MCCHILDFUN ;
    // ========================================================================
    /** @typedef MCCOUNTER
     *  Simple monitoring counter for predicates
     *
     *  @code
     *
     *  // some predicate to be monitored
     *  MCCut cut = ... ;
     *
     *  // Create the monitored predicate using the generic counter
     *  // either local counter, or the counter from IStatSvc
     *  StatEntity* counter = ... ; ///< the generic counter
     *  MCCut mon = MCCOUNTER ( cut , counter ) ;
     *
     *  for ( ... )
     *    {
     *      ...
     *      const LHCb::Particle* p = ... ;
     *      const bool result = mon( p ) ; ///< use the predicate
     *      ...
     *    }
     *
     *  ...
     *  info ()
     *     << " Monitoring results : "                       << endmsg
     *     << " NEntries:  #" << counter->entries ()         << endmsg
     *     << " Efficiency:(" << counter->eff     ()
     *     << "+="            << counter->effErr  () << ")%" << endmsg ;
     *
     *  @endcode
     *
     *  The alternative (recommended) way to create the monitored predicate:
     *  @code
     *
     *  MCCut cut = ... ;
     *
     *  MCCut mon = monitor ( cut , counter("SomeEffCounter") ) ;
     *
     *  @endcode
     *
     *  The substitution of the predicate by monitored predicate
     *  cound be done "on-flight" without the disturbing of the
     *  actual processing:
     *  @code
     *
     *  // some predicate to be monitored
     *  MCCut cut = ... ;
     *
     *  if ( monitoring )
     *    {
     *       cut = monitor ( cut , "Efficiency1" ) ;
     *    }
     *
     *  @endcode
     *
     *  @attention The string representation of the object
     *             is delegated to the underlying prediate,
     *             therefore the object is NOT recontructable
     *             from its string representations. It is done
     *             on purpose to avoid the disturbing of ids.
     *
     *  @see LoKi::Monitoring::Counter
     *  @see LoKi::monitor
     *  @see StatEntity
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    typedef LoKi::Monitoring::Counter<const LHCb::MCParticle*,bool>  MCCOUNTER ;
    // ========================================================================
    /** @var MCCTAU
     *  MCParticle proper lifetime (in c*tau units)
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      t  = MCCTAU ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::ProperLifeTime
     *  @see LoKi::Cuts::MCTIME
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCCTAU = LoKi::MCParticles::ProperLifeTime{};
    // ========================================================================
    /** @typedef MCDECAY
     *  Simple predicate to test certain decay pattern, using
     *  nice tool IMCDecayFinder, developed by Olivier DORMOND
     *
     *  The idea of this predicate belongs to Luis FERNANDEZ
     *
     *  @code
     *
     *   // create the predicate which evaluated to true
     *   // for B0 -> + pi- and all its ancestors
     *   MCCut b2pp = MCDECAY ( "B0 -> pi+ pi-"   ,
     *                          mcDecayFinder () ) ;
     *
     *   // use this predicate :
     *  MCRange B0 = mcselect ( "B0" , b2pp && "B0" == MCID ) ;
     *
     *  @endcode
     *
     *  @attention The "decay" string of IMCDecayFinder tool is redefined!
     *
     *  @see IMCDecayFinder
     *  @see LoKi::MCParticles::MCDecayPattern
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
     *  @date 2005-05-16
     */
    typedef LoKi::MCParticles::MCDecayPattern                         MCDECAY ;
    // ========================================================================
    /** @typedef MCDELTAR2
     *  simple evaluator of "delta eta" of the particle momenta
     *
     *  @code
     *
     *   double et
     *
     *   // create the function:
     *  MCFun r2 = MCDELTAR2( v ) ;
     *
     *  const LHCb::MCParticle* p = ... ;
     *
     *  // use the function
     *  const double result = r2 ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCParticless::DeltaR2
     *  @see LoKi::MCParticless::DeltaEta
     *  @see LoKi::MCParticless::DeltaPhi
     *  @see LoKi::Cuts::MCDR2
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    typedef LoKi::MCParticles::DeltaR2                              MCDELTAR2 ;
    // ========================================================================
    /** @typedef MCDETA
     *  simple evaluator of "delta eta" of the particle momenta
     *
     *  @code
     *
     *   double eta = ... ;
     *
     *   // create the function:
     *  MCFun deta = MCDETA(eta) ;
     *
     *  const LHCb::MCParticle* p = ... ;
     *
     *  // use the function
     *  const double result = deta ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCParticless::DeltaEta
     *  @see LoKi::MCParticless::Eta
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    typedef LoKi::MCParticles::DeltaEta                                MCDETA ;
    // ========================================================================
    /** @typedef MCDPHI
     *  simple evaluator of "delta phi" of the particle momenta
     *
     *  @code
     *
     *   double phi = ... ;
     *
     *   // create the function:
     *  MCFun dphi = GMCDPHI(phi) ;
     *
     *  const LHCb::MCParticle* p = ... ;
     *
     *  // use the function
     *  const double result = dphi ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCParticless::DeltaPhi
     *  @see LoKi::MCParticless::Phi
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    typedef LoKi::MCParticles::DeltaPhi                                MCDPHI ;
    // ========================================================================
    /** @typedef MCDR2
     *  simple evaluator of "delta eta" of the particle momenta
     *
     *  @code
     *
     *   double et
     *
     *   // create the function:
     *  MCFun r2 = MCDR2( v ) ;
     *
     *  const LHCb::MCParticle* p = ... ;
     *
     *  // use the function
     *  const double result = r2 ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCParticless::DeltaR2
     *  @see LoKi::MCParticless::DeltaEta
     *  @see LoKi::MCParticless::DeltaPhi
     *  @see LoKi::Cuts::MCDELTAR2
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    typedef LoKi::MCParticles::DeltaR2                                  MCDR2 ;
    // ========================================================================
    /** @var MCE
     *  MCParticle energy
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      e  = MCE  (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::Energy
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCE = LoKi::MCParticles::Energy{};
    // =========================================================================
    /** @var MCFALSE
     *  primitive predicate, "always false"
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-27
     */
    const auto MCFALSE = LoKi::Constant<const LHCb::MCParticle*,bool>{ false } ;
    // =========================================================================
    /** @var MCETA
     *  MCParticle pseudorapidity
     *
     *  @code
     *
     *   const MCParticle* p    = ...         ;
     *   const double      eta  = MCETA(  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::PseudoRapidity
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCETA = LoKi::MCParticles::PseudoRapidity{};
    // ========================================================================
    /** @var MCFROMDECAYS
     *  Simple predicate to check if particle comes from "decays"
     *  using LHCb::MCVertex::MCVertexType
     *
     *  @see LHCb::MCVertex
     *  @see LHCb::MCVertex::MCVertexType
     *
     *  The functor return true, if all valid "mother" vertices
     *  are of type:
     *
     *     - LHCb::MCVertex::DecayVertex and
     *     - LHCb::MCVertex::OscillatedAndDecay
     *     - LHCb::MCVertex::ppCollision
     *
     *  @code
     *
     *  const LHCb::MCParticle* mcp = ... ;
     *
     *  const bool fromDecays = MCFROMDECAYS ( mcp ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCParticles::FromDecays
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-18
     */
    const auto MCFROMDECAYS = LoKi::MCParticles::FromDecays{};
    // ========================================================================
    /** @var MCFROMXS
     *  Simple predicate to check if particle comes from "interactions"
     *  using LHCb::MCVertex::MCVertexType
     *
     *  @see LHCb::MCVertex
     *  @see LHCb::MCVertex::MCVertexType
     *
     *  The functor return true, if at least one 'mother' vertex
     *  is not of the type:
     *
     *     - LHCb::MCVertex::DecayVertex and
     *     - LHCb::MCVertex::OscillatedAndDecay
     *     - LHCb::MCVertex::ppCollision
     *
     *  @code
     *
     *  const LHCb::MCParticle* mcp = ... ;
     *
     *  const bool fromXs = MCFROMXS ( mcp ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCParticles::FromInteractions
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-18
     */
    const auto MCFROMXS = LoKi::MCParticles::FromInteractions{};
    // ========================================================================
    /** @var MCFROMSIGNAL
     *  Is this partiicle is a SIGNAL particle?
     *  @code
     *  const LHCb::MCParticle* p = ... ;
     *  const bool signal = MCFROMSIGNAL ( p ) ;
     *  if ( signal) { ... } 
     *  @endcode 
     *  @see LHCb::MCParticle
     *  @see LHCb::MCParticle::fromSignal
     *  @see LoKi::MCParticles::FromSignal
     *  @see LoKi::MCParticles::Signal
     *  @see LoKi::CUTS::MCSIGNAL
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2016-11-17
     */
    const LoKi::MCParticles::FromSignal                       MCFROMSIGNAL {} ;
    // ========================================================================
    /** @var MCID
     *  MCParticle ID
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      id = MCID (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::Identifier
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCID = LoKi::MCParticles::Identifier{};
    // ========================================================================
    /** @typedef MCINANCESTORS
     *  Simple predicate to chck the presence of certain criteria
     *  within ancestors
     *
     *  @code
     *
     *   const MCINANCESTOS fromBeauty = MCINANCESTOS  ( BEAUTY )  ;
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool ok = fromBeauty ( p ) ;
     *
     *  @endcode
     *  @see LoKi::MCParticles::InAncestors
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-16
     */
    typedef LoKi::MCParticles::InAncestors                      MCINANCESTORS ;
    // ========================================================================
    /** @typedef MCINTREE
     *  Simple predicate to check the presence of a certain
     *  particle in the decay tree
     *  @see LoKi::MCParticles::InTree
     *  @see LoKi::MCAlgs::found
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::InTree                                MCINTREE ;
    // ========================================================================
    /** @var MCM
     *  MCParticle mass
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      v  = MCM  ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::MCM0
     *  @see LoKi::Cuts::MCMASS
     *  @see MCParticle
     *  @see LoKi::MCParticles::Mass
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCM = LoKi::MCParticles::Mass{};
    // ========================================================================
    /** @var MCM0
     *  MCParticle mass
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      v  = MCM0 ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::MCM
     *  @see LoKi::Cuts::MCMASS
     *  @see MCParticle
     *  @see LoKi::MCParticles::Mass
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCM0 = LoKi::MCParticles::Mass{};
    // ========================================================================
    /** @var MCMASS
     *  MCParticle mass
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      v  = MCMASS ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::MCM
     *  @see LoKi::Cuts::MCM0
     *  @see MCParticle
     *  @see LoKi::MCParticles::Mass
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCMASS = LoKi::MCParticles::Mass{};
    // ========================================================================
    /** @typedef MCMAXTREE
     *  Simple function to find a maximum value in the decay tree
     *  @see LoKi::MCParticles::MaxTree
     *  @see LoKi::MCAlgs::min_value
     *  @see LoKi::Cuts::MCTREEMAX
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::MaxTree                              MCMAXTREE ;
    // ========================================================================
    /** @typedef MCMINTREE
     *  Simple function to find a minimum value in the decay tree
     *  @see LoKi::MCParticles::MinTree
     *  @see LoKi::MCAlgs::min_value
     *  @see LoKi::Suts::MCTREEMIN
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::MinTree                              MCMINTREE ;
    // ========================================================================
    /** @typedef MCMOMDIST
     *  the evaluator of euclidian distance with
     *  respect to the given 4-momentum, useful
     *  e.g. for trivial "kinematical matching".
     *  One can find
     *  the particle with "closest" 4-momentum
     *  to the given one:
     *
     *  @code
     *
     *  SEQUENCE            particles = ... ;
     *  LoKi::LorentzVector vct         = ... ;
     *
     *  // find "best match"
     *  SEQUENCE::const_iterator imin =
     *    LoKi::select_min( particles.begin () ,
     *                      particles.end   () ,
     *                      MCMOMDIST ( vct )    ) ;
     *
     *  @endcode
     *
     *  @see LoKi::LorentzVector
     *  @see LoKi::MCParticles::MomentumDistance
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-23
     */
    typedef LoKi::MCParticles::MomentumDistance                     MCMOMDIST ;
    // ========================================================================
    /** @typedef MCMOTHCUT
     *  simple adapter function which delegates the
     *  evaluation of 'main' predicate to the mother
     *  particle
     *
     *  @code
     *
     *   const MCParticle* p = ... ;
     *
     *   // check if mother is beauty particle
     *   MCCut cut = MCMOHCUT( BEAUTY , false ) ;
     *
     *   bool fromBeauty  = cut( p ) ;
     *
     *  @endcode
     */
    typedef LoKi::MCParticles::MCMotherPredicate                    MCMOTHCUT ;
    // ========================================================================
    /** @typedef MCMOTHER
     *  simple adapter function whcih delegates the
     *  evaluation of 'main' function to the mother particle
     *
     *  @code
     *
     *   const MCParticle* p = ... ;
     *
     *   // mc ID for mother particle
     *   MCFun fun = MCMOTHER( MCABSID , -1000 ) ;
     *
     *   double mothID = fun( p ) ;
     *
     *  @endcode
     *  @see LoKi::Cuts::MCMOTHERFUN
     */
    typedef LoKi::MCParticles::MCMotherFunction                      MCMOTHER ;
    // ========================================================================
    /** @typedef MCMOTHERFUN
     *  simple adapter function whcih delegates the
     *  evaluation of 'main' function to the mother particle
     *
     *  @code
     *
     *   const MCParticle* p = ... ;
     *
     *   // mc ID for mother particle
     *   MCFun fun = MCMOTHER( MCABSID , -1000 ) ;
     *
     *   double mothID = fun( p ) ;
     *
     *  @endcode
     *  @see LoKi::Cuts::MCMOTHER
     */
    typedef LoKi::MCParticles::MCMotherFunction                   MCMOTHERFUN ;
    // ========================================================================
    /** @typedef MCMULTTREE
     *  Simple function to accumulate certain  values in the decay tree
     *  @see LoKi::MCParticles::MultTree
     *  @see LoKi::MCAlgs::accumulate
     *  @see LoKi::Cuts::MCTREEMULT
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::MultTree                            MCMULTTREE ;
    // ========================================================================
    /** @typedef MCNINANCESTORS
     *  Simple function to count the presence of certain criteria
     *  within ancestors
     *
     *  @code
     *
     *   const MCINANCESTOS fromBeauty = MCINANCESTOS  ( BEAUTY )  ;
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const double num = fromBeauty ( p ) ;
     *
     *  @endcode
     *  @see LoKi::MCParticles::NinAncestors
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-16
     */
    typedef LoKi::MCParticles::NinAncestors                    MCNINANCESTORS ;
    // ========================================================================
    /** @typedef MCNINTREE
     *  Simple function to count a certain particles in the decay tree
     *  @see LoKi::MCParticles::NinTree
     *  @see LoKi::MCAlgs::count_if
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::NinTree                              MCNINTREE ;
    // ========================================================================
    /** @var MCNONE
     *  primitive predicate, "always false"
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-27
     */
    const auto MCNONE = LoKi::Constant<const LHCb::MCParticle*,bool>{ false } ;
    // ========================================================================
    /** @var MCONE
     *  primitive function, "always 1"
     *  @see LoKi::Constant
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-27
     */
    const auto MCONE = LoKi::Constant<const LHCb::MCParticle*,double>{ 1 } ;
    // ========================================================================
    /** @var MCOSCILLATED
     *
     *  It evaluated to 'true' for scilalted particles
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const bool oscillated = MCOSCILLATED (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::Oscillated
     *
     *  @author Vanya Belyaev ibelyaev@physics.syr.edu
     *  @date   2006-02-05
     */
    const auto MCOSCILLATED = LoKi::MCParticles::Oscillated{};
    // ========================================================================
    /** @var MCOVALID
     *  trivial predicate which evaluated to true for
     *  MC particlres with valid "origin" vertex
     *  @see LoKi::MCParticles::VaildOrigin
     *  @author Vanya BELYAEV Ivanb.Belyaev@lapp.in2p3.fr
     *  @date 2005-05-16
     */
    const auto MCOVALID = LoKi::MCParticles::ValidOrigin{};
    // ========================================================================
    /** @var MCP
     *  MCParticle momentum
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      m  = MCP  (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::Momentum
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCP = LoKi::MCParticles::Momentum{};
    // ========================================================================
    /** @var MCP0
     *  MCParticle momentum
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      m  = MCP  (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::Momentum
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCP0 = LoKi::MCParticles::Momentum{};
    // ========================================================================
    /** @var MCPHI
     *  MCParticle asymuthal angle
     *
     *  @code
     *
     *   const MCParticle* p    = ...         ;
     *   const double      phi  = MCPHI(  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::Phi
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCPHI = LoKi::MCParticles::Phi{};
    // ========================================================================
    /** @typedef MCPLOT
     *  Simple monitoring histogram for the functions
     *
     *  @code
     *
     *  // some function to be monitored
     *  MCFun fun = ... ;
     *
     *  // Create the monitored function using the histogram:
     *  AIDA::IHistogram1D* histo = ... ;
     *  MCFun mon = PLOT ( fun , histo ) ;
     *
     *  for ( ... )
     *    {
     *      ...
     *      const LHCb::Particle* p = ... ;
     *      const double result = mon( p ) ; ///< use the function
     *      ...
     *    }
     *
     *  @endcode
     *
     *  The alternative (recommended) way to create the monitored function
     *  @code
     *
     *  // some function to be monitored
     *  MCFun fun = ... ;
     *
     *  AIDA::IHistogram1D* histo = ... ;
     *  MCFun mon = monitor ( fun , histo ) ;
     *
     *  @endcode
     *
     *  The substitution of the function by monitored function
     *  cound be done "on-flight" without the disturbing of the
     *  actual processing:
     *  @code
     *
     *  // some function to be monitored
     *  MCFun fun = ... ;
     *
     *  if ( monitoring )
     *    {
     *       AIDA::IHistogram1D* histo = ... ;
     *       fun = monitor ( fun , histo ) ;
     *    }
     *
     *  @endcode
     *
     *  @attention The string representation of the object
     *             is delegated to the underlying function,
     *             therefore the object is NOT recontructable
     *             from its string representations. It is done
     *             on purpose to avoid the disturbing of ids.
     *
     *  @see LoKi::Monitoring::Plot
     *  @see LoKi::monitor
     *  @see AIDA::IHistogram1D
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    typedef LoKi::Monitoring::Plot<const LHCb::MCParticle*,double>    MCPLOT ;
    // ========================================================================
    /** @var MCPT
     *  MCParticle transverse momentum
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      v  = MCPT (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::Cuts::MCPT0
     *  @see LoKi::MCParticles::TransverseMomentum
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCPT = LoKi::MCParticles::TransverseMomentum{};
    // ========================================================================
    /** @var MCPT0
     *  MCParticle transverse momentum
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      v  = MCPT (  p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::MCPT
     *  @see MCParticle
     *  @see LoKi::MCParticles::TransverseMomentum
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCPT0 = LoKi::MCParticles::TransverseMomentum{};
    // ========================================================================
    /** @typedef MCPTDIR
     *  primitive evaluator of particle's transverse momentum
     *  relative to some direction
     *
     *  @code
     *
     *  const MCParticle*           p = ... ;
     *  const LoKi::ThreeVector& vct = ... ;
     *
     *  MCFun ptRel = MCPTDIR( vct ) ;
     *
     *  const double pt = ptRel( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::LorentzVector
     *  @see LoKi::MCParticles::TransverseMomentumRel
     *  @see LoKi::Cuts::MCPTREL
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-23
     */
    typedef LoKi::MCParticles::TransverseMomentumRel                  MCPTDIR ;
    // ========================================================================
    /** @typedef MCPTREL
     *  primitive evaluator of particle's transverse momentum
     *  relative to some direction
     *
     *  @code
     *
     *  const MCParticle*           p = ... ;
     *  const LoKi::ThreeVector& vct = ... ;
     *
     *  MCFun ptRel = MCPTREL ( vct ) ;
     *
     *  const double pt = ptRel( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::LorentzVector
     *  @see LoKi::MCParticles::TransverseMomentumRel
     *  @see LoKi::Cuts::MCPTDIR
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-23
     */
    typedef LoKi::MCParticles::TransverseMomentumRel                  MCPTREL ;
    // ========================================================================
    /** @var MCPX
     *  MCParticle x-momentum
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      v  = MCPX (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::MomentumX
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCPX = LoKi::MCParticles::MomentumX{};
    // ========================================================================
    /** @var MCPY
     *  MCParticle y-momentum
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      v  = MCPY (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::MomentumY
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCPY = LoKi::MCParticles::MomentumY{};
    // ========================================================================
    /** @var MCPZ
     *  MCParticle z-momentum
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      v  = MCPZ (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::MomentumZ
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCPZ = LoKi::MCParticles::MomentumZ{};
    // ========================================================================
    /** @typedef MCQUARK
     *  quark content
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   MCCut cut = MCQUARK( ParticleID::bottom )
     *
     *   const bool beauty  = cut ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::HasQuark
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    typedef LoKi::MCParticles::HasQuark                               MCQUARK ;
    // ========================================================================
    /** @typedef MCREC
     *  the simple function(wrapper for IMCRecontructible tool)
     *  which return the recontruction category for the given MC-particle
     *
     *  @code
     *
     *  const LHCb::MCParticle*  kaon = ... ;
     *  const IMCRecontructible* tool = ... ;
     *
     *  // create the function to recontruction category
     *
     *  MCFun rec = MCREC(tool) ;
     *
     *  // use the function:
     *
     *  const bool recAsLong = rec ( kaon ) == IMCRecontructible::ChargedLong ;
     *
     *  @endcode
     *
     *  @see IMCRecontructible
     *  @see IMCRecontructible::RecCategory
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    typedef LoKi::MCParticles::MCReconstructible                        MCREC ;
    // ========================================================================
    /** @typedef MCRECAS
     *  the simple predicate(wrapper for IMCRecontructible tool)
     *  which checks the reocntruction category for the given MC-particle
     *
     *  @code
     *
     *  const LHCb::MCParticle*  kaon = ... ;
     *  const IMCRecontructible* tool = ... ;
     *
     *  // create the cut to check if it is recontructibleas long track:
     *
     *  MCCut asLong = MCRECAS ( tool , IMCRecontructible::ChargedLong ) ;
     *
     *  // use the predicate
     *
     *  const bool recAsLong = asLong ( kaon )
     *
     *  @endcode
     *
     *  @see IMCRecontructible
     *  @see IMCRecontructible::RecCategory
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    typedef LoKi::MCParticles::MCReconstructibleAs                    MCRECAS ;
    // ========================================================================
    /** @var MCSIGNAL
     *  Is this partiicle is a SIGNAL particle?
     *  @code
     *  const LHCb::MCParticle* p = ... ;
     *  const bool signal = SIGNAL ( p ) ;
     *  if ( signal) { ... } 
     *  @endcode 
     *  @see LHCb::MCParticle
     *  @see LHCb::MCParticle::fromSignal
     *  @see LoKi::MCParticles::Signal
     *  @see LoKi::MCParticles::FromSignal
     *  @see LoKi::CUTS::MCFROMSIGNAL
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2016-11-17
     */
    const LoKi::MCParticles::Signal                                MCSIGNAL {} ;
    // ========================================================================
    /** Simple "switch"
     *  The function evaluated the predicate and returns
     *  one of the predefined values, depending on result on
     *  predicate evaluation.
     *  In some sense it is a conversion of "predicate" to "function"
     *
     *  @code
     *
     *   const MCParticle* mcp = ...  ;
     *
     *   MCFun fun = MCSSWITCH( MCMOTHCUT( MCVALID )  , 1 , -1 )
     *
     *   const double value = fun( p ) ;
     *
     *  @endcode
     *
     *  For this example function returns 1 if MC Particle has valid
     *  mother particle and -1 otherwise
     *
     *  @see LoKi::SipleSwitch
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     */
    typedef LoKi::SimpleSwitch<const LHCb::MCParticle*>             MCSSWITCH ;
    // ========================================================================
    /** @typedef MCSTAT
     *  Simple monitoring counter for the functions
     *
     *  @code
     *
     *  // some function to be monitored
     *  MCFun fun = ... ;
     *
     *  // Create the monitored function using the generic counter
     *  // either local counter, or the counter from IStatSvc
     *  StatEntity* counter = ... ; ///< the generic counter
     *  MCFun mon = MCSTAT ( fun , counter ) ;
     *
     *  for ( ... )
     *    {
     *      ...
     *      const LHCb::Particle* p = ... ;
     *      const double result = mon( p ) ; ///< use the function
     *      ...
     *    }
     *
     *  ...
     *  info ()
     *     << " Monitoring results : "                 << endmsg
     *     << " NEntries:  #" << counter->entries  ()  << endmsg
     *     << " TotalSum:   " << counter->flag     ()  << endmsg
     *     << " Mean+-RMS:  " << counter->flagMean ()
     *     << "+="            << counter->flagRMS  ()  << endmsg
     *     << " Min/Max:    " << counter->flagMin  ()
     *     << "/"             << counter->flagMax  ()  << endmsg ;
     *
     *  @endcode
     *
     *  The alternative (recommended) way to create the monitored function
     *  @code
     *
     *  // some function to be monitored
     *  MCFun fun = ... ;
     *
     *  MCFun mon = monitor ( fun , counter("SomeCounter") ) ;
     *
     *  @endcode
     *
     *  The substitution of the function by monitored function
     *  cound be done "on-flight" without the disturbing of the
     *  actual processing:
     *  @code
     *
     *  // some function to be monitored
     *  MCFun fun = ... ;
     *
     *  if ( monitoring )
     *    {
     *       fun = monitor ( fun , "MonitoredFun" ) ;
     *    }
     *
     *  @endcode
     *
     *  @attention The string representation of the object
     *             is delegated to the underlying function,
     *             therefore the object is NOT recontructable
     *             from its string representations. It is done
     *             on purpose to avoid the disturbing of ids.
     *
     *  @see LoKi::Monitoring::Stat
     *  @see LoKi::monitor
     *  @see StatEntity
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    typedef LoKi::Monitoring::Counter<const LHCb::MCParticle*,double> MCSTAT ;
    // ========================================================================
    /** @typedef MCSUMTREE
     *  Simple function to accumulate certain  values in the decay tree
     *  @see LoKi::MCParticles::SumTree
     *  @see LoKi::MCAlgs::accumulate
     *  @see LoKi::Cut::MCTREESUM
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::SumTree                              MCSUMTREE ;
    // ========================================================================
    /** switch"
     *  The function evaluates the predicate and returns
     *  the values of one of the predefined functions,
     *  depending on result on predicate evaluation.
     *
     *  @code
     *
     *   const MCParticle* mcp = ...  ;
     *
     *   MCFun fun = MCSWITCH( MCMOTHCUT( MCVALID )  ,
     *                          MCMOTHER ( MCPT )     ,
     *                         -1 * GeV               )
     *
     *   const double value = fun( p ) ;
     *
     *  @endcode
     *
     *  For this example function returns the value of
     *  PT of mother particle for MC Particle with valid mother
     *  and -1 * GeV otherwise
     *
     *  @see LoKi::Switch
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     */
    typedef LoKi::Switch<const LHCb::MCParticle*>                     MCSWITCH ;
    // =========================================================================
    /** @var MCTHETA
     *  MCParticle polar angle
     *
     *  @code
     *
     *   const MCParticle* p    = ...         ;
     *   const double      theta  = MCTHETA(  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::Theta
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-17
     */
    const auto MCTHETA = LoKi::MCParticles::Theta{};
    // ========================================================================

    /** @var MCTIME
     *  MCParticle proper lifetime (in c*tau units)
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const double      t  = MCTIME ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::ProperLifeTime
     *  @see LoKi::Cuts::MCCTAU
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MCTIME = LoKi::MCParticles::ProperLifeTime{};
    // ========================================================================
    /** @typedef MCTREEMAX
     *  Simple function to find a maximum value in the decay tree
     *  @see LoKi::MCParticles::MaxTree
     *  @see LoKi::MCAlgs::min_value
     *  @see LoKi::Cuts::MCMAXTREE
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::MaxTree                              MCTREEMAX ;
    // ========================================================================
    /** @typedef MCTREEMIN
     *  Simple function to find a minimum value in the decay tree
     *  @see LoKi::MCParticles::MinTree
     *  @see LoKi::MCAlgs::min_value
     *  @see LoKi::Suts::MCMINTREE
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::MinTree                              MCTREEMIN ;
    // ========================================================================
    /** @typedef MCTREEMULT
     *  Simple function to accumulate certain  values in the decay tree
     *  @see LoKi::MCParticles::MultTree
     *  @see LoKi::MCAlgs::accumulate
     *  @see LoKi::Cuts::MCMULTTREE
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::MultTree                            MCTREEMULT ;
    // ========================================================================
    /** @typedef MCTREESUM
     *  Simple function to accumulate certain  values in the decay tree
     *  @see LoKi::MCParticles::SumTree
     *  @see LoKi::MCAlgs::accumulate
     *  @see LoKi::Cut::MCSUMTREE
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    typedef LoKi::MCParticles::SumTree                              MCTREESUM ;
    // ========================================================================
    /** @var MCTRUE
     *  primitive predicate, "always true"
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-27
     */
    const auto MCTRUE = LoKi::Constant<const LHCb::MCParticle*,bool>{ true } ;
    // ========================================================================
    /** @var MCVALID
     *
     *  The trivial checker of "the validity" of the pointer to
     *   LHCb::MCParticle objects
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const bool valid =   = MCVALID (  p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Valid
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-01-21
     */
    const auto MCVALID = LoKi::Valid<const LHCb::MCParticle*>{} ;
    // ========================================================================
    /** @typedef MCVFASPF
     *  simple adapter, which evaluated the actual function
     *  computation to "origin" vertex for MC particle
     *
     *  @code
     *
     *   const LoKi::Point3D& point = ... ;
     *
     *   MCFun dist = MCVFASPF( MCVDIST( point ) ) ;
     *
     *  @endcode
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
     *  @date 2005-05-16
     */
    typedef LoKi::MCParticles::MCVertexFunAdapter                    MCVFASPF ;
    // ========================================================================
    /** @var MCY
     *  Evaluator of rapidity of the particle
     *  \f$ y = \frac{1}{2}\log \frac{ E - p_z }{ E + p_z } \f$
     *  @see LoKi::MCParticles::Rapidity
     *  @see LoKi::Cuts::MCETA
     *  @see LoKi::Cuts::MCY0
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-02-10
     */
    const auto MCY = LoKi::MCParticles::Rapidity {};
    // ========================================================================
    /** @var MCY0
     *  Evaluator of rapidity of the particle
     *  \f$ y_0 = \frac{1}{2}\log \frac{ E - p }{ E + p } \f$
     *  @see LoKi::MCParticles::Rapidity0
     *  @see LoKi::Cuts::MCETA
     *  @see LoKi::Cuts::MCY
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-02-10
     */
    const auto MCY0 = LoKi::MCParticles::Rapidity0{};
    // ========================================================================
    /** @var MCZERO
     *  primitive function, "always 0"
     *  @see LoKi::Constant
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-27
     */
    const auto MCZERO = LoKi::Constant<const LHCb::MCParticle*,double>{ 0 } ;
    // ========================================================================
    /** @var MESON
     *  Check for particle type
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool meson   = MESON   ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::IsMeson
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto MESON = LoKi::MCParticles::IsMeson{};
    // ========================================================================
    /** @typedef NINMCDOWN
     *  simple adapter function which counts teh number
     *  particle in MC decay tree which satisfy a certain criteria
     *
     *  @code
     *
     *   const MCParticle* p = ... ;
     *
     *   // number of pions
     *   MCFun fun = NINMCDOWN( "pi+" == MCABSID ) ;
     *
     *   double numPI = fun( p ) ;
     *
     *  @endcode
     *
     */
    typedef LoKi::MCParticles::NinMCdownTree                        NINMCDOWN ;
    // ========================================================================
    /** @var NUCLEUS
     *  Check for particle type
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool nucleus = NUCLEUS ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::IsNucleus
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto NUCLEUS = LoKi::MCParticles::IsNucleus{};
    // ========================================================================
    /** @var NEUTRAL
     *  Check for particle charge
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool neutral = NEUTRAL ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::IsNeutral
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto NEUTRAL = LoKi::MCParticles::IsNeutral{};
    // ========================================================================
    /** @var OSCILLATED
     *
     *  It evaluated to 'true' for scilalted particles
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *   const bool oscillated = OSCILLATED (  p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::Oscillated
     *
     *  @author Vanya Belyaev ibelyaev@physics.syr.edu
     *  @date   2006-02-05
     */
    const auto OSCILLATED = LoKi::MCParticles::Oscillated{};
    // ========================================================================
    /** @var STRANGE
     *  Check for strange quark
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool strange = STRANGE ( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::HasQuark
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const auto STRANGE = LoKi::MCParticles::HasQuark{ LHCb::ParticleID::strange };
    // ========================================================================
    /** @var TOP
     *  Check for TOP  quark
     *
     *  @code
     *
     *   const MCParticle* p  = ...         ;
     *
     *   const bool top  = TOP( p ) ;
     *
     *  @endcode
     *
     *  @see MCParticle
     *  @see LoKi::MCParticles::HasQuark
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    const auto TOP = LoKi::MCParticles::HasQuark{ LHCb::ParticleID::top } ;
    // ========================================================================
  } //                                              end of namespace LoKi::Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCPARTICLECUTS_H
// ============================================================================

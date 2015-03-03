// $Id: ParticleCuts.h,v 1.25 2008-04-10 18:42:02 ibelyaev Exp $
// ============================================================================
#ifndef LOKI_PHYSPARTICLECUTS_H 
#define LOKI_PHYSPARTICLECUTS_H 1
// ============================================================================
// Include files
// ============================================================================
// Event
// ============================================================================
#include "Event/ProtoParticle.h"
#include "Event/Track.h"
// ============================================================================
// LoKiCore 
// ============================================================================
#include "LoKi/CoreCuts.h"
#include "LoKi/MoreFunctions.h"
#include "LoKi/Monitoring.h"
// ============================================================================
// LoKiPhys 
// ============================================================================
#include "LoKi/Particles.h"
#include "LoKi/PIDOperators.h"
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
 *  @date 2006-01-28 
 */
// ============================================================================
namespace LoKi
{
  namespace Cuts
  {
    // ========================================================================
    /** @typedef EQUALTO 
     *  The trivial prodicate which tests the value of some function 
     *  against some predefinied value 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *
     *  @see LoKi::EqualToValue 
     *
     *  @date 2006-02-24
     */
    typedef LoKi::EqualToValue<const LHCb::Particle*>                 EQUALTO ;
    // ========================================================================
    /** @var TRTYPE 
     *  The trivial function which evaluates LHCb::Track::type
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *
     *  const bool Long = LHCb::Track::Long == TRTYPE( p ) ;
     *
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LHCb::Track
     *  @see LHCb::Track::Type
     *  @see LHCb::Track::type 
     *  @see LoKi::Cuts::TRTYPE
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const LoKi::Particles::TrackType                                   TRTYPE ;
    // ========================================================================
    /** @typedef PPINFO 
     *  Trivial function which evaluates LHCb::Particle::info
     *  
     *  It relies on the method LHCb::Particle::info
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  Fun richPi = PPINFO( LHCb::ProtoParticle::RichPion , -500 , -1000 ) ;
     * 
     *  const  double pion = richPi ( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LHCb::Particle::proto
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::info
     *  @see LHCb::ProtoParticle::additionalInfo
     *  @see LoKi::Cuts::PROTOINFO
     *  @see LoKi::ExtraInfo::info
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::Particles::ProtoInfo                                 PPINFO ;
    // ========================================================================
  }
  namespace Cuts 
  {    
    // ========================================================================
    /** @var ABSID 
     *  evaluator of the absolute value for the particle id 
     *
     *  @code
     *  
     *   const LHCb::Particle* p = ... ;
     *   int pid = (int) ABSID( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::AbsIdentifier 
     *  @see LHCb::ParticleID 
     *  @see LHCb::Particle
     *  
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    const LoKi::Particles::AbsIdentifier                                ABSID ;
    // ========================================================================
    /** @typedef ADMASS 
     *  Absolute value for delta Mass 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     Fun dm = ADMASS( 3.010 * GeV );
     *     const double    mm       = dm( particle ) ;
     *
     *  @endcode
     *
     *  Alternatively the function object could be created
     *  from Particle ID or particle name, in this case it requires
     *  to be supplied with pointer to <tt>IParticlePpopertySvc</tt>
     *  service. The reference to <tt>ParticleProperty</tt> object 
     *  also could be used for creation of functor: 
     *  
     *
     *  @code 
     *
     *     const Particle*       particle = ... ; // get the particle
     *     IParticlePropertySvc* ppsvc    = ... ; // get the service 
     *     const ParticleProperty& D0     = ... ; // get from the service 
     *
     *     Fun dm1 = ADMASS( "D0" , ppsvc ) ;
     *     const double    dm1       = dm1 ( particle ) ;
     *     Fun dm2 = ADMASS( 241  , ppsvc ) ;
     *     const double    dm2       = dm2 ( particle ) ;
     *     Fun dm3 = ADMASS( D0 ) ;
     *     const double    dm3       = dm3 ( particle ) ;
     *     Fun dm4 = ADMASS( "D0" ) ;
     *     const double    mm4       = dm4 ( particle ) ;
     *     Fun dm5 = ADMASS( ParticleID( 421 )  ) ;
     *     const double    mm5       = dm5 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::AbsDeltaMass
     *  @see LoKi::Function
     *  @see LoKi::Cuts::Fun
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */        
    typedef LoKi::Particles::AbsDeltaMass                              ADMASS ;
    // ========================================================================
    /** @typedef ADMMASS 
     *  Absolute value of delta Measured Mass 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     Fun dmm = ADMMASS( 3.010 * GeV );
     *     const double    mm       = dmm( particle ) ;
     *
     *  @endcode
     *
     *  Alternatively the function object could be created
     *  from Particle ID or particle name, in this case it requires
     *  to be supplied with pointer to <tt>IParticlePpopertySvc</tt>
     *  service. The reference to <tt>ParticleProperty</tt> object 
     *  also could be used for creation of functor: 
     *       
     *  @code 
     *
     *     const Particle*       particle = ... ; // get the particle
     *     IParticlePropertySvc* ppsvc    = ... ; // get the service 
     *     const ParticleProperty& D0     = ... ; // get from the service 
     *
     *     Fun dmm1 = ADMMASS( "D0" , ppsvc ) ;
     *     const double    mm1       = dmm1 ( particle ) ;
     *     Fun dmm2 = ADMMASS( 241  , ppsvc ) ;
     *     const double    mm2       = dmm2 ( particle ) ;
     *     Fun dmm3 = ADMMASS( D0  ) ;
     *     const double    mm3       = dmm3 ( particle ) ;
     *     Fun dmm4 = ADMMASS( "D0" ) ;
     *     const double    mm4       = dmm4 ( particle ) ;
     *     Fun dmm5 = ADMMASS( ParticleID( 421 )  ) ;
     *     const double    mm5       = dmm5 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::DeltaMeasuredMass
     *  @see LoKi::Particle::AbsDeltaMeasuredMass
     *  @see LoKi::Function
     *  @see LoKi::Cuts::Fun
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */        
    typedef LoKi::Particles::AbsDeltaMeasuredMass                     ADMMASS ;    
    // ========================================================================
    /** @var ALL
     *  the trivial functor which always evaluates to "true"
     *  
     *  @see LoKi::BooleanConstant
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,bool>           ALL ( true ) ;    
    // ========================================================================
    /** @var BASIC 
     *  The trivial predicate which evaluates to "true" for 
     *  the "basic" particles 
     *
     *  It relies on the method LHCb::Particle::isBasic
     *
     *  @code 
     * 
     *   const LHCb::Particle* p = ... ;
     *   const bool basic = BASIC( p ) ;
     *  
     *  @endcode 
     *
     *  @see LoKi::Cuts::ISBASIC
     *  @see LHCb::Particle
     *  @see LoKi::Particles::IsBasic
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const   LoKi::Particles::IsBasic                                    BASIC ;
    // ========================================================================
    /** @typedef CHI2CLAPP
     *  Evaluator of the closest approach chi2 
     *  distance between 2 particles 
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  const LHCb:Particle* K2 = ... ;
     * 
     *  Cun clapp = CHI2CLAPP ( K1 , geo() ) ;
     *  
     *  const double chi2 = clapp( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ClosestApproachChi2 
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::CLAPPCHI2
     *  @see LoKi::Cuts::CHI2DOCA
     *  @see LoKi::Cuts::DOCACHI2
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ClosestApproachChi2                    CHI2CLAPP ;
    // ========================================================================
    /** @typedef CHI2DOCA 
     *  Evaluator of the closest approach chi2 
     *  distance between 2 particles 
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  const LHCb:Particle* K2 = ... ;
     * 
     *  Cun fun = CHI2DOCA ( K1 , geo() ) ;
     *  
     *  const double value = clapp( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ClosestApproachChi2 
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::CLAPPCHI2
     *  @see LoKi::Cuts::CHI2CLAPP
     *  @see LoKi::Cuts::DOCACHI2
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ClosestApproachChi2                     CHI2DOCA ;
    // ========================================================================
    /** @typedef CHI2IP 
     *  Evaluator of chi2 for the impact parameter of 
     *  the particle with respect to vertex/point 
     *
     *  The tool IGeomDispCalculator is used 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  const LHCb::Vertex*   v = ... ;
     * 
     *  Fun chi2 = CHI2IP ( v , geo() ) ;
     * 
     *  const double value = chi2( p ) ;
     *  
     *  @endcode 
     *  
     *  @see IGeomDispCalculator
     *  @see LoKi::Vertices::ImpParBase
     *  @see LoKi::Vertices::ImpactParamTool
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::IPCHI2
     *  @see LoKi::Particles::ImpParChi2 
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ImpParChi2                                CHI2IP ;
    // ========================================================================
    /** @typedef CHI2M 
     *  Delta Measured Mass in chi2 units 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     Fun dmm = CHI2M ( 3.010 * GeV );
     *     const double  chi2  = dmm( particle ) ;
     *
     *  @endcode
     *
     *  Alternatively the function object could be created
     *  from Particle ID or particle name, in this case it requires
     *  to be supplied with pointer to <tt>IParticlePpopertySvc</tt>
     *  service: 
     *
     *  @code 
     *
     *     const Particle*       particle = ... ; // get the particle
     *     IParticlePropertySvc* ppsvc    = ... ; // get the service 
     *     const ParticleProperty& D0     = ... ; // get from the service 
     *
     *     Fun dmm1 = CHI2M( "D0" , ppsvc ) ;
     *     const double    chi2_1       = dmm1 ( particle ) ;
     *     Fun dmm2 = CHI2M( 241  , ppsvc ) ;
     *     const double    chi2_2       = dmm2 ( particle ) ;
     *     Fun dmm3 = CHI2M( D0 ) ;
     *     const double    chi2_3       = dmm3 ( particle ) ;
     *
     *  @endcode
     *
     *  More simple ways are also valid:
     *
     *  @code 
     *
     *     const Particle*       particle = ... ; // get the particle
     *
     *     Fun dmm1 = CHI2M( LHCb::ParticleID( 241 ) ) ;
     *     const double    chi2_1       = dmm1 ( particle ) ;
     *     Fun dmm2 = CHI2M( "D0" ) ;
     *     const double    chi2_2       = dmm2 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::DeltaMeasuredMassChi2
     *  @see LoKi::Particle::DeltaMeasuredMass
     *  @see LoKi::Particle::MeasuredMass
     *  @see LoKi::Function
     *  @see LoKi::Cuts::Fun
     *  @attention LoKi::Particles::ErrorValue returned on error 
     */        
    typedef LoKi::Particles::DeltaMeasuredMassChi2                      CHI2M ;
    // ========================================================================
    /** @typedef CHI2MASS 
     *  Delta Measured Mass in chi2 units 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     Fun dmm = CHI2MASS ( 3.010 * GeV );
     *     const double  chi2  = dmm( particle ) ;
     *
     *  @endcode
     *
     *  Alternatively the function object could be created
     *  from Particle ID or particle name, in this case it requires
     *  to be supplied with pointer to <tt>IParticlePpopertySvc</tt>
     *  service: 
     *
     *  @code 
     *
     *     const Particle*       particle = ... ; // get the particle
     *     IParticlePropertySvc* ppsvc    = ... ; // get the service 
     *     const ParticleProperty& D0     = ... ; // get from the service 
     *
     *     Fun dmm1 = CHI2MASS( "D0" , ppsvc ) ;
     *     const double    chi2_1       = dmm1 ( particle ) ;
     *     Fun dmm2 = CHI2MASS( 241  , ppsvc ) ;
     *     const double    chi2_2       = dmm2 ( particle ) ;
     *     Fun dmm3 = CHI2MASS( D0 ) ;
     *     const double    chi2_3       = dmm3 ( particle ) ;
     *
     *  @endcode
     *
     *  More simple ways are also valid:
     *
     *  @code 
     *
     *     const Particle*       particle = ... ; // get the particle
     *
     *     Fun dmm1 = CHI2MASS( LHCb::ParticleID( 241 ) ) ;
     *     const double    chi2_1       = dmm1 ( particle ) ;
     *     Fun dmm2 = CHI2MASS( "D0" ) ;
     *     const double    chi2_2       = dmm2 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::DeltaMeasuredMassChi2
     *  @see LoKi::Particle::DeltaMeasuredMass
     *  @see LoKi::Particle::MeasuredMass
     *  @see LoKi::Function
     *  @see LoKi::Cuts::Fun
     *  @attention LoKi::Particles::ErrorValue returned on error 
     */        
    typedef LoKi::Particles::DeltaMeasuredMassChi2                   CHI2MASS ;
    // ========================================================================
    /** @typedef CHI2MIP 
     *  Evaluation of minimal value of chi2 for the impact parameter of 
     *  particle with respect to several vertices 
     *
     *  The tool IGeomDispCalculator is used 
     *
     *  @code 
     *  
     *   const LHCb::Particle* p = ... ;
     *   SEQUENCE vertices  ;
     * 
     *   Fun ipMin = CHI2MIP ( vertices , geo() ) ;
     *   
     *   const double value = ipMin ( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinImpParChi2
     *  @see IGeomDispCalculator
     *  @see LoKi::Vertices::ImpParBase
     *  @see LoKi::Vertices::ImpactParamTool
     *  @see LoKi::Vertices::ImpPar 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::MIPCHI2
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinImpParChi2                            CHI2MIP ;
    // ========================================================================
    /** @var CHI2TR 
     *  The trivial funtion which evaluates LHCb::Track::chi2
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  const double chi2  = CHI2TR ( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see LHCb::Track
     *  @see LHCb::Track::chi2 
     *  @see LoKi::Particles::TrackChi2 
     *  @see LoKi::TRCHI2
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const LoKi::Particles::TrackChi2                                   CHI2TR ;
    // ========================================================================
    /** @typedef CHILD 
     *  Simple adapter function which 
     *  apply the function to a daughter  particle
     * 
     *  @code
     *
     *  const LHCb::Particle* B = ... ;
     *
     *  // the transverse momentum of the first daughter particle:
     *  Fun fun = CHILD( PT , 1 ) ;
     *  const double pt1 = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ChildFunction
     *  @see LoKi::Cuts::CHILDFUN
     *  @see LoKi::Cuts::FUNCHILD
     *  @see LoKi::Child::child 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-21
     */
    typedef LoKi::Particles::ChildFunction                              CHILD ;
    // ========================================================================
    /** @typedef CHILDCUT 
     *  Simple adapter predicate  which 
     *  apply the predicate to a daughter  particle
     * 
     *  @code
     *
     *  const LHCb::Particle* B = ... ;
     *
     *  // ask if the first daughter is pi+
     *  Cut cut = CHILDCUT( "pi+" == ID  , 1 ) ;
     *  const bool pion = cut ( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ChildPredicate
     *  @see LoKi::Child::child 
     *  @see LoKi::Cuts::CUTCHILD
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-21
     */
    typedef LoKi::Particles::ChildPredicate                          CHILDCUT ;
    // ========================================================================
    /** @typedef CHILDFUN
     *  Simple adapter function which 
     *  apply the function to a daughter  particle
     * 
     *  @code
     *
     *  const LHCb::Particle* B = ... ;
     *
     *  // the transverse momentum of the first daughter particle:
     *  Fun fun = CHILDFUN ( PT , 1 ) ;
     *  const double pt1 = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ChildFunction
     *  @see LoKi::Cuts::CHILD
     *  @see LoKi::Cuts::FUNCHILD
     *  @see LoKi::Child::child 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-21
     */
    typedef LoKi::Particles::ChildFunction                           CHILDFUN ;
    // ========================================================================
    /** @var CL
     *  Trivial function which evaluates "confidence level" for 
     *  the particle
     *
     *  It relies on the method LHCb::Particle::confLevel
     *
     *  @code 
     * 
     *   const LHCb::Particle* p = ... ;
     *   const double cl = CL( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Particles::ConfidenceLevel 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const   LoKi::Particles::ConfidenceLevel                               CL ;
    // ========================================================================
    /** @typedef CLAPP 
     *  Evaluator of the closest approach 
     *  distance between 2 particles 
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  const LHCb:Particle* K2 = ... ;
     * 
     *  Cun clapp = CLAPP( K1 , geo() ) ;
     *  
     *  const double closestApproach = clapp( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Cuts::CLOSESTAPPROACH
     *  @see LoKi::Particles::ClosestApproach
     *  @see LHCb::Particle
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ClosestApproach                            CLAPP ;
    // ========================================================================
    /** @typedef CLAPPCHI2 
     *  Evaluator of the closest approach chi2 
     *  distance between 2 particles 
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  const LHCb:Particle* K2 = ... ;
     * 
     *  Cun clapp = CLAPPCHI2( K1 , geo() ) ;
     *  
     *  const double chi2 = clapp( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ClosestApproachChi2 
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::DOCACHI2
     *  @see LoKi::Cuts::CHI2DOCA
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ClosestApproachChi2                    CLAPPCHI2 ;
    // ========================================================================
    /** @typedef CLOSESTAPPROACH
     *  Evaluator of the closest approach 
     *  distance between 2 particles 
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  const LHCb:Particle* K2 = ... ;
     * 
     *  Fun fun = CLAPP( K1 , geo() ) ;
     *  
     *  const double value = fun ( K2 ) ;
     *
     *  @endcode 
     *
     *  The name comes from Vava GLIGOROV v.gligorov@physics.gla.ac.uk
     *
     *  @see LoKi::Cuts::CLAPP
     *  @see LoKi::Particles::ClosestApproach
     *  @see LHCb::Particle
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ClosestApproach                  CLOSESTAPPROACH ;
    // ========================================================================
    /** @typedef CLOSESTAPPROACHCHI2
     *  Evaluator of the closest approach chi2 
     *  distance between 2 particles 
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  const LHCb:Particle* K2 = ... ;
     * 
     *  Fun fun = CLOSESTAPPROACHCHI2 ( K1 , geo() ) ;
     *  
     *  const double value = fun ( K2 ) ;
     *
     *  @endcode 
     *
     *  The name comes from Vava GLIGOROV v.gligorov@physics.gla.ac.uk
     *
     *  @see LoKi::Particles::ClosestApproachChi2 
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::DOCACHI2
     *  @see LoKi::Cuts::CHI2DOCA
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ClosestApproachChi2          CLOSESTAPPROACHCHI2 ;
    // ========================================================================
    /** @var CONFLEV
     *  Trivial function which evaluates "confidence level" for 
     *  the particle
     *
     *  It relies on the method LHCb::Particle::confLevel
     *
     *  @code 
     * 
     *   const LHCb::Particle* p = ... ;
     *   const double cl = CONFLEV ( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Particles::ConfidenceLevel 
     *  @see LoKi::Cuts::CL
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const   LoKi::Particles::ConfidenceLevel                          CONFLEV ;
    // ========================================================================
    /** @var CONFLEVEL
     *  Trivial function which evaluates "confidence level" for 
     *  the particle
     *
     *  It relies on the method LHCb::Particle::confLevel
     *
     *  @code 
     * 
     *   const LHCb::Particle* p = ... ;
     *   const double cl = CONFLEVEL ( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Particles::ConfidenceLevel 
     *  @see LoKi::Cuts::CL
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const   LoKi::Particles::ConfidenceLevel                        CONFLEVEL ;
    // ========================================================================
    /** @typedef COSPF
     *  Simple function which evalutes the coside of the angle inbetween
     *  the particle momentum and the vector from the primary to the secondary
     *  vertex 
     * 
     *  @code
     * 
     *  const LHCb::Particle* B = ... ;
     *
     *  // get the tool 
     *  const LHCb::VertexBase* primary = ... ;
     *
     *  // construct a function
     *  Fun fun = COSPF ( primary ) ;
     *
     *  // use it!
     *  const double cosTheta = dira( B ) ;
     *
     *  @endcode
     *
     *  The name comes from Vava GLIGOROV v.gligorov@physics.gla.ac.uk
     *
     *  @see LoKi::Particles::CosineDirectionAngle
     *  @see LoKi::Cuts::DANG
     *  @see LoKi::Cuts::DIRA
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-03-20
     */
    typedef LoKi::Particles::CosineDirectionAngle                       COSPF ;
    // ========================================================================
    /** @typedef COUNTER
     *  Simple monitoring counter for predicates
     *
     *  @code 
     *
     *  // some predicate to be monitored
     *  Cut cut = ... ;
     *
     *  // Create the monitored predicate using the generic counter
     *  // either local counter, or the counter from IStatSvc or ICounterSvc
     *  StatEntity* counter = ... ; ///< the generic counter
     *  Cut mon = COUNTER ( cut , counter ) ;
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
     *     << " Monitoring results : "                       << endreq 
     *     << " NEntries:  #" << counter->entries ()         << endreq 
     *     << " Efficiency:(" << counter->eff     ()   
     *     << "+="            << counter->effErr  () << ")%" << endreq ;
     *
     *  @endcode
     *
     *  The alternative (recommended) way to create the monitored predicate:
     *  @code 
     *
     *  Cut cut = ... ;
     *
     *  Cut mon = monitor ( cut , counter("SomeEffCounter") ) ;
     *  
     *  @endcode 
     *
     *  The substitution of the predicate by monitored predicate 
     *  cound be done "on-flight" without the disturbing of the
     *  actual processing:
     *  @code
     *
     *  // some predicate to be monitored
     *  Cut cut = ... ;
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
    typedef LoKi::Monitoring::Counter<const LHCb::Particle*,bool>    COUNTER ;
    // ========================================================================
    /** @typedef CTAU
     *  Simple evaluator of the proper lifetime 
     *  distance (c*tau) of the particle between 
     *  the "endVertex" and the other vertex
     *
     *  LoKi::Particles::VertexDistance is used to estimate the 
     *  3D-distance 
     *  
     *  @code 
     * 
     *  const LHCb::Particle* B   = ... ;
     *  const LHCb::Vertex*   pv  = ... ;
     *
     *  Fun cTau = CTAU( pv ) ;
     * 
     *  const double ctau = cTau( B ) ;
     *
     *  @endcode 
     *  
     *  @see LoKi::Particles::TimeDistance 
     *  @see LoKi::Particles::VertexDistance 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::TIMEDIST
     *  @see LoKi::Cuts::TDIST
     *  @see LoKi::Cuts::TD
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::TimeDistance                                CTAU ;
    // ========================================================================
    /** @typedef CTAUDOT 
     *  Simple evaluator of the proper lifetime 
     *  distance (c*tau) of the particle between 
     *  the "endVertex" and the other vertex along the particle momentum
     *
     *  LoKi::Particles::VertexDotDistance is used to estimate the 
     *  signed 3D-distance 
     *  
     *  @code 
     * 
     *  const LHCb::Particle* B   = ... ;
     *  const LHCb::Vertex*   pv  = ... ;
     *
     *  Fun cTau = CTAUDOT( pv ) ;
     * 
     *  const double ctau = cTau( B ) ;
     *
     *  @endcode 
     *  
     *  @see LoKi::Particles::TimeDotDistance 
     *  @see LoKi::Particles::VertexDistance 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::TDOT
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::TimeDotDistance                          CTAUDOT ;
    // ========================================================================
    /** @typedef CTAUSIGN 
     *  simple evaluator of the (signed) proper lifetime 
     *  distance (c*tau) of the particle between 
     *  the "endVertex" and the other vertex
     *
     *  LoKi::Particles::VertexSignedDistance is used to estimate the 
     *  signed 3D-distance 
     *  
     *  @code 
     * 
     *  const LHCb::Particle* B   = ... ;
     *  const LHCb::Vertex*   pv  = ... ;
     *
     *  Fun cTau = CTAUSIGN( pv ) ;
     * 
     *  const double ctau = cTau( B ) ;
     *
     *  @endcode 
     *  
     *  @see LoKi::Particles::TimeSignedDistance 
     *  @see LoKi::Particles::VertexDistance 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::TSIGND
     *  @see LoKi::Cuts::TDSIGN
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::TimeSignedDistance                      CTAUSIGN ;
    // ========================================================================
    /** @typedef CUTCHILD 
     *  Simple adapter predicate  which 
     *  apply the predicate to a daughter  particle
     * 
     *  @code
     *
     *  const LHCb::Particle* B = ... ;
     *
     *  // ask if the first daughter is pi+
     *  Cut cut = CHILDCUT( "pi+" == ID  , 1 ) ;
     *  const bool pion = cut ( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ChildPredicate
     *  @see LoKi::Child::child 
     *  @see LoKi::Cuts::CHILDCUT
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-21
     */
    typedef LoKi::Particles::ChildPredicate                          CUTCHILD ;
    // ========================================================================
    /** @typedef DANG
     *  Simple function which evalutes the coside of the angle inbetween
     *  the particle momentum and the vector from the primary to the secondary
     *  vertex 
     * 
     *  @code
     * 
     *  const LHCb::Particle* B = ... ;
     *
     *  // get the tool 
     *  const LHCb::VertexBase* primary = ... ;
     *
     *  // construct a function
     *  Fun dang = DANG ( primary ) ;
     *
     *  // use it!
     *  const dobuble cosTheta = DANG( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::CosineDirectionAngle
     *  @see LoKi::Cuts::DIRA
     *  @see LoKi::Cuts::COSPF
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-03-20
     */
    typedef LoKi::Particles::CosineDirectionAngle                        DANG ;
    // ========================================================================
    /** @typedef DETA
     *  simple evaluator of "delta eta" of the particle momenta
     * 
     *  @code 
     *
     *   double eta = ... ;
     * 
     *   // create the function: 
     *  Fun deta = DETA(eta) ;
     * 
     *  const LHCb::Particle* p = ... ;
     *
     *  // use the function 
     *  const double result = deta ( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particless::DeltaEta 
     *  @see LoKi::Particless::Eta
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03 
     */
    typedef LoKi::Particles::DeltaEta                                    DETA ;
    // ========================================================================    
    /** @typedef DELTAR2 
     *  simple evaluator of "delta eta" of the particle momenta
     * 
     *  @code 
     *
     *   double et
     * 
     *   // create the function: 
     *  Fun r2 = DELTAR2( v ) ;
     * 
     *  const LHCb::::Particle* p = ... ;
     *
     *  // use the function 
     *  const double result = r2 ( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particless::DeltaR2  
     *  @see LoKi::Particless::DeltaEta
     *  @see LoKi::Particless::DeltaPhi
     *  @see LoKi::Cuts::DR2 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03 
     */
    typedef LoKi::Particles::DeltaR2                                  DELTAR2 ;
    // ========================================================================
    /** @typedef DIRA
     *  Simple function which evalutes the coside of the angle inbetween
     *  the particle momentum and the vector from the primary to the secondary
     *  vertex 
     * 
     *  @code
     * 
     *  const LHCb::Particle* B = ... ;
     *
     *  // get the tool 
     *  const LHCb::VertexBase* primary = ... ;
     *
     *  // construct a function
     *  Fun dira = DIRA ( primary ) ;
     *
     *  // use it!
     *  const dobuble cosTheta = dira( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::CosineDirectionAngle
     *  @see LoKi::Cuts::DANG
     *  @see LoKi::Cuts::COSPF
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-03-20
     */
    typedef LoKi::Particles::CosineDirectionAngle                        DIRA ;
    // ========================================================================
    /** @typedef DMASS 
     *  Delta Mass 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     Fun dm = DMASS( 3.010 * GeV );
     *     const double    mm       = dm( particle ) ;
     *
     *  @endcode
     *
     *  Alternatively the function object could be created
     *  from Particle ID or particle name, in this case it requires
     *  to be supplied with pointer to <tt>IParticlePpopertySvc</tt>
     *  service. The reference to <tt>ParticleProperty</tt> object 
     *  also could be used for creation of functor: 
     *  
     *
     *  @code 
     *
     *     const Particle*       particle = ... ; // get the particle
     *     IParticlePropertySvc* ppsvc    = ... ; // get the service 
     *     const ParticleProperty& D0     = ... ; // get from the service 
     *
     *     Fun dm1 = DMASS( "D0" , ppsvc ) ;
     *     const double    dm1       = dm1 ( particle ) ;
     *     Fun dm2 = DMASS( 241  , ppsvc ) ;
     *     const double    dm2       = dm2 ( particle ) ;
     *     Fun dm3 = DMASS( D0 ) ;
     *     const double    dm3       = dm3 ( particle ) ;
     *     Fun dm4 = DMASS( "D0" ) ;
     *     const double    mm4       = dm4 ( particle ) ;
     *     Fun dm5 = DMASS( ParticleID( 421 )  ) ;
     *     const double    mm5       = dm5 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::MeasuredMass
     *  @see LoKi::Function
     *  @see LoKi::Cuts::Fun
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */        
    typedef LoKi::Particles::DeltaMass                                  DMASS ;
    // ========================================================================
    /** @typedef DMMASS 
     *  Delta Measured Mass 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     Fun dmm = DMMASS( 3.010 * GeV );
     *     const double    mm       = dmm( particle ) ;
     *
     *  @endcode
     *
     *  Alternatively the function object could be created
     *  from Particle ID or particle name, in this case it requires
     *  to be supplied with pointer to <tt>IParticlePpopertySvc</tt>
     *  service. The reference to <tt>ParticleProperty</tt> object 
     *  also could be used for creation of functor: 
     *       
     *  @code 
     *
     *     const Particle*       particle = ... ; // get the particle
     *     IParticlePropertySvc* ppsvc    = ... ; // get the service 
     *     const ParticleProperty& D0     = ... ; // get from the service 
     *
     *     Fun dmm1 = DMMASS( "D0" , ppsvc ) ;
     *     const double    mm1       = dmm1 ( particle ) ;
     *     Fun dmm2 = DMMASS( 241  , ppsvc ) ;
     *     const double    mm2       = dmm2 ( particle ) ;
     *     Fun dmm3 = DMMASS( D0 ) ;
     *     const double    mm3       = dmm3 ( particle ) ;
     *     Fun dmm4 = DMMASS( "D0" ) ;
     *     const double    mm4       = dmm4 ( particle ) ;
     *     Fun dmm5 = DMMASS( ParticleID( 421 )  ) ;
     *     const double    mm5       = dmm5 ( particle ) ;
     *
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::MeasuredMass
     *  @see LoKi::Function
     *  @see LoKi::Cuts::Fun
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */        
    typedef LoKi::Particles::DeltaMeasuredMass                         DMMASS ;
    // ========================================================================
    /** @typedef DOCA
     *  Evaluator of the closest approach 
     *  distance between 2 particles 
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  const LHCb:Particle* K2 = ... ;
     * 
     *  Cun clapp = DOCA ( K1 , geo() ) ;
     *  
     *  const double closestApproach = clapp( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ClosestApproach
     *  @see LHCb::Particle
     *  @see LHCb::Cuts::CLAPP
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ClosestApproach                             DOCA ;
    // ========================================================================
    /** @typedef DOCACHI2 
     *  Evaluator of the closest approach chi2 
     *  distance between 2 particles 
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  const LHCb:Particle* K2 = ... ;
     * 
     *  Cun clapp = DOCACHI2( K1 , geo() ) ;
     *  
     *  const double chi2 = clapp( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ClosestApproachChi2 
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::CLAPPCHI2
     *  @see LoKi::Cuts::CHI2DOCA
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ClosestApproachChi2                     DOCACHI2 ;
    // ========================================================================
    /** @typedef DOCAMIN
     *  Evaluator of the minimal closest approach 
     *  distance between the particle and sequence of other particles  
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  
     *  SEQUENCE parts = ... ;
     * 
     *  Cun clappr = DOCAMIN ( parts , geo() ) ;
     *  
     *  const double minclApp  = clappr( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinClosestApproach
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::MINCLAPP
     *  @see LoKi::Cuts::MINDOCA
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinClosestApproach                       DOCAMIN ;
    // ========================================================================
    /** @typedef DPHI
     *  simple evaluator of "delta phi" of the particle momenta
     * 
     *  @code 
     *
     *   double phi = ... ;
     * 
     *   // create the function: 
     *  Fun dphi = DPHI(phi) ;
     * 
     *  const LHCb::Particle* p = ... ;
     *
     *  // use the function 
     *  const double result = dphi ( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particless::DeltaPhi 
     *  @see LoKi::Particless::Phi
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03 
     */
    typedef LoKi::Particles::DeltaPhi                                    DPHI ;
    // ========================================================================
    /** @typedef DR2 
     *  simple evaluator of "delta eta" of the particle momenta
     * 
     *  @code 
     *
     *   double et
     * 
     *   // create the function: 
     *  Fun r2 = DR2( v ) ;
     * 
     *  const LHCb::Particle* p = ... ;
     *
     *  // use the function 
     *  const double result = r2 ( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particless::DeltaR2  
     *  @see LoKi::Particless::DeltaEta
     *  @see LoKi::Particless::DeltaPhi
     *  @see LoKi::Cuts::DELTAR2 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03 
     */
    typedef LoKi::Particles::DeltaR2                                      DR2 ;
    // ========================================================================
    /** @var E 
     *  Particle's energy
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    e        = E( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::Energy
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::Energy                                         E ;
    // ========================================================================
//     /** @typedef EQUALTO 
//      *  The trivial prodicate which tests the value of some function 
//      *  against some predefinied value 
//      *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
//      *
//      *  @see LoKi::EqualToValue 
//      *
//      *  @date 2006-02-24
//      */
//     typedef LoKi::EqualToValue<const LHCb::Particle*>                 EQUALTO ;
//     // ========================================================================
    /** @var ETA
     *  Particle pseudorapidity 
     * 
     *  @code
     * 
     *   const Particle* p    = ...         ;
     *   const double    eta  = ETA(  p ) ;
     *
     *  @endcode 
     *
     *  @see Particle
     *  @see LoKi::Particles::PseudoRapidity           
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const LoKi::Particles::PseudoRapidity                                 ETA ;
    // ========================================================================
    /** @typedef FILTER
     *  Simple adapter which allows to use the tool IFilterCriterion
     *  as LoKi functor/cut
     * 
     *  @code
     * 
     *  const LHCb::Particle* p = ... ;
     *
     *  // get the tool 
     *  const IFilterCriterion* filter = ... ;
     *
     *  // construct a predicate 
     *  Cut cut = FILTER( filter ) ;
     *
     *  // use it!
     *  const bool OK = cut ( p ) ;
     *
     *  @endcode
     *
     *  @see IFilterCriterion
     *  @see LoKi::Cuts::FILTER
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-03-20
     */
    typedef LoKi::Particles::Filter                                    FILTER ;
    // ========================================================================
    /** @typedef FROM
     *  The trivial predicate which 
     *  evalautes to "true" if particle comes from the 
     *  decay trees of other particles from the list 
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE particles = ... ;
     * 
     *  Cut cut = FROM ( particles ) ;
     *  
     *  const bool inList = cut( p ) ;
     *
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *
     *  @see LoKi::Particles::IsAParticleFromTree 
     *  @see LoKi::Cuts::FROMTREE
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::IsAParticleFromTree                         FROM ;
    // ========================================================================
    /** @typedef FROMTREE 
     *  The trivial predicate which 
     *  evalautes to "true" if particle comes from the 
     *  decay trees of other particles from the list 
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE particles = ... ;
     * 
     *  Cut cut = FROMTREE ( particles ) ;
     *  
     *  const bool inList = cut( p ) ;
     *
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *
     *  @see LoKi::Particles::IsAParticleFromTree 
     *  @see LoKi::Cuts::FROM
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::IsAParticleFromTree                     FROMTREE ;
    // ========================================================================
    /** @typedef FUNCHILD
     *  Simple adapter function which 
     *  apply the function to a daughter  particle
     * 
     *  @code
     *
     *  const LHCb::Particle* B = ... ;
     *
     *  // the transverse momentum of the first daughter particle:
     *  Fun fun = FUNCHIL ( PT , 1 ) ;
     *  const double pt1 = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::ChildFunction
     *  @see LoKi::Cuts::CHILD
     *  @see LoKi::Cuts::CHILDFUN
     *  @see LoKi::Child::child 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-21
     */
    typedef LoKi::Particles::ChildFunction                           FUNCHILD ;
    // ========================================================================
    /** @var HASCALOS 
     *  the trivial predicate which tests the validity of 
     *  ProtoParticle::calo() 
     * 
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  const bool OK = HASCALOS( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::calo
     *  @see LHCb::CaloHypo
     *  @see LoKi::Particles::ProtoHasCaloHypos
     *  @see LoKi::Cuts::PPHASCALOS
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::ProtoHasCaloHypos                         HASCALOS ;
    // ========================================================================
    /** @typedef HASINFO 
     *  Trivial predicate which evaluates LHCb::Particle::hasInfo
     *  function
     *
     *  It relies on the method LHCb::Particle::hasInfo
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  Cut hasWeight = HASINFO( LHCb::Particle::Weight ) ;
     * 
     *  const bool good = hasWeight( p ) ;
     *
     *  @endcode 
     *
     * 
     *  @see LHCb::Particle
     *  @see LoKi::Particles::HasInfo
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::Particles::HasInfo                                  HASINFO ;
    // ========================================================================
    /** @var HASKEY
     *  The trivial predicate, it relies on Particle::hasKey method 
     *
     *  @code 
     *
     *  const LHCb::Particle* p =  ... ;
     *  const bool good = HASKEY( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see Particle::hasKey
     *  @see KeyedObject::hasKey
     *  @see LoKi::Particles::HasKey
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23
     */   
    const LoKi::Particles::HasKey                                      HASKEY ;
    // ========================================================================
    /** @var HASMUON
     *  the trivial predicate which tests the validity of 
     *  ProtoParticle::muonPID 
     * 
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  const bool OK = HASMUON ( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::muonPID 
     *  @see LHCb::MuonPID 
     *  @see LoKi::Particles::ProtoHasMuonPID 
     *  @see LoKi::Cuts::PPHASMUON
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::ProtoHasMuonPID                            HASMUON ;
    // ========================================================================
    /** @typedef HASPINFO
     *  Trivial predicate which evaluates LHCb::Particle::hasInfo
     *  function
     *
     *  It relies on the method LHCb::Particle::hasInfo
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  Cut hasWeight = HASPINFO( LHCb::Particle::Weight ) ;
     * 
     *  const bool good = hasWeight( p ) ;
     *
     *  @endcode 
     *
     * 
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::HASINFO
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::Particles::HasInfo                                 HASPINFO ;
    // ========================================================================
    /** @var HASPROTO
     *  Trivial predicate which evaluates to true 
     *  for particles with the valid protoparticle 
     *
     *  It relies on the method LHCb::Particle::proto()
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     * 
     *  const bool good = HASPROTO( p ) ;
     *
     *  @endcode 
     *
     * 
     *  @see LHCb::Particle
     *  @see LoKi::Particles::HasProto
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::HasProto                                  HASPROTO ;
    // ========================================================================
    /** @typedef HASPROTOS 
     *  Trivial class which check if the particle has a 
     *  protoparticle from the list 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE protos = ... ;
     *  
     *  Cut hasProtos = HASPROTOS ( protos.begin() , protos.end() ) ;
     *
     *  const bool good = hasProtos( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::HasProtos
     *  @see LHCb::Particle
     *  @see LHCb::Particle::proto
     *  @see LHCb::ProtoParticle
     *  @see LoKi::Cuts::PROTOS
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */ 
    typedef LoKi::Particles::HasProtos                              HASPROTOS ;
    // ========================================================================
    /** @typedef HASPROTOSINTREE
     *  Trivial class which check if the particle (or any of its 
     *  daughter particle) has a 
     *  protoparticle from the list 
     *  
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE protos = ... ;
     *  
     *  Cut hasProtos = HASPROTOSINTREE ( protos.begin() , protos.end() ) ;
     *
     *  const bool good = hasProtos( p ) ;
     *
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LHCb::Particle::proto
     *  @see LHCb::ProtoParticle
     * 
     *  @see LoKi::Particles::HasProtosInTree
     *  @see LoKi::Cuts::PROTOSINTREE
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::HasProtosInTree                  HASPROTOSINTREE ;
    // ========================================================================
    /** @var HASRICH
     *  the trivial predicate which tests the validity of 
     *  ProtoParticle::richPID 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  const bool OK = HASRICH ( p ) ;
     *
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::richPID 
     *  @see LHCb::RichPID 
     *  @see LoKi::Particles::ProtoHasRichPID 
     *  @see LoKi::Cuts::PPHASRICH 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::ProtoHasRichPID                            HASRICH ;
    // ========================================================================
    /** @typedef HASSTATE 
     *  The trivial funtion which evaluates LHCb::Track::hasStateAt
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *
     *  Cut atTT = HASSTATE( LHCb::State::AtTT ) ;  
     * 
     *  const bool OK = atTT ( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see LHCb::Track
     *  @see LHCb::Track::hasStateAt
     *  @see LHCb::State
     *  @see LHCb::State::Location
     *  @see LoKi::Particles::TrackHasState
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    typedef LoKi::Particles::TrackHasState                           HASSTATE ;
    // ========================================================================
    /** @typedef HASTRACKS 
     *  The simple predicate which checks if the particle 
     *  has a track from the specified list of tracks 
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE  tracks = ... ;
     *
     *  Cut hasTracks = HASTRACKS ( tracks.begin() , tracks.end() ) ;
     *  
     *  const bool good = hasTrack ( p ) ;
     * 
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Extract::Particle2Track
     *  @see LHCb::Track
     * 
     *  @see LoKi::Particles::HasTracks
     *  @see LoKi::Cuts::TRACKS
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::HasTracks                              HASTRACKS ;
    // ========================================================================
    /** @typedef HASTRACKSINTREE
     *  The simple predicate which checks if the particle 
     *  has a track from the specified list of tracks 
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE  tracks = ... ;
     *
     *  Cut hasTracks = HASTRACKSINTREE ( tracks.begin() , tracks.end() ) ;
     *  
     *  const bool good = hasTrack ( p ) ;
     * 
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Extract::Particle2Track
     *  @see LHCb::Track
     * 
     *  @see LoKi::Particles::HasTracksInTree
     *  @see LoKi::Cuts::TRACKSINTREE
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::HasTracksInTree                  HASTRACKSINTREE ;
    // ========================================================================
    /** @var HASVERTEX
     *  Trivial predicate which evaluates to true 
     *  for particles with the valid endVertex 
     *
     *  It relies on the method LHCb::Particle::endVertex()
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     * 
     *  const bool good = HASVERTEX( p ) ;
     *
     *  @endcode 
     *
     * 
     *  @see LHCb::Particle
     *  @see LoKi::Particles::HasVertex
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::HasVertex                                HASVERTEX ;
    // ========================================================================
    /** @var ID 
     *  evaluator of the id of the particle 
     *
     *  @code
     *  
     *   const LHCb::Particle* p = ... ;
     *   int pid = (int) ID( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::Identifier
     *  @see LHCb::ParticleID 
     *  @see LHCb::Particle
     *  
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    const LoKi::Particles::Identifier                                      ID ;
    // ========================================================================
    /** @typedef INFO
     *  Trivial function which evaluates LHCb::Particle::info
     *  
     *  It relies on the method LHCb::Particle::info
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *
     *  const int index = ... ;
     *
     *  Fun info = INFO( index , -1000 ) ;
     * 
     *  const double result = info( p ) ;
     *
     *  @endcode 
     *
     * 
     *  @see LHCb::Particle
     *  @see LoKi::Particles::Info
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::Particles::Info                                        INFO ;
    // ========================================================================
    /** @typedef INFOPP 
     *  Trivial function which evaluates LHCb::Particle::info
     *  
     *  It relies on the method LHCb::Particle::info
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  Fun richPi = INFOPP ( LHCb::ProtoParticle::RichPion , -500 , -1000 ) ;
     * 
     *  const  double pion = richPi ( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LHCb::Particle::proto
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::info
     *  @see LHCb::ProtoParticle::additionalInfo
     *  @see LoKi::Cuts::PPINFO
     *  @see LoKi::Cuts::PROTOINFO
     *  @see LoKi::ExtraInfo::info
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::Particles::ProtoInfo                                 INFOPP ;
    // ========================================================================
    /** @typedef INGENERATION 
     *  Simple predicate which checks the  presence of the certain particles
     *  at the fixed level of the decay chain
     *  
     *  @code
     *
     *  const LHCb::Particle* B = ... ;
     * 
     *  const Cut cut = INGENERATION ( "pi+"== ABSID && PT > 1 * GeV , 2 ) ;
     *  // Check the presence of energetic pion as grand-daughter (level 2) 
     *  // in the decay tree of the particle:
     *  const bool OK = cut ( B ) ;
     *  
     *  @endcode 
     *
     *  @attention Level 0 corresponds to the particle itself!
     *
     *  The concept belongs to Patrick KOPPENBURG Patrick.Koppenburg@cern.ch
     * 
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-28
     */
    typedef LoKi::Particles::InGeneration                        INGENERATION ;
    // ========================================================================    
    /** @typedef INTES 
     *
     *  Simple predicate to check if the particle is 
     *  registered in TES at the certain location 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *
     *  Cut ksLL = INTES( "LLKs2PiPi" , false )
     *
     *  const double llK0S = ksLL( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::InTES
     *  @see LoKi::Cuts::PINTES 
     *  
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23
     */   
    typedef LoKi::Particles::InTES                                      INTES ;
    // ========================================================================
    /** @typedef INTREE 
     *  The trivial predicate which evaluates to true 
     *  if there is at least one particle in the decay 
     *  tree of the given particle which satisfies the 
     *  certain criteria
     *
     *  The function uses the algorithm LoKi::PhysAlgs::found 
     *
     *  @code 
     * 
     *  const LHCb::Particle* B = ... ;
     *  
     *  // does the particle has in the decay tree 
     *  // the "phi" with the large transvese momentum?
     *  Cut cut =  INTREE ( ( PT > 1 * GeV ) && ( "phi(1020)" == ID ) ) ;
     *  
     *  const bool hasGoodPhi = cut ( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::InTree 
     *  @see LoKi::PhysAlgs::found 
     *  @see LoKi::Cuts::INTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::InTree                                    INTREE ;
    // ========================================================================
    /** @typedef IP 
     *  Evaluator of impact parameter of 
     *  the particle with respect to vertex/point 
     *
     *  The tool IGeomDispCalculator is used 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  const LHCb::Vertex*   v = ... ;
     * 
     *  Fun ip = IP( v , geo() ) ;
     * 
     *  const double value = ip( p ) ;
     *  
     *  @endcode 
     *  
     *  @see IGeomDispCalculator
     *  @see LoKi::Vertices::ImpParBase
     *  @see LoKi::Vertices::ImpactParamTool
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Particles::ImpPar 
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ImpPar                                        IP ;
    // ========================================================================
    /** @typedef IPCHI2 
     *  Evaluator of chi2 for the impact parameter of 
     *  the particle with respect to vertex/point 
     *
     *  The tool IGeomDispCalculator is used 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  const LHCb::Vertex*   v = ... ;
     * 
     *  Fun chi2 = IPCHI2( v , geo() ) ;
     * 
     *  const double value = chi2( p ) ;
     *  
     *  @endcode 
     *  
     *  @see IGeomDispCalculator
     *  @see LoKi::Vertices::ImpParBase
     *  @see LoKi::Vertices::ImpactParamTool
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::CHI2IP
     *  @see LoKi::Particles::ImpParChi2 
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::ImpParChi2                                IPCHI2 ;
    // ========================================================================
    /** @typedef IPMIN 
     *  Evaluation of minimal value of the impact parameter of 
     *  particle with respect to several vertices 
     *
     *  The tool IGeomDispCalculator is used 
     *
     *  @code 
     *  
     *   const LHCb::Particle* p = ... ;
     *   SEQUENCE vertices  ;
     * 
     *   Fun ipMin = IPMIN( vertices , geo() ) ;
     *   
     *   const double value = ipMin ( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinImpPar
     *  @see IGeomDispCalculator
     *  @see LoKi::Vertices::ImpParBase
     *  @see LoKi::Vertices::ImpactParamTool
     *  @see LoKi::Vertices::ImpPar 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::MINIP
     *  @see LoKi::Cuts::MIP
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinImpPar                                  IPMIN ;
    // ========================================================================
    /** @typedef IS 
     *  The trivial predicate which 
     *  evalautes to "true" if particle 
     *  is in the list of particles 
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE particles = ... ;
     * 
     *  Cut cut = IS( particles ) ;
     *  
     *  const bool inList = cut( p ) ;
     *
     *  @endcode 
     *
     *  @see LHCb::Particle
     *
     *  @see LoKi::Particles::IsAParticle
     *  @see LoKi::Cuts::IN
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::IsAParticle                                   IS ;
    // ========================================================================
    /** @var ISBASIC 
     *  Trivial function which evaluates "weight" for 
     *  Trivial predicate which evaluates to "true" for 
     *  the "basic" particles 
     *
     *  It relies on the method LHCb::Particle::isBasicParticle
     *
     *  @code 
     * 
     *   const LHCb::Particle* p = ... ;
     *   const bool basic = ISBASIC( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Particles::IsBasic
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const   LoKi::Particles::IsBasic                                  ISBASIC ;
    // ========================================================================
    /** @var ISDOWN
     *  The trivial predicate whoch evalauted to true for particles, 
     *  made from "Dowstream" tracks 
     * 
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ; 
     *  
     *  const bool isDown = ISDOWN( p ) ;
     *  
     *  @endcode 
     *   
     *  @see LoKi:Particle
     *  @see LoKi:Track
     *  @see LoKi:Track::Types 
     *  @see LoKi:Track::Downstream
     *  @see LoKi::Cuts::EQUALTO
     *  @see LoKi::Cuts::TRTYPE  
     *  @see LoKi::Cuts::TRDOWN  
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const EQUALTO                 ISDOWN ( TRTYPE , LHCb::Track::Downstream ) ;
    // ========================================================================
    /** @typedef ISINTREE 
     *  The trivial predicate which 
     *  evalautes to "true" if particle or any of dauhter particles 
     *  is in the list of particles 
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE particles = ... ;
     * 
     *  Cut cut = ISINTREE ( particles ) ;
     *  
     *  const bool inList = cut( p ) ;
     *
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *
     *  @see LoKi::Particles::IsAParticelInTree
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::IsAParticleInTree                       ISINTREE ;
    // ========================================================================
    /** @var ISLONG 
     *  The trivial predicate whoch evalauted to true for particles, 
     *  made from "Long" tracks 
     * 
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ; 
     *  
     *  const bool isLong = ISLONG( p ) ;
     *  
     *  @endcode 
     *   
     *  @see LoKi:Particle
     *  @see LoKi:Track
     *  @see LoKi:Track::Types 
     *  @see LoKi:Track::Long
     *  @see LoKi::Cuts::EQUALTO
     *  @see LoKi::Cuts::TRTYPE  
     *  @see LoKi::Cuts::TRLONG
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const EQUALTO                       ISLONG ( TRTYPE , LHCb::Track::Long ) ;
    // ========================================================================
    /** @var ISMUONPID
     *  The simple evaluator of LHCb::MuonPID::isMuon
     *  
     *  @code 
     *
     *  const LHCb::Particle* p = ... ;
     * 
     *  bool muon = ISMUON ( p ) ;
     *
     *  @endcode 
     *     
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::MuonPID 
     *  @see LoKi::Particles::IsMuon
     *  
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @daet 2007-02-25
     */
    const LoKi::Particles::IsMuon                                   ISMUONPID ;
    // ========================================================================
    /** @var ISTTRACK
     *  The trivial predicate whoch evalauted to true for particles, 
     *  made from "Ttrack" tracks 
     * 
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ; 
     *  
     *  const bool isT = ISTTRACK( p ) ;
     *  
     *  @endcode 
     *   
     *  @see LoKi:Particle
     *  @see LoKi:Track
     *  @see LoKi:Track::Types 
     *  @see LoKi:Track::Ttrack
     *  @see LoKi::Cuts::EQUALTO
     *  @see LoKi::Cuts::TRTYPE  
     *  @see LoKi::Cuts::TRTTRACK  
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const EQUALTO                   ISTTRACK ( TRTYPE , LHCb::Track::Ttrack ) ;
    // ========================================================================
    /** @var ISUP
     *  The trivial predicate which evaluates to true for particles, 
     *  made from "Upstream" tracks 
     * 
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ; 
     *  
     *  const bool isLong = ISUP ( p ) ;
     *  
     *  @endcode 
     *   
     *  @see LoKi:Particle
     *  @see LoKi:Track
     *  @see LoKi:Track::Types 
     *  @see LoKi:Track::Upstream
     *  @see LoKi::Cuts::EQUALTO
     *  @see LoKi::Cuts::TRTYPE  
     *  @see LoKi::Cuts::TRUP  
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const EQUALTO                     ISUP ( TRTYPE , LHCb::Track::Upstream ) ;
    // ========================================================================
    /** @var KEY
     *  The trivial predicate, it relies on Particle::hasKey method 
     *
     *  @code 
     *
     *  const LHCb::Particle* p =  ... ;
     *  const bool good = 1335 == KEY( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see Particle::key
     *  @see KeyedObject::key
     *  @see LoKi::Particles::Key
     *  @see LoKi::Cuts::PKEY
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23
     */   
    const LoKi::Particles::Key                                            KEY ;
    // ========================================================================
    /** @var M 
     *  Particle's mass (as <tt>sqrt(E*E-P*P)</tt>) 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    m        = M( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::Mass
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::Mass                                           M ; 
    // ========================================================================
    /** @typedef LIFETIME
     *  The simple functor for evalution of the lifetime of the particle
     *  using the tool, which implements ILifetimeFitter interface
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LIFETIME fun ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltime = fun ( B ) ;
     *
     *  @endcode
     *
     *  @attention Please mind the units! Unlike all other "time" quantities in LoKi,
     *             this functor returns "time"-units instead of "length"-units!
     * 
     *  @see LoKi::Particles::LifeTime
     *  @see LoKi::Cuts::LTIME
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.BElyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTime                               LIFETIME ;
    // ========================================================================
    /** @typedef LIFETIMECHI2
     *  The simple functor for evalution of the lifetime chi2 of the particle
     *  using the tool, which implements ILifetimeFitter interface
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LIFETIMECHI2 chi2 ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltimechi2 = chi2 ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::LifeTimeChi2
     *  @see LoKi::Cuts::LTIMECHI2
     *  @see LoKi::Cuts::LTCHI2
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTimeChi2                       LIFETIMECHI2 ;
    // ========================================================================
    /** @typedef LIFETIMEFITCHI2
     *  The simple functor for evalution of the chi2 of the "lifetime fit"
     *  of the particle using the tool, which implements ILifetimeFitter 
     *  interface
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LIFETIMEFITCHI2 chi2 ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltimefit = chi2 ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::LifeTimeFitChi2
     *  @see LoKi::Cuts::LTCHI2
     *  @see LoKi::Cuts::LTIMECHI2
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTimeFitChi2                 LIFETIMEFITCHI2 ;
    // ========================================================================
    /** @typedef LIFETIMESIGNCHI2
     *  The simple functor for evalution of the SIGNED lifetime chi2 of the particle
     *  using the tool, which implements ILifetimeFitter interface.
     *  The signs is prescribed according teh sign of lifetiem itself 
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LIFETIMESIGNCHI2 chi2 ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltimechi2 = chi2 ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::LifeTimeSignedChi2
     *  @see LoKi::Cuts::LTSIGNCHI2
     *  @see LoKi::Cuts::LTIMESIGNCHI2
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTimeSignedChi2             LIFETIMESIGNCHI2 ;
    // ========================================================================
    /** @typedef LTCHI2
     *  The simple functor for evalution of the lifetime chi2 of the particle
     *  using the tool, which implements ILifetimeFitter interface
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LTCHI2 chi2 ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltimechi2 = chi2 ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::LifeTimeChi2
     *  @see LoKi::Cuts::LTIMECHI2
     *  @see LoKi::Cuts::LIFETIMECHI2
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTimeChi2                             LTCHI2 ;
    // ========================================================================
    /** @typedef LTFITCHI2
     *  The simple functor for evalution of the chi2 of the "lifetime fit"
     *  of the particle using the tool, which implements ILifetimeFitter 
     *  interface
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LTFITCHI2 chi2 ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltimefit = chi2 ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::LifeTimeFitChi2
     *  @see LoKi::Cuts::LTIMECHI2
     *  @see LoKi::Cuts::LIFETIMECHI2
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTimeFitChi2                       LTFITCHI2 ;
    // ========================================================================
    /** @typedef LTIMECHI2
     *  The simple functor for evalution of the lifetime chi2 of the particle
     *  using the tool, which implements ILifetimeFitter interface
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LTIMECHI2 chi2 ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltimechi2 = chi2 ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::LifeTimeChi2
     *  @see LoKi::Cuts::LTCHI2
     *  @see LoKi::Cuts::LIFETIMECHI2
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTimeChi2                          LTIMECHI2 ;
    // ========================================================================
    /** @typedef LTIMEFITCHI2
     *  The simple functor for evalution of the chi2 of the "lifetime fit"
     *  of the particle using the tool, which implements ILifetimeFitter 
     *  interface
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LTIMEFITCHI2 chi2 ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltimefit = chi2 ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::LifeTimeFitChi2
     *  @see LoKi::Cuts::LTCHI2
     *  @see LoKi::Cuts::LIFETIMECHI2
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTimeFitChi2                     LTIMEFITCHI2 ;
    // ========================================================================
    /** @typedef LTIME
     *  The simple functor for evalution of the lifetime of the particle
     *  using the tool, which implements ILifetimeFitter interface
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LTIME fun ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltime = fun ( B ) ;
     *
     *  @endcode
     *
     *  @attention Please mind the units! Unlike all other "time" quantities in LoKi,
     *             this functor returns "time"-units instead of "length"-units!
     * 
     *  @see LoKi::Particles::LifeTime
     *  @see LoKi::Cuts::LIFETIME
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.BElyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTime                                  LTIME ;
    // ========================================================================
    /** @typedef LTIMESIGNCHI2
     *  The simple functor for evalution of the SIGNED lifetime chi2 of the particle
     *  using the tool, which implements ILifetimeFitter interface.
     *  The signs is prescribed according teh sign of lifetiem itself 
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LTIMESIGNCHI2 chi2 ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltimechi2 = chi2 ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::LifeTimeSignedChi2
     *  @see LoKi::Cuts::LTSIGNCHI2
     *  @see LoKi::Cuts::LIFETIMESIGNCHI2
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTimeSignedChi2                LTIMESIGNCHI2 ;
    // ========================================================================
    /** @typedef LTSIGNCHI2
     *  The simple functor for evalution of the SIGNED lifetime chi2 of the particle
     *  using the tool, which implements ILifetimeFitter interface.
     *  The signs is prescribed according teh sign of lifetiem itself 
     *
     *  @code
     *
     *  // the primary vertex:
     *  const LHCb::VertexBase* pv      = ...;
     *  // the fitting tool itself:
     *  const ILifetimeFitter*  fitter  = ... ;
     *
     *  // create the functor:
     *  const LTSIGNCHI2 chi2 ( pv , fitter ) ;
     *
     *  // get B-candidate:
     *  const LHCb::Particle* B = ... ;
     *
     *  // use the functor:
     *  const double ltimechi2 = chi2 ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Particles::LifeTimeSignedChi2
     *  @see LoKi::Cuts::LTIMESIGNCHI2
     *  @see LoKi::Cuts::LIFETIMESIGNCHI2
     *  @see ILifetimeFitter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2008-01-24
     */
    typedef  LoKi::Particles::LifeTimeSignedChi2                   LTSIGNCHI2 ;
    // ========================================================================
    /** @typedef LV0 
     *
     *  For partice, which has a some daughter particles, 
     *  it evaluates the cosine of the angle between daughter's 
     *  momentum and mother flight direction in the rest 
     *  system of mother particle. For 2-body decays it is just 
     *  a polarization angle of mother particle.
     *  
     *  The name comes from KAL language by H.Algrecht 
     *   used in ARGUS collaboaration
     *
     *  @see LoKi::Particles::DecayAngle 
     *
     *  @author Vanya Belyaev ibelyaev@physics.syr.edu
     *  @date   2002-11-02
     */
    typedef LoKi::Particles::DecayAngle                                   LV0 ;
    // ========================================================================
    /** @var LV01
     *
     *  For partice, which has a some daughter particles, 
     *  it evaluates the cosine of the angle between the first 
     *  daughter's momentum and mother flight direction in the rest 
     *  system of mother particle. 
     *  
     *  The name comes from KAL language by H.Algrecht 
     *   used in ARGUS collaboaration
     *
     *  @see LoKi::Cuts::LV0 
     *  @see LoKi::Particles::DecayAngle 
     *
     *  @author Vanya Belyaev ibelyaev@physics.syr.edu
     *  @date   2002-11-02
     */
    const LoKi::Particles::DecayAngle                              LV01 ( 1 ) ;
    // ========================================================================
    /** @var LV02
     *
     *  For partice, which has a some daughter particles, 
     *  it evaluates the cosine of the angle between the second
     *  daughter's momentum and mother flight direction in the rest 
     *  system of mother particle. 
     *  
     *  The name comes from KAL language by H.Algrecht 
     *   used in ARGUS collaboaration
     *
     *  @see LoKi::Cuts::LV0  
     *  @see LoKi::Particles::DecayAngle 
     *
     *  @author Vanya Belyaev ibelyaev@physics.syr.edu
     *  @date   2002-11-02
     */
    const LoKi::Particles::DecayAngle                              LV02 ( 2 ) ;
    // ========================================================================
    /** @var LV03
     *
     *  For partice, which has a some daughter particles, 
     *  it evaluates the cosine of the angle between the third
     *  daughter's momentum and mother flight direction in the rest 
     *  system of mother particle. 
     *  
     *  The name comes from KAL language by H.Algrecht 
     *   used in ARGUS collaboaration
     *
     *  @see LoKi::Cuts::LV0 
     *  @see LoKi::Particles::DecayAngle 
     * 
     *  @author Vanya Belyaev ibelyaev@physics.syr.edu
     *  @date   2002-11-02
     */
    const LoKi::Particles::DecayAngle                              LV03 ( 3 ) ;
    // ========================================================================
    /** @var LV04
     *
     *  For partice, which has a some daughter particles, 
     *  it evaluates the cosine of the angle between  the fourth
     *  daughter's momentum and mother flight direction in the rest 
     *  system of mother particle. 
     *  
     *  The name comes from KAL language by H.Algrecht 
     *   used in ARGUS collaboaration
     *
     *  @see LoKi::Cuts::LV0 
     *  @see LoKi::Particles::DecayAngle 
     *
     *  @author Vanya Belyaev ibelyaev@physics.syr.edu
     *  @date   2002-11-02
     */
    const LoKi::Particles::DecayAngle                              LV04 ( 4 ) ;
    // ========================================================================
    /** @var M0 
     *  Particle's mass (as <tt>sqrt(E*E-P*P)</tt>) 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    m        = M0 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::Mass
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::Mass                                          M0 ;
    // ========================================================================
    /** @var M1 
     *  The mass of the first daughter 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    m1      = M1 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::Mass
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::InvariantMass                           M1 ( 1 ) ;
    // ========================================================================
    /** @var M12 
     *  invariant mass of the first and the second daughter particles 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     // evaluate the invarinat mass of 1st and 2nd daugter particles 
     *     const double  m12  = M12( particle ) ;
     *
     *  @endcode
     *  
     *  @see Particle
     *  @see LoKi::Particle::InvariantMass
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const LoKi::Particles::InvariantMass                        M12 ( 1 , 2 ) ;
    // ========================================================================
    /** @var M13 
     *  invariant mass of the first and the third daughter particles 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     // evaluate the invarinat mass of 1st and 3rd daugter particles 
     *     const double  m13  = M13( particle ) ;
     *
     *  @endcode
     *  
     *  @see Particle
     *  @see LoKi::Particle::InvariantMass
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const LoKi::Particles::InvariantMass                        M13 ( 1 , 3 ) ;
    // ========================================================================
    /** @var M14 
     *  invariant mass of the first and the fourth daughter particles 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     // evaluate the invarinat mass of 1st and 4th daugter particles 
     *     const double  m14  = M14( particle ) ;
     *
     *  @endcode
     *  
     *  @see Particle
     *  @see LoKi::Particle::InvariantMass
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const LoKi::Particles::InvariantMass                        M14 ( 1 , 4 ) ;
    // ========================================================================
    /** @var M2 
     *  The mass of the second daughter 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    m2      = M2 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::Mass
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::InvariantMass                           M2 ( 2 ) ;
    // ========================================================================
    /** @var M23 
     *  invariant mass of the second and the third daughter particles 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     // evaluate the invarinat mass of 2nd and 3rd daugter particles 
     *     const double  m23  = M23( particle ) ;
     *
     *  @endcode
     *  
     *  @see Particle
     *  @see LoKi::Particle::InvariantMass
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const LoKi::Particles::InvariantMass                        M23 ( 2 , 3 ) ;
    // ========================================================================
    /** @var M24 
     *  invariant mass of the second and the fourth daughter particles 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     // evaluate the invarinat mass of 2nd and 4th daugter particles 
     *     const double  m24  = M24( particle ) ;
     *
     *  @endcode
     *  
     *  @see Particle
     *  @see LoKi::Particle::InvariantMass
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10 
     */
    const LoKi::Particles::InvariantMass                        M24 ( 2 , 4 ) ;
    // ========================================================================
    /** @var M3 
     *  The mass of the third daughter 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    m3       = M3 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::Mass
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::InvariantMass                           M3 ( 3 ) ;
    // ========================================================================
    /** @var M34 
     *  invariant mass of the third and the fourth daughter particles 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     // evaluate the invarinat mass of 3rd and 4th daugter particles 
     *     const double  m34  = M34( particle ) ;
     *
     *  @endcode
     *  
     *  @see Particle
     *  @see LoKi::Particle::InvariantMass
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const LoKi::Particles::InvariantMass                        M34 ( 3 , 4 ) ;
    // ========================================================================
    /** @var M4 
     *  The mass of the fourth daughter 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    m4       = M4 ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::Mass
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::InvariantMass                           M4 ( 4 ) ;
    // ========================================================================
    /** @typedef MASS
     *  Invariant mass of subcombination of dauhter particles 
     *  
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     // evaluate the invarinat mass of 1st and 2nd daugter particles 
     *     Fun m12 = MASS(1,2) 
     *     const double  mass  = m12( particle ) ;
     *
     *  @endcode
     * 
     *  Up to 3 indices could be used with explicit constructors 
     *  and unlimited number of indices could be used with 
     *  constructor from the vector of indices 
     * 
     *  @see Particle
     *  @see LoKi::Particle::InvariantMass
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    typedef LoKi::Particles::InvariantMass                               MASS ;
    // ========================================================================
    /** @typedef MAXTREE 
     *  The trivial algorithm which scans the decay 
     *  tree of the particle and searches for the the 
     *  minimal value for some functions for
     *  particles which satisfy the certain criteria 
     *
     *  The function uses the algorithm LoKi::PhysAlgs::max_value 
     *
     *  @code 
     *
     *  const LHCb::Particle* B = ... ;
     *  
     *  // the maximal value of chi2 of vertex fit  in the decay tree
     *  Fun fun = MAXTREE ( VFUNASPFUN( VCHI2 ) , HASVERTEX ) ;
     * 
     *  const double maxChi2 = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::PhysAlgs::max_value  
     *  @see LoKi::Cuts::TREEMAX 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::MaxTree                                  MAXTREE ;
    // ========================================================================
    /** @typedef MINCLAPP 
     *  Evaluator of the minimal closest approach 
     *  distance between the particle and sequence of other particles  
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  
     *  SEQUENCE parts = ... ;
     * 
     *  Cun clappr = MINCLAPP( parts , geo() ) ;
     *  
     *  const double minclApp  = clappr( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinClosestApproach
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::MINDOCA
     *  @see LoKi::Cuts::DOCAMIN
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinClosestApproach                      MINCLAPP ;
    // ========================================================================
    /** @typedef MINCLAPPCHI2 
     *  Evaluator of the minimal closest approach chi2  
     *  distance between the particle and sequence of other particles  
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  
     *  SEQUENCE parts = ... ;
     * 
     *  Cun clappr = MINCLAPPCHI2( parts , geo() ) ;
     *  
     *  const double chi2 = clappr( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinClosestApproachChi2
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::MINDOCACHI2
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinClosestApproachChi2              MINCLAPPCHI2 ;
    // ========================================================================
    /** @typedef MINDOCA
     *  Evaluator of the minimal closest approach 
     *  distance between the particle and sequence of other particles  
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  
     *  SEQUENCE parts = ... ;
     * 
     *  Cun clappr = MINDOCA ( parts , geo() ) ;
     *  
     *  const double minclApp  = clappr( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinClosestApproach
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::MINCLAPP
     *  @see LoKi::Cuts::DOCAMIN
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinClosestApproach                       MINDOCA ;
    // ========================================================================
    /** @typedef MINDOCACHI2 
     *  Evaluator of the minimal closest approach chi2  
     *  distance between the particle and sequence of other particles  
     *  
     *  The tool IGeomDispCalculator is used 
     *  for evaluation
     *
     *  @code 
     *
     *  const LHCb:Particle* K1 = ... ;
     *  
     *  SEQUENCE parts = ... ;
     * 
     *  Cun clappr = MINDOCACHI2( parts , geo() ) ;
     *  
     *  const double chi2 = clappr( K2 ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinClosestApproachChi2
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::MINCLAPPCHI2
     *  @see IGeomDispCalculator
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinClosestApproachChi2               MINDOCACHI2 ;
    // ========================================================================
    /** @typedef MINIP 
     *  Evaluation of minimal value of the impact parameter of 
     *  particle with respect to several vertices 
     *
     *  The tool IGeomDispCalculator is used 
     *
     *  @code 
     *  
     *   const LHCb::Particle* p = ... ;
     *   SEQUENCE vertices  ;
     * 
     *   Fun ipMin = MINIP ( vertices , geo() ) ;
     *   
     *   const double value = ipMin ( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinImpPar
     *  @see IGeomDispCalculator
     *  @see LoKi::Vertices::ImpParBase
     *  @see LoKi::Vertices::ImpactParamTool
     *  @see LoKi::Vertices::ImpPar 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::IPMIN
     *  @see LoKi::Cuts::MIP
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinImpPar                                  MINIP ;
    // ========================================================================
    /** @typedef MINTREE 
     *  The trivial algorithm which scans the decay 
     *  tree of the particle and searches for the the 
     *  minimal value for some functions for
     *  particles which satisfy the certain criteria 
     *
     *  The function uses the algorithm LoKi::PhysAlgs::min_value 
     *
     *  @code 
     * 
     *  const LHCb::Particle* B = ... ;
     * 
     *  // minimal transverse momentum of the track in the decay tree:
     *  Fun fun = MINTREE( HASTRACK , PT ) ;
     * 
     *  const double minPT = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::PhysAlgs::min_value  
     *  @see LoKi::Cuts::TREEMIN 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::MinTree                                  MINTREE ;
    // ========================================================================
    /** @typedef MINVD 
     *  Evaluator of the minimal GEOMETRY distance between the particle 
     *  "endVertex" and some set of vertices  
     *
     *  @code 
     *
     *  const LHCb::Particle* p = ... ;
     * 
     *  const LHCb::PrimVertices* pv = ... ;
     * 
     *  Fun dist = MINVD( pv ) ;
     * 
     *  const double minDistance = dist( p ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi:Particles::MinVertexDistance
     *  @see LHCb::Vertex
     *  @see LHCb::Particle
     *  @see LoKi:Cuts::VDMIN
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinVertexDistance                         MINVD ;
    // =======================================================================
    /** @typedef MINVDCHI2 
     *  Evaluator of the minimal chi2 for distance between the particle 
     *  "endVertex" and some set of vertices  
     *
     *  @code 
     *
     *  const LHCb::Particle* p = ... ;
     * 
     *  const LHCb::PrimVertices* pv = ... ;
     * 
     *  Fun dist = MINVDCHI2( pv ) ;
     * 
     *  const double chi2  = dist( p ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Particles::MinVertexChi2Distance 
     *  @see LHCb::Vertex
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::VDMINCHI2 
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinVertexChi2Distance                  MINVDCHI2 ;
    // ========================================================================
    /** @typedef MIP 
     *  Evaluation of minimal value of the impact parameter of 
     *  particle with respect to several vertices 
     *
     *  The tool IGeomDispCalculator is used 
     *
     *  @code 
     *  
     *   const LHCb::Particle* p = ... ;
     *   SEQUENCE vertices  ;
     * 
     *   Fun ipMin = MIP( vertices , geo() ) ;
     *   
     *   const double value = ipMin ( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinImpPar
     *  @see IGeomDispCalculator
     *  @see LoKi::Vertices::ImpParBase
     *  @see LoKi::Vertices::ImpactParamTool
     *  @see LoKi::Vertices::ImpPar 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::MINIP
     *  @see LoKi::Cuts::IPMIN
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinImpPar                                    MIP ;
    // ========================================================================
    /** @typedef MIPCHI2 
     *  Evaluation of minimal value of chi2 for the impact parameter of 
     *  particle with respect to several vertices 
     *
     *  The tool IGeomDispCalculator is used 
     *
     *  @code 
     *  
     *   const LHCb::Particle* p = ... ;
     *   SEQUENCE vertices  ;
     * 
     *   Fun ipMin = MIPCHI2 ( vertices , geo() ) ;
     *   
     *   const double value = ipMin ( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::MinImpParChi2
     *  @see IGeomDispCalculator
     *  @see LoKi::Vertices::ImpParBase
     *  @see LoKi::Vertices::ImpactParamTool
     *  @see LoKi::Vertices::ImpPar 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::CHI2MIP
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinImpParChi2                            MIPCHI2 ;
    // ========================================================================
    /** @var MM 
     *  Particle's measured mass 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    mm       = MM( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::MeasuredMass
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::MeasuredMass                                  MM ;
    // ========================================================================
    /** @typedef MULTTREE 
     *  The simple function which accumulated the 
     *  value of certain function over the decay 
     *  tree of the particle for the articles which 
     *  satisfy the certain selection criteria
     *  Accumulation is done throught summation
     *
     *  The function uses the algorithm LoKi::PhysAlgs::accumulate
     *
     *  @code 
     * 
     *  const LHCb::Particle* B = ... ;
     *  
     *  // get the product of "weight" 
     *  // for all non-basic particles in the tree 
     *  Fun fun = MULTTREE( W , !BASIC , 1.0 ) ;
     *  
     *  const double weight = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::PhysAlgs::accumulate
     *  @see LoKi::Cuts::TREEMULT 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::MultTree                                MULTTREE ;
    // ========================================================================
    /** @var NDAUGHTERS 
     *  Trivial function which evaluates number of daughters for 
     *  the particle
     *
     *  It relies on the method LHCb::Particle::daughters().size()
     *
     *  @code 
     * 
     *   const LHCb::Particle* p = ... ;
     *   const int nDau = NDAUGHTERS  ( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Cuts::NDAUGS 
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::NumberOfDaughters
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::NumberOfDaughters                       NDAUGHTERS ;
    // ========================================================================
    /** @var NDAUGS 
     *  Trivial function which evaluates number of daughters for 
     *  the particle
     *
     *  It relies on the method LHCb::Particle::daughters().size()
     *
     *  @code 
     * 
     *   const LHCb::Particle* p = ... ;
     *   const int nDau = NDAUGS ( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::NumberOfDaughters
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::NumberOfDaughters                           NDAUGS ;
    // ========================================================================
    /** @typedef NINGENERATION 
     *  Simple functon which counts the certain particles
     *  at the fixed level of the decay chain
     *  
     *  @code
     *
     *  const LHCb::Particle* B = ... ;
     * 
     *  const Cut cut = 2 == NINGENERATION ( "pi+"== ABSID && PT > 1 * GeV , 2 ) ;
     *  // Check the presence of exctly two energetic pions as 
     *  // grand-daughters (level 2) in the decay tree of the particle:
     *  const bool OK = cut ( B ) ;
     *  
     *  @endcode 
     *
     *  @attention Level 0 corresponds to the particle itself!
     *
     *  The concept belongs to Patrick KOPPENBURG Patrick.Koppenburg@cern.ch
     * 
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-28
     */
    typedef LoKi::Particles::NinGeneration                      NINGENERATION ;
    // ========================================================================
    /** @typedef NINTREE 
     *  The simple function which evaluates the number 
     *  of particle in decay tree which satisfies the certan criteria 
     * 
     *  The function uses the algorithm LoKi::PhysAlgs::count_if 
     *
     *  @code 
     * 
     *  const LHCb::Particle* B = ... ;
     *  
     *  // does the particle has in the decay tree 
     *  // pion with the large transvese momentum?
     *  Fun fun =  ( PT > 1 * GeV ) && ( "pi+" == ABSID ) ;
     *  
     *  const bool good = fun( B ) == 2 ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::NinTree 
     *  @see LoKi::PhysAlgs::count_if 
     *  @see LoKi::Cuts::NINTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::NinTree                                  NINTREE ;
    // ========================================================================
    /** @var NONE
     *  the trivial functor which always evaluates to "false"
     *  
     *  @see LoKi::BooleanConstant
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,bool>          NONE ( false ) ;
    // ========================================================================
    /** @var ONE
     *  the trivial functor which always evaluates to 1
     *  
     *  @see LoKi::Constant
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,double>              ONE ( 1  ) ;
    // ========================================================================
    /** @var P 
     *  Particle's momentum 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    p        = P( particle ) ;
     *
     *  @endcode
     *
     *  @see LHCb::Particle
     *  @see LoKi::Particle::Momentum 
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::Momentum                                       P ;
    // ========================================================================
    /** @var P2
     *  Particle's momentum squared
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    p2       = P2( particle ) ;
     *
     *  @endcode
     *
     *  @see LHCb::Particle
     *  @see LoKi::Particle::Momentum2 
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::Momentum2                                     P2 ;
    // ========================================================================
    /** @var PALL
     *  the trivial functor which always evaluates to "true"
     *  
     *  @see LoKi::BooleanConstant
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::ALL
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,bool>           PALL ( true ) ;
    // ========================================================================
    /** @var PFALSE
     *  the trivial functor which always evaluates to "false"
     *  
     *  @see LoKi::BooleanConstant
     *  @see LHCb::Particle
     *  @see LHCb::Cuts::NONE 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,bool>       PFALSE ( false ) ;
    // ========================================================================
    /** @typedef PHASINFO 
     *  Trivial predicate which evaluates LHCb::Particle::hasInfo
     *  function
     *
     *  It relies on the method LHCb::Particle::hasInfo
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  Cut hasWeight = PHASINFO( LHCb::Particle::Weight ) ;
     * 
     *  const bool good = hasWeight( p ) ;
     *
     *  @endcode 
     *
     * 
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::HASINFO
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::Particles::HasInfo                                 PHASINFO ;
    // ========================================================================
    /** @var PHI
     *  Particle asimuthal angle 
     * 
     *  @code
     * 
     *   const Particle* p    = ...         ;
     *   const double      phi  = PHI(  p ) ;
     *
     *  @endcode 
     *
     *  @see Particle
     *  @see LoKi::Particles::Phi
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-03-01
     */
    const LoKi::Particles::Phi                                            PHI ;
    // ========================================================================
    /** @var PID 
     *  evaluator of the id of the particle 
     *
     *  @code
     *  
     *   const LHCb::Particle* p = ... ;
     *   int pid = (int) PID( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::Identifier
     *  @see LHCb::ParticleID 
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::ID
     *  
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    const LoKi::Particles::Identifier                                     PID ;
    // ========================================================================
    /** @var PIDe 
     *  Combined delta log-likelihood  for electron hypothesis 
     *  
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ;
     *  
     *  conds double DLL = PIDe ( p ) ;
     *
     *  @endcode  
     *  
     *  @see LoKi::Cuts::PPINFO
     *  @see LoKi::Particles::ProtoInfo
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::info
     *  @see LHCb::ProtoParticle::additionalInfo
     *  @see LHCb::ProtoParticle::LkhPIDe 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const PPINFO          PIDe ( LHCb::ProtoParticle::CombDLLe  , 0 , -1000 ) ;
    // ========================================================================
    /** @var PIDk
     *  Combined delta log-likelihood  for kaon hypothesis 
     *
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ;
     *  
     *  const double DLL = PIDk ( p ) ;
     *
     *  @endcode  
     *  
     *  @see LoKi::Cuts::PPINFO
     *  @see LoKi::Particles::ProtoInfo
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::info
     *  @see LHCb::ProtoParticle::additionalInfo
     *  @see LHCb::ProtoParticle::LkhPIDK
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const PPINFO          PIDk ( LHCb::ProtoParticle::CombDLLk  , 0 , -1000 ) ;
    // ========================================================================
    /** @var PIDK
     *  Combined delta log-likelihood  for kaon hypothesis 
     *
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ;
     *  
     *  const double DLL = PIDK ( p ) ;
     *
     *  @endcode  
     *  
     *  @see LoKi::Cuts::PPINFO
     *  @see LoKi::Particles::ProtoInfo
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::info
     *  @see LHCb::ProtoParticle::additionalInfo
     *  @see LHCb::ProtoParticle::LkhPIDK
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const PPINFO          PIDK ( LHCb::ProtoParticle::CombDLLk  , 0 , -1000 ) ;
    // ========================================================================
    /** @var PIDmu
     *  Combined delta log-likelihood  for muon hypothesis 
     *
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ;
     *  
     *  const double DLL = PIDmu ( p ) ;
     *
     *  @endcode  
     *  
     *  @see LoKi::Cuts::PPINFO
     *  @see LoKi::Particles::ProtoInfo
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::info
     *  @see LHCb::ProtoParticle::additionalInfo
     *  @see LHCb::ProtoParticle::LkhPIDmu
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const PPINFO        PIDmu ( LHCb::ProtoParticle::CombDLLmu  , 0 , -1000 ) ;
    // ========================================================================
    /** @var PIDp
     *  Combined delta log-likelihood  for proton hypothesis 
     *
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ;
     *  
     *  const double DLL = PIDp ( p ) ;
     *
     *  @endcode  
     *  
     *  @see LoKi::Cuts::PPINFO
     *  @see LoKi::Particles::ProtoInfo
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::info
     *  @see LHCb::ProtoParticle::additionalInfo
     *  @see LHCb::ProtoParticle::LkhPIDp
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const PPINFO          PIDp ( LHCb::ProtoParticle::CombDLLp  , 0 , -1000 ) ;
    // ========================================================================
    /** @var PIDpi
     *  Combined delta log-likelihood  for pion hypothesis 
     *
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ;
     *  
     *  const double DLL = PIDpi ( p ) ;
     *
     *  @endcode  
     *  
     *  @see LoKi::Cuts::PPINFO
     *  @see LoKi::Particles::ProtoInfo
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::info
     *  @see LHCb::ProtoParticle::additionalInfo
     *  @see LHCb::ProtoParticle::LkhPIDmu
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const PPINFO        PIDpi ( LHCb::ProtoParticle::CombDLLpi  , 0 , -1000 ) ;
    // ========================================================================
    /** @typedef PINFO 
     *  Trivial function which evaluates LHCb::Particle::info
     *  
     *  It relies on the method LHCb::Particle::info
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *
     *  const int index = ... ;
     *
     *  Fun info = PINFO( index , -1000 ) ;
     * 
     *  const double result = info( p ) ;
     *
     *  @endcode 
     *
     * 
     *  @see LoKi::Cuts::INFO
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::Particles::Info                                       PINFO ;
    // ========================================================================
    /** @typedef PINTES 
     *
     *  Simple predicate to check if the particle is 
     *  registered in TES at the certain location 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *
     *  Cut ksLL = PINTES( "LLKs2PiPi" , false )
     *
     *  const double llK0S = ksLL( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::InTES
     *  @see LoKi::Cuts::INTES 
     *  
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23
     */   
    typedef LoKi::Particles::InTES                                     PINTES ;
    // ========================================================================
    /** @var PKEY 
     *  The trivial function, it relies on Particle::key method 
     *
     *  @code 
     *
     *  const LHCb::Particle* p =  ... ;
     *  Fun key = PKEY( -1 , -3 ) ;
     *
     *  const bool good = 1335 == key( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see Particle::key
     *  @see KeyedObject::key
     *  @see LoKi::Particles::Key
     *  @see LoKi::Cuts::KEY
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23
     */   
    const LoKi::Particles::Key                                           PKEY ;
    // ========================================================================
    /** @typedef PLOT
     *  Simple monitoring histogram for the functions
     *
     *  @code 
     *
     *  // some function to be monitored
     *  Fun fun = ... ;
     *
     *  // Create the monitored function using the histogram:
     *  AIDA::IHistogram1D* histo = ... ;
     *  Fun mon = PLOT ( fun , histo ) ;
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
     *  Fun fun = ... ;
     *
     *  AIDA::IHistogram1D* histo = ... ;
     *  Fun mon = monitor ( fun , histo ) ;
     *  
     *  @endcode 
     *
     *  The substitution of the function by monitored function 
     *  cound be done "on-flight" without the disturbing of the
     *  actual processing:
     *  @code
     *
     *  // some function to be monitored
     *  Fun fun = ... ;
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
    typedef LoKi::Monitoring::Plot<const LHCb::Particle*,double>         PLOT ;
    // ========================================================================
    /** @var PNONE
     *  the trivial functor which always evaluates to "false"
     *  
     *  @see LoKi::BooleanConstant
     *  @see LHCb::Particle
     *  @see LHCb::Cuts::NONE 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,bool>        PNONE ( false )  ;
    // ========================================================================
    /** @var PONE
     *  the trivial functor which always evaluates to 1
     *  
     *  @see LoKi::Cuts::ONE
     *  @see LoKi::Constant
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,double>            PONE ( 1 ) ;
    // ========================================================================
    /** @var PPHASCALOS 
     *  the trivial predicate which tests the validity of 
     *  ProtoParticle::calo() 
     * 
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  const bool OK = PPHASCALOS( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::calo
     *  @see LHCb::CaloHypo
     *  @see LoKi::Particles::ProtoHasCaloHypos
     *  @see LoKi::Cuts::HASCALOS
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::ProtoHasCaloHypos                       PPHASCALOS ;
    // ========================================================================
    /** @typedef PPHASINFO 
     *  The trivial predicate which evaluates for true 
     *  if the protoparticle "hasInfo".
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  Cut hasPhotonID = PPHASINFO( LHCb::ProtoParticle::PhotonID ) ;
     * 
     *  const bool OK = hasPhotonID( p ) ;
     *  
     *  @endcode 
     *
     *
     *  @see LHCb::Particle
     *  @see LHCb::Particle::proto
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::hasInfo
     *
     *  @see LoKi::Particles::ProtoHasInfo 
     *  @see LoKi::ExtraInfo::hasInfo
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    typedef LoKi::Particles::ProtoHasInfo                           PPHASINFO ;
    // ========================================================================
    /** @var PPHASMUON
     *  the trivial predicate which tests the validity of 
     *  ProtoParticle::muonPID 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  const bool OK = PPHASMUON ( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::muonPID 
     *  @see LHCb::MuonPID 
     *  @see LoKi::Particles::ProtoHasMuonPID 
     *  @see LoKi::Cuts::HASMUON
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::ProtoHasMuonPID                     PPHASMUON ;
    // ========================================================================
    /** @var PPHASRICH
     *  the trivial predicate which tests the validity of 
     *  ProtoParticle::richPID 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  const bool OK = PPHASRICH ( p ) ;
     *
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::richPID 
     *  @see LHCb::RichPID 
     *  @see LoKi::Particles::ProtoHasRichPID 
     *  @see LoKi::Cuts::HASRICH 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Particles::ProtoHasRichPID                          PPHASRICH ;
    // ========================================================================
//     /** @typedef PPINFO 
//      *  Trivial function which evaluates LHCb::Particle::info
//      *  
//      *  It relies on the method LHCb::Particle::info
//      *
//      *  @code 
//      *  
//      *  const LHCb::Particle* p = ... ;
//      *  
//      *  Fun richPi = PPINFO( LHCb::ProtoParticle::RichPion , -500 , -1000 ) ;
//      * 
//      *  const  double pion = richPi ( p ) ;
//      *  
//      *  @endcode 
//      *
//      *  @see LHCb::Particle
//      *  @see LHCb::Particle::proto
//      *  @see LHCb::ProtoParticle
//      *  @see LHCb::ProtoParticle::info
//      *  @see LHCb::ProtoParticle::additionalInfo
//      *  @see LoKi::Cuts::PROTOINFO
//      *  @see LoKi::ExtraInfo::info
//      *
//      *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
//      *  @date 2006-02-15
//      */
//     typedef LoKi::Particles::ProtoInfo                                 PPINFO ;
//     // ========================================================================
    /** @typedef PROTOINFO 
     *  Trivial function which evaluates LHCb::Particle::info
     *  
     *  It relies on the method LHCb::Particle::info
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  Fun richPi = PROTOINFO( LHCb::ProtoParticle::RichPion , -500 , -1000 ) ;
     * 
     *  const  double pion = richPi ( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LHCb::Particle::proto
     *  @see LHCb::ProtoParticle
     *  @see LHCb::ProtoParticle::info
     *  @see LHCb::ProtoParticle::additionalInfo
     *  @see LoKi::Cuts::PPINFO
     *  @see LoKi::ExtraInfo::info
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::Particles::ProtoInfo                              PROTOINFO ;
    // ========================================================================
    /** @typedef PROTOS 
     *  Trivial class which check if the particle has a 
     *  protoparticle from the list 
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE protos = ... ;
     *  
     *  Cut hasProtos = PROTOS ( protos.begin() , protos.end() ) ;
     *
     *  const bool good = hasProtos( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::HasProtos
     *  @see LHCb::Particle
     *  @see LHCb::Particle::proto
     *  @see LHCb::ProtoParticle
     *  @see LoKi::Cuts::HASPROTOS
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */ 
    typedef LoKi::Particles::HasProtos                                 PROTOS ;
    // ========================================================================
    /** @typedef PROTOSINTREE
     *  Trivial class which check if the particle (or any of its 
     *  daughter particle) has a 
     *  protoparticle from the list 
     *  
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE protos = ... ;
     *  
     *  Cut hasProtos = PROTOSINTREE ( protos.begin() , protos.end() ) ;
     *
     *  const bool good = hasProtos( p ) ;
     *
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LHCb::Particle::proto
     *  @see LHCb::ProtoParticle
     * 
     *  @see LoKi::Particles::HasProtosInTree
     *  @see LoKi::Cuts::HASPROTOSINTREE
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::HasProtosInTree                     PROTOSINTREE ;
    // ========================================================================
    /** @typedef PSAME 
     *  The trivial predicate whoch check the identity of 2 particles
     * 
     *  @code 
     *
     *  const LHCb::Particle* B1 = ... ;
     *  Cut same = PSAME( B1 ) ;
     * 
     *  const LHCb::Particle* B2 = ... ;
     *  
     *  const bool theSame = same( B2 ) ;
     *
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *  @see LoKi::TheSame 
     *  @see LoKi::Cuts::SAME
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::TheSame<const LHCb::Particle*>                        PSAME ;
    // ========================================================================
    /** @typedef PSWITCH 
     *  Simple function, which acts as 
     *   result = condition ? function1 : function2 ;
     * 
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *
     *  Fun fun = PSWITCH( VALID , P , -1 * GeV ) ;
     *
     *  const double value = fun ( p ) ;
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *  @see LoKi::Switch 
     *  @see LoKi::Cuts::SWITCH
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23
     */
    typedef LoKi::Switch<const LHCb::Particle*>                       PSWITCH ;
    // ========================================================================
    /** @var PVALID
     *  the trivial functor which evaluayed the "validity" of the argument
     *  
     *  @see LoKi::Cuts::VALID
     *  @see LoKi::Valid
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Valid<const LHCb::Particle*>                           PVALID ;
    // ========================================================================
    /** @var PT 
     *  Particle's transverse momentum 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    pt       = PT( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::TransverseMomentum 
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::TransverseMomentum                            PT ;
    // ========================================================================
    /** @typedef PTDIR
     *  Simple evaluator of the transverse momentum of the particle 
     *  relative to some direction 
     *
     *  @code
     *
     *   const LHCb::Particle* jet = ... ;
     *  
     *   Fun ptdir = PTDIR( jet ) ;
     *  
     *   const LHCb::Particle* p = ... ;
     *
     *   double pt = prdir( p ) ;
     *
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Particles:TransverseMomentumRel 
     *  @see LoKi::Cuts::PTREL 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-01
     */
    typedef LoKi::Particles::TransverseMomentumRel                      PTDIR ;
    // ========================================================================
    /** @typedef PTREL
     *  Simple evaluator of the transverse momentum of the particle 
     *  relative to some direction 
     *
     *  @code
     *
     *   const LHCb::Particle* jet = ... ;
     *  
     *   Fun ptdir = PTREL( jet ) ;
     *  
     *   const LHCb::Particle* p = ... ;
     *
     *   double pt = prdir( p ) ;
     *
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Particles:TransverseMomentumRel 
     *  @see LoKi::Cuts::PTDIR
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-01
     */
    typedef LoKi::Particles::TransverseMomentumRel                      PTREL ;
    // ========================================================================
    /** @var PTRUE
     *  the trivial functor which always evaluates to "true"
     *  
     *  @see LoKi::BooleanConstant
     *  @see LHCb::Particle
     *  @see LoKi::Cuts::ALL
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,bool>         PTRUE ( true ) ;
    // ========================================================================
    /** @typedef PVTRACK
     *  The simple predicate which evaluates to 'true' for 
     *  the particle which *DIRECTLY* participates in to 
     *  the primary vertex reconstruction
     *
     *  @code 
     *  
     *  const LHCb::Particle*   p  = ... ;
     *  const LHCb::PrimVertex* pv = ... ;
     *  
     *  Cut fromPV = PVTRACK ( pv ) ;
     * 
     *  const bool OK = fromPV( p ) ;
     *
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LHCb::PrimVertex
     *  @see LoKi::Particles::HasTracks
     *
     *  @see LoKi::Particles::HasTracksFromPV
     *  @see LoKi::Cuts::TRACKFROMPV 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22
     */
    typedef LoKi::Particles::HasTracksFromPV                          PVTRACK ;
    // ========================================================================
    /** @typedef PVTRACKINTREE
     *  The simple predicate which evaluates to 'true' for 
     *  the particle which has in the decay tree at least 
     *  one particle which participates into 
     *  the primary vertex reconstruction
     *
     *  @code 
     *  
     *  const LHCb::Particle*   p  = ... ;
     *  const LHCb::PrimVertex* pv = ... ;
     *  
     *  Cut fromPV = PVTRACKINTREE ( pv ) ;
     * 
     *  const bool OK = fromPV( p ) ;
     *
     *  @endcode 
     *  
     *  
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LHCb::PrimVertex
     *  @see LoKi::Particles::HasTracksFromPV 
     *
     *  @see LoKi::Particles::HasTracksInTreeFromPV 
     *  @see LoKi::Cuts::TRACKFROMPVINTREE
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22
     */
    typedef LoKi::Particles::HasTracksInTreeFromPV              PVTRACKINTREE ;
    // ========================================================================
    /** @var PX 
     *  X-component of Particle's momentum 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    px       = PX( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::MomentumX
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::MomentumX                                     PX ;
    // ========================================================================
    /** @var PY 
     *  Y-component of Particle's momentum 
     *
     *  @code 
     * 
     *     const Particle* particle = ... ; //  get the particle
     *     const double    py       = PY( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::MomentumY
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::MomentumY                                     PY ;
    // ========================================================================
    /** @var PZ 
     *  Z-component of Particle's momentum 
     *
     *  @code 
     *
     *     const Particle* particle = ... ; //  get the particle
     *     const double    pz       = PZ( particle ) ;
     *
     *  @endcode
     *
     *  @see Particle
     *  @see LoKi::Particle::MomentumZ
     *  @see LoKi::Function
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-10
     */
    const   LoKi::Particles::MomentumZ                                     PZ ;
    // ========================================================================
    /** @var PZERO
     *  the trivial functor which always evaluates to 0
     *     
     *  @see LoKi::Cuts::ZERO
     *  @see LoKi::Constant
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,double>            PZERO ( 0 ) ;
    // ========================================================================
    /** @var Q 
     *  the trivial evaluator of particle charge 
     *  it relies on LHCb::Particle::charge 
     *
     *  @code
     *  
     *   const LHCb::Particle* p = ... ;
     *   int charge = (int) Q( p ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::Charge
     *  @see LHCb::Particle
     *  @see LHCb::Particle::charge
     *  
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2006-05-22
     */
    const LoKi::Particles::Charge                                           Q ;
    // ========================================================================
    /** @typedef SAME 
     *  The trivial predicate which check the identity of 2 particles
     * 
     *  @code 
     *
     *  const LHCb::Particle* B1 = ... ;
     *  Cut same = SAME( B1 ) ;
     * 
     *  const LHCb::Particle* B2 = ... ;
     *  
     *  const bool theSame = same( B2 ) ;
     *
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *  @see LoKi::TheSame 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    typedef LoKi::TheSame<const LHCb::Particle*>                         SAME ;
    // ========================================================================
    /** @typedef SSWITCH 
     *  Simple function, which acts as 
     *   result = condition ? constant1 : constant2 ;
     * 
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *
     *  Fun fun = SSWITCH( VALID , 1 , 0 ) ;
     *
     *  const double value = fun ( p ) ;
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *  @see LoKi::SimpleSwitch 
     *  @see LoKi::Cuts::PSSWITCH
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23
     */
    typedef LoKi::SimpleSwitch<const LHCb::Particle*>                 SSWITCH ;
    // ========================================================================
    /** @typedef STAT
     *  Simple monitoring counter for the functions
     *
     *  @code 
     *
     *  // some function to be monitored
     *  Fun fun = ... ;
     *
     *  // Create the monitored function using the generic counter
     *  // either local counter, or the counter from IStatSvc or ICounterSvc
     *  StatEntity* counter = ... ; ///< the generic counter
     *  Fun mon = STAT ( fun , counter ) ;
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
     *     << " Monitoring results : "                 << endreq 
     *     << " NEntries:  #" << counter->entries  ()  << endreq 
     *     << " TotalSum:   " << counter->flag     ()  << endreq 
     *     << " Mean+-RMS:  " << counter->flagMean () 
     *     << "+="            << counter->flagRMS  ()  << endreq      
     *     << " Min/Max:    " << counter->flagMin  ()  
     *     << "/"             << counter->flagMax  ()  << endreq ;
     *
     *  @endcode
     *
     *  The alternative (recommended) way to create the monitored function
     *  @code 
     *
     *  // some function to be monitored
     *  Fun fun = ... ;
     *
     *  Fun mon = monitor ( fun , counter("SomeCounter") ) ;
     *  
     *  @endcode 
     *
     *  The substitution of the function by monitored function 
     *  cound be done "on-flight" without the disturbing of the
     *  actual processing:
     *  @code
     *
     *  // some function to be monitored
     *  Fun fun = ... ;
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
    typedef LoKi::Monitoring::Counter<const LHCb::Particle*,double>      STAT ;
    // ========================================================================
    /** @var SUMQ
     *  the trivial evaluator of particle charge 
     *  it relies on LoKi::Particles::Charge for the 
     *  basic particles, and for recursive sum for 
     *  non-basic particles 
     *
     *  @code
     *  
     *   const LHCb::Particle* p = ... ;
     *   int charge = (int) SUMQ( p ) ;
     *
     *  @endcode 
     *     
     *  @see LoKi::Particles::SumCharge
     *  @see LoKi::Particles::Charge
     *  
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2006-05-22
     */
    const LoKi::Particles::SumCharge                                     SUMQ ;
    // ========================================================================
    /** @typedef SUMTREE 
     *  The simple function which accumulated the 
     *  value of certain function over the decay 
     *  tree of the particle for the articles which 
     *  satisfy the certain selection criteria
     *  Accumulation is done throught summation
     *
     *  The function uses the algorithm LoKi::PhysAlgs::accumulate
     *
     *  @code 
     * 
     *  const LHCb::Particle* B = ... ;
     *  
     *  // get the sum of the transverse moemntum of all pions in the tree
     *  Fun fun = SUMTREE( PT , "pi+" == ABSID , 0.0 ) ;
     *  
     *  const double sumPT = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::PhysAlgs::accumulate
     *  @see LoKi::Cuts::TREESUM 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::SumTree                                  SUMTREE ;
    // ========================================================================
    /** @typedef SWITCH 
     *  Simple function, which acts as 
     *   result = condition ? function1 : function2 ;
     * 
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *
     *  Fun fun = SWITCH( VALID , P , -1 * GeV ) ;
     *
     *  const double value = fun ( p ) ;
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *  @see LoKi::Switch 
     *  @see LoKi::Cuts::PSWITCH
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23
     */
    typedef LoKi::Switch<const LHCb::Particle*>                        SWITCH ;
    // ========================================================================
    /** @var VALID
     *  the trivial functor which evaluayed the "validity" of the argument
     *  
     *  @see LoKi::Valid
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Valid<const LHCb::Particle*>                            VALID ;
    // ========================================================================
    /** @typedef TD
     *  Simple evaluator of the proper lifetime 
     *  distance (c*tau) of the particle between 
     *  the "endVertex" and the other vertex
     *
     *  LoKi::Particles::VertexDistance is used to estimate the 
     *  3D-distance 
     *  
     *  @code 
     * 
     *  const LHCb::Particle* B   = ... ;
     *  const LHCb::Vertex*   pv  = ... ;
     *
     *  Fun cTau = TD( pv ) ;
     * 
     *  const double ctau = cTau( B ) ;
     *
     *  @endcode 
     *  
     *  @see LoKi::Particles::TimeDistance 
     *  @see LoKi::Particles::VertexDistance 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::TIMEDIST
     *  @see LoKi::Cuts::TDIST
     *  @see LoKi::Cuts::CTAU
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::TimeDistance                                  TD ;
    // ========================================================================
    /** @typedef TDIST 
     *  Simple evaluator of the proper lifetime 
     *  distance (c*tau) of the particle between 
     *  the "endVertex" and the other vertex
     *
     *  LoKi::Particles::VertexDistance is used to estimate the 
     *  3D-distance 
     *  
     *  @code 
     * 
     *  const LHCb::Particle* B   = ... ;
     *  const LHCb::Vertex*   pv  = ... ;
     *
     *  Fun cTau = TDIST( pv ) ;
     * 
     *  const double ctau = cTau( B ) ;
     *
     *  @endcode 
     *  
     *  @see LoKi::Particles::TimeDistance 
     *  @see LoKi::Particles::VertexDistance 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::TIMEDIST
     *  @see LoKi::Cuts::CTAU
     *  @see LoKi::Cuts::TD
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::TimeDistance                               TDIST ;
    // ========================================================================
    /** @typedef TDOT 
     *  Simple evaluator of the proper lifetime 
     *  distance (c*tau) of the particle between 
     *  the "endVertex" and the other vertex along 
     *  the partile momentum
     *
     *  LoKi::Particles::VertexDotDistance is used to estimate the 
     *  3D-distance 
     *  
     *  @code 
     * 
     *  const LHCb::Particle* B   = ... ;
     *  const LHCb::Vertex*   pv  = ... ;
     *
     *  Fun cTau = TDOT( pv ) ;
     * 
     *  const double ctau = cTau( B ) ;
     *
     *  @endcode 
     *  
     *  @see LoKi::Particles::TimeDotDistance 
     *  @see LoKi::Particles::VertexDistance 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::CTAUDOT
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::TimeDotDistance                             TDOT ;
    // ========================================================================
    /** @typedef TDSIGN 
     *  Simple evaluator of the proper lifetime 
     *  distance (c*tau) of the particle between 
     *  the "endVertex" and the other vertex
     *
     *  LoKi::Particles::VertexDistance is used to estimate the 
     *  3D-distance 
     *  
     *  @code 
     * 
     *  const LHCb::Particle* B   = ... ;
     *  const LHCb::Vertex*   pv  = ... ;
     *
     *  Fun cTau = TDSIGN( pv ) ;
     * 
     *  const double ctau = cTau( B ) ;
     *
     *  @endcode 
     *  
     *  @see LoKi::Particles::TimeSignedDistance 
     *  @see LoKi::Particles::VertexDistance 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::TSIGND 
     *  @see LoKi::Cuts::CTAUSIGN
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::TimeSignedDistance                        TDSIGN ;
    // ========================================================================
    /** @typedef TIMEDIST 
     *  Simple evaluator of the proper lifetime 
     *  distance (c*tau) of the particle between 
     *  the "endVertex" and the other vertex
     *
     *  LoKi::Particles::VertexDistance is used to estimate the 
     *  3D-distance 
     *  
     *  @code 
     * 
     *  const LHCb::Particle* B   = ... ;
     *  const LHCb::Vertex*   pv  = ... ;
     *
     *  Fun cTau = TIMEDIST( pv ) ;
     * 
     *  const double ctau = cTau( B ) ;
     *
     *  @endcode 
     *  
     *  @see LoKi::Particles::TimeDistance 
     *  @see LoKi::Particles::VertexDistance 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::CTAU
     *  @see LoKi::Cuts::TDIST
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::TimeDistance                            TIMEDIST ;
    // ========================================================================
    /** @typedef TRACKFROMPV 
     *  The simple predicate which evaluates to 'true' for 
     *  the particle which *DIRECTLY* participates in to 
     *  the primary vertex reconstruction
     *
     *  @code 
     *  
     *  const LHCb::Particle*   p  = ... ;
     *  const LHCb::PrimVertex* pv = ... ;
     *  
     *  Cut fromPV = TRACKFROMPV ( pv ) ;
     * 
     *  const bool OK = fromPV( p ) ;
     *
     *  @endcode 
     *  
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LHCb::PrimVertex
     *  @see LoKi::Particles::HasTracks
     *
     *  @see LoKi::Particles::HasTracksFromPV
     *  @see LoKi::Cuts::PVTRACK
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22
     */
    typedef LoKi::Particles::HasTracksFromPV                      TRACKFROMPV ;
    // ========================================================================
    /** @typedef TRACKFROMPVINTREE
     *  The simple predicate which evaluates to 'true' for 
     *  the particle which has in the decay tree at least 
     *  one particle which participates into 
     *  the primary vertex reconstruction
     *
     *  @code 
     *  
     *  const LHCb::Particle*   p  = ... ;
     *  const LHCb::PrimVertex* pv = ... ;
     *  
     *  Cut fromPV = TRACKFROMPVINTREE ( pv ) ;
     * 
     *  const bool OK = fromPV( p ) ;
     *
     *  @endcode 
     *  
     *  
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LHCb::PrimVertex
     *  @see LoKi::Particles::HasTracksFromPV 
     *
     *  @see LoKi::Particles::HasTracksInTreeFromPV 
     *  @see LoKi::Cuts::PVTRACKINTREE
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22
     */
    typedef LoKi::Particles::HasTracksInTreeFromPV          TRACKFROMPVINTREE ;
    // ========================================================================
    /** @typedef TRACKS 
     *  The simple predicate which checks if the particle 
     *  has a track from the specified list of tracks 
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE  tracks = ... ;
     *
     *  Cut hasTracks = TRACKS ( tracks.begin() , tracks.end() ) ;
     *  
     *  const bool good = hasTrack ( p ) ;
     * 
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Extract::Particle2Track
     *  @see LHCb::Track
     * 
     *  @see LoKi::Particles::HasTracks
     *  @see LoKi::Cuts::HASTRACKS
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::HasTracks                                 TRACKS ;
    // ========================================================================
    /** @typedef TRACKSINTREE
     *  The simple predicate which checks if the particle 
     *  has a track from the specified list of tracks 
     *
     *  @code 
     * 
     *  const LHCb::Particle* p = ... ;
     *  SEQUENCE  tracks = ... ;
     *
     *  Cut hasTracks = TRACKSINTREE ( tracks.begin() , tracks.end() ) ;
     *  
     *  const bool good = hasTrack ( p ) ;
     * 
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Extract::Particle2Track
     *  @see LHCb::Track
     * 
     *  @see LoKi::Particles::HasTracksInTree
     *  @see LoKi::Cuts::HASTRACKSINTREE
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-22 
     */
    typedef LoKi::Particles::HasTracksInTree                     TRACKSINTREE ;
    // ========================================================================
    /** @var TRCHI2 
     *  The trivial funtion which evaluates LHCb::Track::chi2
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  const double chi2  = TRCHI2 ( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see LHCb::Track
     *  @see LHCb::Track::chi2 
     *  @see LoKi::Particles::TrackChi2 
     *  @see LoKi::CHI2TR
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const LoKi::Particles::TrackChi2                                   TRCHI2 ;
    // ========================================================================
    /** @var TRCHI2DOF 
     *  The trivial funtion which evaluates LHCb::Track::chi2PerDoF
     *
     *  @code 
     *  
     *  const LHCb::Particle* p = ... ;
     *  
     *  const double chi2  = TRCHI2DOF ( p ) ;
     *
     *  @endcode 
     * 
     *  @see LHCb::Particle
     *  @see LHCb::Track
     *  @see LHCb::Track::chi2 
     *  @see LoKi::Particles::TrackChi2PerDof
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const LoKi::Particles::TrackChi2PerDoF                          TRCHI2DOF ;
    // ========================================================================
    /** @typedef TREEMAX 
     *  The trivial algorithm which scans the decay 
     *  tree of the particle and searches for the the 
     *  minimal value for some functions for
     *  particles which satisfy the certain criteria 
     *
     *  The function uses the algorithm LoKi::PhysAlgs::max_value 
     *
     *  @code 
     *
     *  const LHCb::Particle* B = ... ;
     *  
     *  // the maximal value of chi2 of vertex fit  in the decay tree
     *  Fun fun = TREEMAX ( VFUNASPFUN( VCHI2 ) , HASVERTEX ) ;
     * 
     *  const double maxChi2 = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::PhysAlgs::max_value  
     *  @see LoKi::Cuts::MAXTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::MaxTree                                  TREEMAX ;
    // ========================================================================
    /** @typedef TREEMIN 
     *  The trivial algorithm which scans the decay 
     *  tree of the particle and searches for the the 
     *  minimal value for some functions for
     *  particles which satisfy the certain criteria 
     *
     *  The function uses the algorithm LoKi::PhysAlgs::min_value 
     *
     *  @code 
     * 
     *  const LHCb::Particle* B = ... ;
     * 
     *  // minimal transverse momentum of the track in the decay tree:
     *  Fun fun = TREEMIN ( HASTRACK , PT ) ;
     * 
     *  const double minPT = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::PhysAlgs::min_value  
     *  @see LoKi::Cuts::MINTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::MinTree                                  TREEMIN ;
    // ========================================================================
    /** @typedef TREEMULT 
     *  The simple function which accumulated the 
     *  value of certain function over the decay 
     *  tree of the particle for the articles which 
     *  satisfy the certain selection criteria
     *  Accumulation is done throught summation
     *
     *  The function uses the algorithm LoKi::PhysAlgs::accumulate
     *
     *  @code 
     * 
     *  const LHCb::Particle* B = ... ;
     *  
     *  // get the product of "weight" 
     *  // for all non-basic particles in the tree 
     *  Fun fun = MULTTREE( W , !BASIC , 1.0 ) ;
     *  
     *  const double weight = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::PhysAlgs::accumulate
     *  @see LoKi::Cuts::MULTTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::MultTree                                TREEMULT ;
    // ========================================================================
    /** @typedef TREESUM 
     *  The simple function which accumulated the 
     *  value of certain function over the decay 
     *  tree of the particle for the articles which 
     *  satisfy the certain selection criteria
     *  Accumulation is done throught summation
     *
     *  The function uses the algorithm LoKi::PhysAlgs::accumulate
     *
     *  @code 
     * 
     *  const LHCb::Particle* B = ... ;
     *  
     *  // get the sum of the transverse moemntum of all pions in the tree
     *  Fun fun = SUMTREE( PT , "pi+" == ABSID , 0.0 ) ;
     *  
     *  const double sumPT = fun( B ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::PhysAlgs::accumulate
     *  @see LoKi::Cuts::SUMTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    typedef LoKi::Particles::SumTree                                  TREESUM ;
    // ========================================================================
    /** @var TRDOWN
     *  The trivial predicate whoch evalauted to true for particles, 
     *  made from "Dowstream" tracks 
     * 
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ; 
     *  
     *  const bool isDown = TRDOWN( p ) ;
     *  
     *  @endcode 
     *   
     *  @see LoKi:Particle
     *  @see LoKi:Track
     *  @see LoKi:Track::Types 
     *  @see LoKi:Track::Downstream
     *  @see LoKi::Cuts::EQUALTO
     *  @see LoKi::Cuts::TRTYPE  
     *  @see LoKi::Cuts::ISDOWN  
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const EQUALTO                 TRDOWN ( TRTYPE , LHCb::Track::Downstream ) ;
    // ========================================================================
    /** @var TRHISTORY 
     *  The trivial function which evaluates LHCb::Track::history
     *
     *  @see LHCb::Particle
     *  @see LHCb::Track
     *  @see LHCb::Track::History
     *  @see LHCb::Track::history
     *  @see LoKi::Particles::TrackHistory
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */    
    const LoKi::Particles::TrackHistory                             TRHISTORY ;
    // ========================================================================
    /** @var TRHISTFIT 
     *  The trivial function which evaluates LHCb::Track::historyFit
     *
     *  @see LHCb::Particle
     *  @see LHCb::Track
     *  @see LHCb::Track::HistoryFit
     *  @see LHCb::Track::historyFit
     *  @see LoKi::Particles::TrackHistoryFit 
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const LoKi::Particles::TrackHistoryFit                          TRHISTFIT ;
    // ========================================================================
    /** @var TRLONG 
     *  The trivial predicate whoch evalauted to true for particles, 
     *  made from "Long" tracks 
     * 
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ; 
     *  
     *  const bool isLong = TRLONG( p ) ;
     *  
     *  @endcode 
     *   
     *  @see LoKi:Particle
     *  @see LoKi:Track
     *  @see LoKi:Track::Types 
     *  @see LoKi:Track::Long
     *  @see LoKi::Cuts::EQUALTO
     *  @see LoKi::Cuts::TRTYPE  
     *  @see LoKi::Cuts::ISLONG
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const EQUALTO                       TRLONG ( TRTYPE , LHCb::Track::Long ) ;
    // ========================================================================
    /** @var TRPCHI2 
     *  The trivial evaluato of \f$\chi^2\f$-probability for the track 
     *
     *  @code
     * 
     *  const LHCb::Particle* pion = ... ;
     *  
     *  const double prob = TRPCHI2 ( pion ) ;
     *
     *  @endcode
     *  @see LoKi::Particles::TrackChi2Prob
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-04-10
     */
    const LoKi::Particles::TrackChi2Prob                              TRPCHI2 ;
    // ========================================================================
    /** @var TRSTATUS 
     *  The trivial function which evaluates LHCb::Track::status
     *
     *  @see LHCb::Particle
     *  @see LHCb::Track
     *  @see LHCb::Track::Status
     *  @see LHCb::Track::status
     *  @see LoKi::Particles::TrackStatus
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const LoKi::Particles::TrackStatus                               TRSTATUS ;
    // ========================================================================
//     /** @var TRTYPE 
//      *  The trivial function which evaluates LHCb::Track::type
//      *
//      *  @code 
//      *  
//      *  const LHCb::Particle* p = ... ;
//      *
//      *  const bool Long = LHCb::Track::Long == TRTYPE( p ) ;
//      *
//      *  @endcode 
//      *
//      *  @see LHCb::Particle
//      *  @see LHCb::Track
//      *  @see LHCb::Track::Type
//      *  @see LHCb::Track::type 
//      *  @see LoKi::Cuts::TRTYPE
//      *
//      *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
//      *  @date 2006-02-23 
//      */
//     const LoKi::Particles::TrackType                                   TRTYPE ;
//     // ========================================================================
    /** @var TRTTRACK
     *  The trivial predicate whoch evalauted to true for particles, 
     *  made from "Ttrack" tracks 
     * 
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ; 
     *  
     *  const bool isT = TRTTRACK( p ) ;
     *  
     *  @endcode 
     *   
     *  @see LoKi:Particle
     *  @see LoKi:Track
     *  @see LoKi:Track::Types 
     *  @see LoKi:Track::Ttrack
     *  @see LoKi::Cuts::EQUALTO
     *  @see LoKi::Cuts::TRTYPE  
     *  @see LoKi::Cuts::ISTTRACK  
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const EQUALTO                   TRTTRACK ( TRTYPE , LHCb::Track::Ttrack ) ;
    // ========================================================================
    /** @var TRUP
     *  The trivial predicate which evaluates to true for particles, 
     *  made from "Upstream" tracks 
     * 
     *  @code 
     *   
     *  const LHCb::Particle* p = ... ; 
     *  
     *  const bool trUp = TRUP ( p ) ;
     *  
     *  @endcode 
     *   
     *  @see LoKi:Particle
     *  @see LoKi:Track
     *  @see LoKi:Track::Types 
     *  @see LoKi:Track::Upstream
     *  @see LoKi::Cuts::EQUALTO
     *  @see LoKi::Cuts::TRTYPE  
     *  @see LoKi::Cuts::ISUP  
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-23 
     */
    const EQUALTO                      TRUP( TRTYPE , LHCb::Track::Upstream ) ;
    // ========================================================================
    /** @typedef TSIGND 
     *  Simple evaluator of the proper lifetime 
     *  distance (c*tau) of the particle between 
     *  the "endVertex" and the other vertex
     *
     *  LoKi::Particles::VertexDistance is used to estimate the 
     *  3D-distance 
     *  
     *  @code 
     * 
     *  const LHCb::Particle* B   = ... ;
     *  const LHCb::Vertex*   pv  = ... ;
     *
     *  Fun cTau = TSIGND( pv ) ;
     * 
     *  const double ctau = cTau( B ) ;
     *
     *  @endcode 
     *  
     *  @see LoKi::Particles::TimeSignedDistance 
     *  @see LoKi::Particles::VertexDistance 
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *  @see LoKi::Cuts::CTAUSIGN
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::TimeSignedDistance                        TSIGND ;
    // ========================================================================
    /** @typedef VD 
     *  Evaluator of the GEOMETRY 
     *  distance between the particle 
     *  "endVertex" and "the vertex". 
     *
     *  @code 
     *
     *  const LHCb::Particle* p = ... ;
     * 
     *  const LHCb::PrimVertex* pv = ... ;
     * 
     *  Fun dist = VD( pv ) ;
     * 
     *  const double flight = dist( p ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Particles::VertexDistance  
     *  @see LHCb::Vertex
     *  @see LHCb::Particle
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::VertexDistance                                VD ;
    // ========================================================================
    /** @typedef VDCHI2 
     *  Evaluator of the chi2 of GEOMETRY distance between the particle 
     *  "endVertex" and "the vertex" 
     *
     *  @code 
     *
     *  const LHCb::Particle* p = ... ;
     * 
     *  const LHCb::PrimVertex* pv = ... ;
     * 
     *  Fun dist = VDCHI2( pv ) ;
     * 
     *  const double chi2 = dist( p ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Particles::VertexChi2Distance 
     *  @see LHCb::Vertex
     *  @see LHCb::Particle
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::VertexChi2Distance                        VDCHI2 ;
    // ========================================================================
    /** @typedef VDDOT 
     *  Evaluator of the distance between the particle 
     *  "endVertex" and "the vertex" along the particle momentum
     *
     *  @code 
     *
     *  const LHCb::Particle* p = ... ;
     * 
     *  const LHCb::PrimVertex* pv = ... ;
     * 
     *  Fun dist = VDDOT( pv ) ;
     * 
     *  const double flight = dist( p ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Particles::VertexDotDistance 
     *  @see LHCb::Vertex
     *  @see LHCb::Particle
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::VertexDotDistance                          VDDOT ;
    // ========================================================================
    /** @typedef VDMIN 
     *  Evaluator of the minimal GEOMETRY distance between the particle 
     *  "endVertex" and some set of vertices  
     *
     *  @code 
     *
     *  const LHCb::Particle* p = ... ;
     * 
     *  const LHCb::PrimVertex* pv = ... ;
     * 
     *  Fun dist = VDMIN( pv ) ;
     * 
     *  const double minDistance = dist( p ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Particles::MinVertexDistance 
     *  @see LHCb::Vertex
     *  @see LHCb::Particle
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinVertexDistance                          VDMIN ;
    // ========================================================================
    /** @typedef VDMINCHI2 
     *  Evaluator of the minimal chi2 for distance between the particle 
     *  "endVertex" and some set of vertices  
     *
     *  @code 
     *
     *  const LHCb::Particle* p = ... ;
     * 
     *  const LHCb::PrimVertices* pv = ... ;
     * 
     *  Fun dist = VDMINCHI2( pv ) ;
     * 
     *  const double chi2  = dist( p ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Particles::MinVertexChi2Distance 
     *  @see LHCb::Vertex
     *  @see LHCb::Particle
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::MinVertexChi2Distance                  VDMINCHI2 ;
    // ========================================================================
    /** @typedef VDSIGN 
     *  Evaluator of the GEOMETRY distance between the particle 
     *  "endVertex" and "the vertex". The distance is signed according 
     *  to the sign of (Zv-Z0)
     *
     *  @code 
     *
     *  const LHCb::Particle* p = ... ;
     * 
     *  const LHCb::PrimVertex* pv = ... ;
     * 
     *  Fun dist = VDSIGN( pv ) ;
     * 
     *  const double flight = dist( p ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Particles::VertexSignedDistance 
     *  @see LHCb::Vertex
     *  @see LHCb::Particle
     * 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    typedef LoKi::Particles::VertexSignedDistance                      VDSIGN ;
    // ========================================================================
    /** @typedef VFASPF 
     *  The simple adapter of "Vertex function" as "Particle function"
     *
     *  e.g. return z-position of particle endVertex:
     *
     *  @code 
     *
     *  const LHCb::Particle* particle = ... ;
     *  
     *  // create the function
     *  Fun vz = VFASPF ( VZ ) ;
     *
     *  // use it 
     *  double z  = vz ( particle ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::VFunAsPFun
     *  @see LoKi::Cuts::VFUNASPFUN
     *  @see LoKi::Cuts::VXFUN
     *  @see LoKi::Cuts::VFUN
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-02-27
     */  
    typedef LoKi::Particles::VFunAsPFun                                VFASPF ;
    // ========================================================================
    /** @typedef VFUN 
     *  The simple adapter of "Vertex function"
     *  as "Particle function"
     *
     *  e.g. return z-position of particle endVertex:
     *
     *  @code 
     *
     *  const LHCb::Particle* particle = ... ;
     *  
     *  // create the function
     *  Fun vz = VFUN( VZ ) ;
     *
     *  // use it 
     *  double z  = vz ( particle ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::VFunAsPFun
     *  @see LoKi::Cuts::VFASPF
     *  @see LoKi::Cuts::VXFUN
     *  @see LoKi::Cuts::VFUNASPFUN
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-02-27
     */  
    typedef LoKi::Particles::VFunAsPFun                                  VFUN ;
    // ========================================================================
    /** @typedef VFUNASPFUN 
     *  The simple adapter of "Vertex function"
     *  as "Particle function"
     *
     *  e.g. return z-position of particle endVertex:
     *
     *  @code 
     *
     *  const LHCb::Particle* particle = ... ;
     *  
     *  // create the function
     *  Fun vz = VFUNASPFUN ( VZ ) ;
     *
     *  // use it 
     *  double z  = vz ( particle ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::VFunAsPFun
     *  @see LoKi::Cuts::VFASPF
     *  @see LoKi::Cuts::VXFUN
     *  @see LoKi::Cuts::VFUN
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-02-27
     */  
    typedef LoKi::Particles::VFunAsPFun                            VFUNASPFUN ;
    // ========================================================================
    /** @typedef VXFUN 
     *  The simple adapter of "Vertex function"
     *  as "Particle function"
     *
     *  e.g. return z-position of particle endVertex:
     *
     *  @code 
     *
     *  const LHCb::Particle* particle = ... ;
     *  
     *  // create the function
     *  Fun vz = VXFUN ( VZ ) ;
     *
     *  // use it 
     *  double z  = vz ( particle ) ;
     *
     *  @endcode 
     *
     *  @see LoKi::Particles::VFunAsPFun
     *  @see LoKi::Cuts::VFASPF
     *  @see LoKi::Cuts::VFUN
     *  @see LoKi::Cuts::VFUNASPFUN
     *  @see LHCb::Particle
     *  @see LHCb::Vertex
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-02-27
     */  
    typedef LoKi::Particles::VFunAsPFun                                 VXFUN ;
    // ========================================================================
    /** @var W
     *  Trivial function which evaluates "weight" for 
     *  the particle
     *
     *  It relies on the method LHCb::Particle::weight
     *
     *  @code 
     * 
     *   const LHCb::Particle* p = ... ;
     *   const double weight = W( p ) ;
     *  
     *  @endcode 
     *
     *  @see LoKi::Cuts::WEIGHT
     *  @see LHCb::Particle
     *  @see LoKi::Particles::Weight
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const   LoKi::Particles::Weight                                         W ;
    // ========================================================================
    /** @var WEIGHT
     *  Trivial function which evaluates "weight" for 
     *  the particle
     *
     *  It relies on the method LHCb::Particle::weight
     *
     *  @code 
     * 
     *   const LHCb::Particle* p = ... ;
     *   const double weight = WEIGHT( p ) ;
     *  
     *  @endcode 
     *
     *  @see LHCb::Particle
     *  @see LoKi::Particles::Weight
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const   LoKi::Particles::Weight                                    WEIGHT ;
    // ========================================================================
    /** @var ZERO
     *  the trivial functor which always evaluates to 0
     *  
     *  @see LoKi::Constant
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-15
     */
    const LoKi::Constant<const LHCb::Particle*,double>             ZERO ( 0 ) ;
    // ========================================================================
  }  // end of namespace LoKi::Cuts  
}  // end of namespace LoKi
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_PHYSPARTICLECUTS_H
// ============================================================================

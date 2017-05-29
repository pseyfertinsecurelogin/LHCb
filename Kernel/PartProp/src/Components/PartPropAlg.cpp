// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include <iterator>
#include <ostream>
#include <string>

#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/IService.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/SystemOfUnits.h"
// ============================================================================
// Kenrel/PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleID.h"
#include "Kernel/ParticleProperty.h"

class ISvcLocator;

// ============================================================================
namespace LHCb
{
  // ==========================================================================
  namespace Example
  {
    // ========================================================================
    /** @class PartPropAlg
     *  Simple algorithm to illustrate the major properties of new Particle
     *  Property Service
     *  @see LHCb::ParticlePropertySvc
     *  @see LHCb::IParticlePropertySvc
     *  @see LHCb::ParticleProperty
     *  @see LHCb::ParticleProperties
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-12-01
     */
    class PartPropAlg : public Algorithm
    {
      // ======================================================================
      /// friend factory for algorithm instantiation
      friend class AlgFactory<LHCb::Example::PartPropAlg> ;  //     the factory
      // ======================================================================
    public :
      // ======================================================================
      /// standard algorithm initialization
      StatusCode  initialize () override
      {
        StatusCode sc = Algorithm::initialize () ;   //     initialize the base
        if ( sc.isFailure() ) { return sc ; }
        // locate particle property service
        ppSvc() ;
        return StatusCode::SUCCESS ;
      }
      /// standard algorithm finalization
      StatusCode  finalize   () override
      {
        // release the aquired service
        m_ppSvc.reset();
        return Algorithm::finalize () ;            //        finalize the base
      }
      /// algorithm execution
      StatusCode  execute    () override;
      // ======================================================================
    protected:
      // ======================================================================
      /** standard constructor
       *  @param name algorithm instance name
       *  @param pSvc pointer to the service locator
       */
      PartPropAlg
      ( const std::string& name ,      //               algorithm instance name
        ISvcLocator*       pSvc )      //        pointer to the service locator
        : Algorithm ( name , pSvc )
      {}
      // ======================================================================
      /// locate the new particle property service
      const LHCb::IParticlePropertySvc* ppSvc() const ;
      // ======================================================================
      /// the copy constructor is disabled
      PartPropAlg ( const PartPropAlg& ) = delete;// copy constructor is disabled
      /// assignement operator is disbaled
      PartPropAlg& operator=( const PartPropAlg& ) = delete; // no assignement
      // ======================================================================
    private:
      // ======================================================================
      /// the pointer to new particle property service
      mutable SmartIF<LHCb::IParticlePropertySvc> m_ppSvc ;    // new part.property.svc
      // ======================================================================
    } ;
    // ========================================================================
  } // end of namespace LHCb::Example
  // ==========================================================================
} // end of namespace LHCb
// ============================================================================
// locate the new particle property service
// ============================================================================
const LHCb::IParticlePropertySvc* LHCb::Example::PartPropAlg::ppSvc() const
{
  if ( UNLIKELY(!m_ppSvc) )
  {
    m_ppSvc = service ( "LHCb::ParticlePropertySvc", true );
    if ( !m_ppSvc )
    {
      throw GaudiException
        ("Service [LHCb::ParticlePropertySvc] not found",
         name(), StatusCode::FAILURE ) ;
    }
  }
  // ==========================================================================
  return m_ppSvc.get() ;
}
// ============================================================================
// execute the algorithm
// ============================================================================
StatusCode LHCb::Example::PartPropAlg::execute()
{
  // get the service
  const LHCb::IParticlePropertySvc* svc = ppSvc () ;

  if ( !svc ) { return StatusCode::FAILURE ; }

  // avoid long names
  typedef LHCb::IParticlePropertySvc::ParticleProperties PPs ;

  MsgStream log ( msgSvc () , name() ) ;

  // ==========================================================================
  { // get all invalid
    PPs invalid ;
    svc->get
      (
       // functor : invalid
       [](const LHCb::ParticleProperty* pp) { return !pp->pid().isValid(); } ,
       // output
       std::back_inserter( invalid ) ) ;
    // print as the table
    // header ?
    log << MSG::INFO << " # Invalid = " << invalid.size() << std::endl ;
    // content
    LHCb::ParticleProperties::printAsTable ( invalid , log , svc ) ;
    log << endmsg ;
  }
  // ==========================================================================
  { // get all not from quarks
    PPs noquarks ;
    svc->get
      (
       // functor : has no quarks
       [](const LHCb::ParticleProperty* pp) { return !pp->pid().hasQuarks(); },
       // output
       std::back_inserter( noquarks ) ) ;
    // print as the table
    // header ?
    log << MSG::INFO << " # Has no quarks = " << noquarks.size() << std::endl ;
    // content
    LHCb::ParticleProperties::printAsTable ( noquarks , log , svc ) ;
    log << endmsg ;
  }
  // ==========================================================================
  { // get all 'fundamental'
    PPs fundamental  ;
    svc->get
      (
       // functor : fundamental
       [](const LHCb::ParticleProperty* pp) {
           auto fid = pp->pid().fundamentalID();
           return 0 < fid && 100 >= fid; },
       // output
       std::back_inserter( fundamental ) ) ;
    // print as the table
    // header ?
    log << MSG::INFO << " # Fundamental (0,100] = " << fundamental.size() << std::endl ;
    // content
    LHCb::ParticleProperties::printAsTable ( fundamental , log , svc ) ;
    log << endmsg ;
  }
  // ==========================================================================
  { // get all leptons
    PPs leptons ;
    svc->get
      (
       // functor : lepton
       [](const LHCb::ParticleProperty* pp) { return pp->pid().isLepton(); },
       // output
       std::back_inserter( leptons ) ) ;
    // print as the table
    // header ?
    log << MSG::INFO << " # Leptons = " << leptons.size() << std::endl ;
    // content
    LHCb::ParticleProperties::printAsTable ( leptons , log , svc ) ;
    log << endmsg ;
  }
  // ==========================================================================
  { // get all long-lived (>1um)
    PPs longlived ;
    svc -> get
      (
       // functor : ctau>1um
       [](const LHCb::ParticleProperty* pp)
       { return pp->ctau() > 1 * Gaudi::Units::micrometer ; } ,
       // output
       std::back_inserter( longlived ) ) ;
    // print as the table
    // header ?
    log << MSG::INFO << " # Long-lived(>1mu) = " << longlived.size() << std::endl ;
    // content
    LHCb::ParticleProperties::printAsTable ( longlived , log , svc ) ;
    log << endmsg ;
  }
  // =========================================================================
  { // get all nuclea
    PPs nuclea ;
    svc->get
      (
       // functor : nucleus
       [](const LHCb::ParticleProperty* pp) { return pp->pid().isNucleus(); },
       // output
       std::back_inserter( nuclea ) ) ;
    // print as the table
    // header ?
    log << MSG::INFO << " # Nuclea  = " << nuclea.size() << std::endl ;
    // content
    LHCb::ParticleProperties::printAsTable ( nuclea , log , svc ) ;
    log << endmsg ;
  }
  // ==========================================================================
  { // get all beauty baryons
    PPs  bbaryons;
    svc->get
      (
       // functor : beauty & baryon
       [](const LHCb::ParticleProperty* pp) {
           return pp->pid().hasBottom() && pp->pid().isBaryon() ; },
       // output
       std::back_inserter( bbaryons ) ) ;
    // print as the table
    // header ?
    log << MSG::INFO
        << " # Beauty Baryons  = " << bbaryons.size()
        << std::endl ;
    // content
    LHCb::ParticleProperties::printAsTable ( bbaryons , log , svc ) ;
    log << endmsg ;
  }
  // ==========================================================================

  return StatusCode::SUCCESS ;
}
// ============================================================================
/// Declaration of the Algorithm Factory
DECLARE_NAMESPACE_ALGORITHM_FACTORY(LHCb::Example,PartPropAlg)
// ============================================================================
// The END
// ============================================================================

// ============================================================================
// Include files
// ============================================================================
// GaudiAlg
// ============================================================================
#include <functional>
#include <string>
#include <vector>

#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiCommonImp.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleID.h"
#include "Kernel/ParticleProperty.h"
// LoKiCore
// ============================================================================
#include "LoKi/ILoKiSvc.h"
#include "LoKi/ParticleProperties.h"

class ISvcLocator;

// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class PIDTest
   *  Simple algorithm to veryfy the fast access to PID information
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-065-31
   */
  class PIDTest : public GaudiAlgorithm
  {
    // ========================================================================
    /// the friend factory for instantiation
    friend class AlgFactory<LoKi::PIDTest> ;
    // ========================================================================
  public:
    // ========================================================================
    /// standard initialzation
    StatusCode initialize ()  override
    {
      StatusCode sc = GaudiAlgorithm::initialize() ;
      if ( sc.isFailure() ) { return sc ; }                     // RETURN
      svc<LoKi::ILoKiSvc>("LoKiSvc",true) ;
      m_svc = svc<LHCb::IParticlePropertySvc>("LHCb::ParticlePropertySvc",true) ;
      Assert ( 0 != m_svc , "Invalid pointer to  Particle Properety Service!" ) ;
      return StatusCode::SUCCESS ;                              // RETURN
    }
    // ========================================================================
    StatusCode execute ()  override
    {
      // avoid long names
      using namespace LoKi::Particles ;
      // check
      Assert ( 0 != m_svc , "Invalid pointer to  Particle Properety Service!" ) ;
      // loop over properties:
      for ( LHCb::IParticlePropertySvc::iterator ipp = m_svc->begin() ;
            m_svc->end() != ipp ; ++ipp )
      {
        const LHCb::ParticleProperty* pp = *ipp ;
        if ( 0 == pp ) { continue ; }                            // RETURN
        debug() << "Name/PID: "
                << "'" << pp->particle() << "':" << pp->pdgID() << endmsg ;
        // verify name -> pid map
        Assert ( pp->particle   () == nameFromPID ( pp->particleID() ) ,
                 "Mismatch in Name/PID map for '" + pp->particle() + "'" ) ;
        // verify pid  -> name map
        Assert ( pp->particleID () == pidFromName ( pp->particle ()  ) ,
                 "Mismatch in PID/Name map for '" + pp->particle() + "'" ) ;
      }
      return StatusCode::SUCCESS ;
    }
    // ========================================================================
  protected:
    // ========================================================================
    /// standard constructor
    PIDTest ( const std::string& name , ISvcLocator*       pSvc )
      : GaudiAlgorithm ( name , pSvc )
    {}
    // ========================================================================
    /// copy constructror is disabled
    PIDTest( const PIDTest& ) = delete;                      //     no copy constructor
    /// assignement is disabled
    PIDTest& operator=( const PIDTest& ) = delete;           // no assignement operator
    // ========================================================================
  private:
    // ========================================================================
    /// The pointer to particle property sevice
    LHCb::IParticlePropertySvc* m_svc = nullptr ; // pointer to particle property sevice
    // ========================================================================
  } ;
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// Declaration of the Algorithm Factory
// ============================================================================
DECLARE_NAMESPACE_ALGORITHM_FACTORY(LoKi,PIDTest)
// ============================================================================
// The END
// ============================================================================

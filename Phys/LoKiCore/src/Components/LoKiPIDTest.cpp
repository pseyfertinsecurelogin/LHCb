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
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
// LoKiCore
// ============================================================================
#include "LoKi/ILoKiSvc.h"
#include "LoKi/ParticleProperties.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class PIDTest
   *  Simple algorithm to veryfy the fast access to PID information
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-065-31
   */
  class PIDTest : public GaudiAlgorithm {
  public:
    // ========================================================================
    /// standard initialzation
    StatusCode initialize() override {
      StatusCode sc = GaudiAlgorithm::initialize();
      if ( sc.isFailure() ) { return sc; } // RETURN
      svc<LoKi::ILoKiSvc>( "LoKiSvc", true );
      m_svc = svc<LHCb::IParticlePropertySvc>( "LHCb::ParticlePropertySvc", true );
      Assert( 0 != m_svc, "Invalid pointer to  Particle Properety Service!" );
      return StatusCode::SUCCESS; // RETURN
    }
    // ========================================================================
    StatusCode execute() override {
      // avoid long names
      using namespace LoKi::Particles;
      // check
      Assert( 0 != m_svc, "Invalid pointer to  Particle Properety Service!" );
      // loop over properties:
      for ( LHCb::IParticlePropertySvc::iterator ipp = m_svc->begin(); m_svc->end() != ipp; ++ipp ) {
        const LHCb::ParticleProperty* pp = *ipp;
        if ( 0 == pp ) { continue; } // RETURN
        debug() << "Name/PID: "
                << "'" << pp->particle() << "':" << pp->pdgID() << endmsg;
        // verify name -> pid map
        Assert( pp->particle() == nameFromPID( pp->particleID() ),
                "Mismatch in Name/PID map for '" + pp->particle() + "'" );
        // verify pid  -> name map
        Assert( pp->particleID() == pidFromName( pp->particle() ),
                "Mismatch in PID/Name map for '" + pp->particle() + "'" );
      }
      return StatusCode::SUCCESS;
    }

    using GaudiAlgorithm::GaudiAlgorithm;

  private:
    // ========================================================================
    /// The pointer to particle property sevice
    LHCb::IParticlePropertySvc* m_svc = nullptr; // pointer to particle property sevice
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// Declaration of the Algorithm Factory
// ============================================================================
DECLARE_COMPONENT( LoKi::PIDTest )
// ============================================================================
// The END
// ============================================================================

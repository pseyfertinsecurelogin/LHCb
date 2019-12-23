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
#include "GaudiAlg/GaudiAlgorithm.h"
#include <algorithm>

// ----------------------------------------------------------------------------
// Implementation file for class: ServiceStarter
//
// 09/01/2012: Marco Clemencic
// ----------------------------------------------------------------------------

namespace LHCbAlgsTest {

  /** @class ServiceStarter ServiceStarter.h src/ServiceStarter.h
   *
   * Simple test algorithm to trigger the instantiation of a service in
   * a specific phase (initialize, start or execute).
   *
   * @author Marco Clemencic
   * @date 09/01/2012
   */
  class ServiceStarter final : public GaudiAlgorithm {

  public:
    /// Standard constructor
    using GaudiAlgorithm::GaudiAlgorithm;

    StatusCode initialize() override; ///< Algorithm initialization
    StatusCode start() override;      ///< Algorithm start
    StatusCode execute() override;    ///< Algorithm execution
    StatusCode finalize() override;   ///< Algorithm finalization

  private:
    StatusCode i_retrieveService( const std::string& currentPhase );

  private:
    /// Service to get.
    Gaudi::Property<std::string> m_serviceName{this, "Service", {}, "Service to retrieve."};

    /// When to get the service;
    Gaudi::Property<std::string> m_phase{
        this, "Phase", {}, "When to retrieve the service (initialize, start, execute)."};

    /// Internal reference to the retrieved service.
    SmartIF<IService> m_service;
  };

  DECLARE_COMPONENT( ServiceStarter )

  namespace {
    const std::vector<std::string> phases = {"initialize", "start", "execute"};
  }

  StatusCode ServiceStarter::i_retrieveService( const std::string& currentPhase ) {
    if ( !m_service && currentPhase == m_phase ) {
      info() << "Retrieving " << m_serviceName << endmsg;
      m_service = service( m_serviceName );
      if ( !m_service ) {
        error() << "Could not get service";
        return StatusCode::FAILURE;
      }
    }
    return StatusCode::SUCCESS;
  }

  // ============================================================================
  // Initialization
  // ============================================================================
  StatusCode ServiceStarter::initialize() {
    StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
    if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

    if ( m_serviceName.empty() ) {
      error() << "The property 'Service' must be specified." << endmsg;
      return StatusCode::FAILURE;
    }

    if ( m_phase.empty() || std::find( phases.begin(), phases.end(), m_phase ) == phases.end() ) {
      error() << "The property 'Phase' must be set to one of: ";
      for ( auto i = phases.begin(); i != phases.end(); ++i ) {
        if ( phases.begin() != i ) error() << ", ";
        error() << *i;
      }
      error() << endmsg;
      error() << "  current value: " << ( m_phase.empty() ? std::string( "(empty)" ) : m_phase.value() ) << endmsg;
      return StatusCode::FAILURE;
    }

    return i_retrieveService( "initialize" );
  }

  // ============================================================================
  // Main execution
  // ============================================================================
  StatusCode ServiceStarter::start() {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Start" << endmsg;
    return i_retrieveService( "start" );
  }

  // ============================================================================
  // Main execution
  // ============================================================================
  StatusCode ServiceStarter::execute() {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;
    return i_retrieveService( "execute" );
  }

  // ============================================================================
  // Finalize
  // ============================================================================
  StatusCode ServiceStarter::finalize() {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;
    m_service.reset();
    return GaudiAlgorithm::finalize(); // must be called after all other actions
  }
} // namespace LHCbAlgsTest
// ============================================================================

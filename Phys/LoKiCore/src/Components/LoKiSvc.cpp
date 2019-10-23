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
// STD & STL
// ============================================================================
#include <climits>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgContextSvc.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/IChronoSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IHistogramSvc.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiKernel/IStatSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/Incident.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/ServiceLocatorHelper.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/AuxFunBase.h"
#include "LoKi/ErrorReport.h"
#include "LoKi/Exception.h"
#include "LoKi/ILoKiSvc.h"
#include "LoKi/IReporter.h"
#include "LoKi/Services.h"
#include "LoKi/Welcome.h"
// ============================================================================
/** @file
 *
 *  Implementation file for class LoKiSvc
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
/*  @class LoKiSvc
 *  the simplest implementation of LoKi::ILoKiSvc interface
 *  @see LoKi::ILoKiSvc
 *  @date 2006-01-16
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
class LoKiSvc : public Service, public virtual LoKi::ILoKiSvc, public virtual IIncidentListener {
public:
  // ==========================================================================
  /** get "good" error reporter
   *  @return pointer to Good error reporter
   *  @see LoKi::IReporter
   */
  LoKi::IReporter* reporter() const override {
    if ( m_reporter ) { return m_reporter; } // RETURN

    IToolSvc* svc = toolSvc();
    if ( !svc ) { return nullptr; } // RETURN
    StatusCode sc = svc->retrieveTool( m_reporterName, m_reporter, this );
    return sc ? m_reporter : nullptr;
  }
  // ==========================================================================
  /** The the sequential event number
   *  (needed for the proper synchronizations checks)
   *  @return the sequential event number
   */
  unsigned long long event() const override { return m_event; }
  // ==========================================================================
public:
  // ==========================================================================
  /** get the pointer to Particle Property Service
   *  @return pointer to Particle Property Service
   *  @see LoKi::ILoKiSvc
   *  @see IParticlePropertySvc
   */
  LHCb::IParticlePropertySvc* ppSvc() const {
    if ( m_ppSvc ) { return m_ppSvc; }
    // locate the service
    StatusCode sc = service( "LHCb::ParticlePropertySvc", m_ppSvc, true );
    if ( sc.isFailure() ) {
      m_ppSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'PPSvc' could not be located", sc );
    }
    if ( !m_ppSvc ) { LOKI_EXCEPTION( "LoKiSvc: IPPSvc* points to NULL", sc ); }
    //
    return m_ppSvc;
  }
  // ==========================================================================
  /** get the pointer to Tool Service
   *  @return pointer to Tool Service
   *  @see LoKi::ILoKiSvc
   *  @see IToolSvc
   */
  IToolSvc* toolSvc() const {
    if ( m_toolSvc ) { return m_toolSvc; }
    // locate the service
    StatusCode sc = service( "ToolSvc", m_toolSvc, true );
    if ( sc.isFailure() ) {
      m_toolSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'ToolSvc' could not be located", sc );
    }
    if ( !m_toolSvc ) { LOKI_EXCEPTION( "LoKiSvc: IToolSvc* points to NULL", sc ); }
    //

    // make sure we finalize _prior_ to ToolSvc... we have pointers to
    // tools which get finalized and released by the ToolSvc during
    // ToolSvc::finalize, and we don't want dangling pointers...
    SmartIF<ISvcManager> mgr( serviceLocator() );
    auto                 prio = mgr->getPriority( "ToolSvc" );
    mgr->setPriority( name(), prio + 1 ).ignore();

    return m_toolSvc;
  }
  // ==========================================================================
  /** get the pointer to Algorithm Context Service
   *  @return pointer to Algorithm Context Service
   *  @see LoKi::ILoKiSvc
   *  @see IToolSvc
   */
  IAlgContextSvc* contextSvc() const {
    if ( m_contextSvc ) { return m_contextSvc; }
    // locate the service
    StatusCode sc = service( "AlgContextSvc", m_contextSvc, true );
    if ( sc.isFailure() ) {
      m_contextSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'AlgContextSvc' could not be located", sc );
    }
    if ( !m_contextSvc ) { LOKI_EXCEPTION( "LoKiSvc: IAlgContextSvc* points to NULL", sc ); }
    //
    return m_contextSvc;
  }
  // ==========================================================================
  /** get the pointer to Incident Service
   *  @return pointer to Incident Service
   *  @see IIncidentSvc
   */
  IIncidentSvc* incidentSvc() const {
    if ( m_incidentSvc ) { return m_incidentSvc; }
    // locate the service
    StatusCode sc = service( "IncidentSvc", m_incidentSvc, true );
    if ( sc.isFailure() ) {
      m_incidentSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'IncidentSvc' could not be located", sc );
    }
    if ( !m_incidentSvc ) { LOKI_EXCEPTION( "LoKiSvc: IIncicentSvc* points to NULL", sc ); }
    //
    return m_incidentSvc;
  }
  // ==========================================================================
  /** get the pointer to Histogram Service
   *  @return pointer to Histogram Service
   *  @see IIncidentSvc
   */
  IHistogramSvc* histoSvc() const {
    if ( m_histoSvc ) { return m_histoSvc; }
    // locate the service
    StatusCode sc = service( "HistogramDataSvc", m_histoSvc, true );
    if ( sc.isFailure() ) {
      m_histoSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'HistogramDataSvc' could not be located", sc );
    }
    if ( !m_histoSvc ) { LOKI_EXCEPTION( "LoKiSvc: IHistogramSvc* points to NULL", sc ); }
    //
    return m_histoSvc;
  }
  // ==========================================================================
  /** get the pointer to Event Data Provider
   *  @return pointer to Event Data Provider
   *  @see IDataPrioviderSvc
   */
  IDataProviderSvc* evtSvc() const {
    if ( m_evtSvc ) { return m_evtSvc; }
    // locate the service
    StatusCode sc = service( "EventDataSvc", m_evtSvc, true );
    if ( sc.isFailure() ) {
      m_evtSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'EventDataSvc' could not be located", sc );
    }
    if ( !m_evtSvc ) { LOKI_EXCEPTION( "LoKiSvc: IDataProviderSvc* points to NULL", sc ); }
    //
    return m_evtSvc;
  }
  // ==========================================================================
  /** get the pointer to Random Numbers Service
   *  @return pointer to Random Numbers Service
   *  @see IRndmGenSvc
   */
  IRndmGenSvc* rndmSvc() const {
    if ( m_rndmSvc ) { return m_rndmSvc; }
    // locate the service
    StatusCode sc = service( "RndmGenSvc", m_rndmSvc, true );
    if ( sc.isFailure() ) {
      m_rndmSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'RndmGenSvc' could not be located", sc );
    }
    if ( !m_rndmSvc ) { LOKI_EXCEPTION( "LoKiSvc: IRndmGenSvc* points to NULL", sc ); }
    //
    return m_rndmSvc;
  }
  // ==========================================================================
  /** get the pointer to Statistics
   *  @return pointer to Statistics Service
   *  @see IStatSvc
   */
  IStatSvc* statSvc() const {
    if ( m_statSvc ) { return m_statSvc; }
    // locate the service
    StatusCode sc = service( "ChronoStatSvc", m_statSvc, true );
    if ( sc.isFailure() ) {
      m_statSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'ChronoStatSvc' could not be located", sc );
    }
    if ( !m_statSvc ) { LOKI_EXCEPTION( "LoKiSvc: IStatSvc* points to NULL", sc ); }
    //
    return m_statSvc;
  }
  // ==========================================================================
  /** get the pointer to Chrono service
   *  @return pointer to Chrono Service
   *  @see IChronoSvc
   */
  IChronoSvc* chronoSvc() const {
    if ( m_chronoSvc ) { return m_chronoSvc; }
    // locate the service
    StatusCode sc = service( "ChronoStatSvc", m_chronoSvc, true );
    if ( sc.isFailure() ) {
      m_chronoSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'ChronoStatSvc' could not be located", sc );
    }
    if ( !m_chronoSvc ) { LOKI_EXCEPTION( "LoKiSvc: IChronoSvc* points to NULL", sc ); }
    //
    return m_chronoSvc;
  }
  // ==========================================================================
  /** get the pointer to Update Manager Service
   *  @return pointer to Update MAnager Service
   *  @see IChronoSvc
   */
  IUpdateManagerSvc* updateSvc() const {
    if ( m_updateSvc ) { return m_updateSvc; }
    // locate the service
    StatusCode sc = service( "UpdateManagerSvc", m_updateSvc, true );
    if ( sc.isFailure() ) {
      m_updateSvc = nullptr;
      LOKI_EXCEPTION( "LoKiSvc: 'UpdateManagerSvc' could not be located", sc );
    }
    if ( !m_updateSvc ) { LOKI_EXCEPTION( "LoKiSvc: IUpdateManagerSvc* points to NULL", sc ); }
    //
    return m_updateSvc;
  }
  // ==========================================================================
public:
  // ==========================================================================
  /// Inform that a new incident has occured
  void handle( const Incident& inc ) override {
    if ( IncidentType::BeginEvent == inc.type() ) {
      if ( m_event < std::numeric_limits<unsigned long long>::max() ) {
        ++m_event;
      } else {
        m_event = 1;
      }
    }
  }
  // ==========================================================================
public:
  // ==========================================================================
  /** general service initialization
   *  @see IService
   *  @return status code
   */
  StatusCode initialize() override {
    StatusCode sc = Service::initialize();
    if ( sc.isFailure() ) { return sc; }
    //
    //
    if ( m_welcome.value() ) { // welcome message
      auto& log = always() << '\n';
      LoKi::Welcome::instance().welcome( log.stream() );
      log << endmsg;
    }
    // RESET the sequential event number
    m_event = 0;
    //
    {
      /// subscribe the incident:
      IIncidentSvc* isvc = incidentSvc();
      if ( !isvc ) { return StatusCode::FAILURE; }
      isvc->addListener( this, IncidentType::BeginEvent );
    }
    //
    {
      IToolSvc* svc = toolSvc();
      if ( !svc ) { return StatusCode::FAILURE; }
      sc = svc->retrieveTool( m_reporterName, m_reporter, this );
      if ( sc.isFailure() ) { return StatusCode::FAILURE; }
      if ( !m_reporter ) { return StatusCode::FAILURE; }
    }
    //
    LoKi::Services& svc = LoKi::Services::instance();
    if ( !svc.lokiSvc() ) { svc.setLoKi( this ); }
    //
    // validate AuxFunBase
    if ( !LoKi::AuxFunBase::lokiSvc() ) { LoKi::AuxFunBase::setLoKiSvc( this ); }
    //
    LoKi::ErrorReport& rep = LoKi::ErrorReport::instance();
    if ( !rep.reporter() && m_reporter ) { rep.setReporter( m_reporter ); }
    //
    return StatusCode::SUCCESS;
  }
  // ==========================================================================
  /** general service finalization
   *  @see IService
   *  @return status code
   */
  StatusCode finalize() override {
    //
    { // static services
      LoKi::Services& svc = LoKi::Services::instance();
      svc.releaseAll().ignore();
    }
    //
    if ( m_reporter && m_toolSvc ) { m_toolSvc->releaseTool( m_reporter ).ignore(); }
    //
    m_reporter = nullptr;
    //
    {
      /// unsubscribe the incident:
      IIncidentSvc* isvc = incidentSvc();
      if ( !isvc ) { return StatusCode::FAILURE; } // RETURN
      isvc->removeListener( this );
    }
    // Invalidate AuxFunBase
    if ( LoKi::AuxFunBase::lokiSvc().same( this ) ) { LoKi::AuxFunBase::setLoKiSvc( nullptr ); }
    //
    if ( m_toolSvc ) {
      m_toolSvc->release();
      m_toolSvc = nullptr;
    }
    if ( m_ppSvc ) {
      m_ppSvc->release();
      m_ppSvc = nullptr;
    }
    if ( m_contextSvc ) {
      m_contextSvc->release();
      m_contextSvc = nullptr;
    }
    if ( m_incidentSvc ) {
      m_incidentSvc->release();
      m_incidentSvc = nullptr;
    }
    if ( m_histoSvc ) {
      m_histoSvc->release();
      m_histoSvc = nullptr;
    }
    if ( m_evtSvc ) {
      m_evtSvc->release();
      m_evtSvc = nullptr;
    }
    if ( m_rndmSvc ) {
      m_rndmSvc->release();
      m_rndmSvc = nullptr;
    }
    if ( m_statSvc ) {
      m_statSvc->release();
      m_statSvc = nullptr;
    }
    if ( m_chronoSvc ) {
      m_chronoSvc->release();
      m_chronoSvc = nullptr;
    }
    if ( m_updateSvc ) {
      m_updateSvc->release();
      m_updateSvc = nullptr;
    }
    //
    LoKi::ErrorReport& rep = LoKi::ErrorReport::instance();
    if ( rep.reporter() ) { rep.setReporter( nullptr ).ignore(); }
    //
    if ( m_welcome.value() ) { // goodbye message
      auto& log = always() << '\n';
      LoKi::Welcome::instance().goodbye( log.stream() );
      log << endmsg;
    }
    //
    return Service::finalize();
  }
  // ==========================================================================
  /** general service reinitialization
   *  @see IService
   *  @return status code
   */
  StatusCode reinitialize() override {
    StatusCode sc = Service::reinitialize();
    if ( sc.isFailure() ) { return sc; }
    //
    {
      /// unsubscribe the incident:
      IIncidentSvc* isvc = incidentSvc();
      if ( !isvc ) { return StatusCode::FAILURE; } // RETURN
      isvc->removeListener( this );
    }
    //
    // Invalidate AuxFunBase
    if ( LoKi::AuxFunBase::lokiSvc().same( this ) ) { LoKi::AuxFunBase::setLoKiSvc( nullptr ); }
    //
    if ( m_toolSvc ) {
      m_toolSvc->release();
      m_toolSvc = nullptr;
    }
    if ( m_ppSvc ) {
      m_ppSvc->release();
      m_ppSvc = nullptr;
    }
    if ( m_contextSvc ) {
      m_contextSvc->release();
      m_contextSvc = nullptr;
    }
    if ( m_incidentSvc ) {
      m_incidentSvc->release();
      m_incidentSvc = nullptr;
    }
    if ( m_histoSvc ) {
      m_histoSvc->release();
      m_histoSvc = nullptr;
    }
    if ( m_evtSvc ) {
      m_evtSvc->release();
      m_evtSvc = nullptr;
    }
    if ( m_rndmSvc ) {
      m_rndmSvc->release();
      m_rndmSvc = nullptr;
    }
    if ( m_statSvc ) {
      m_statSvc->release();
      m_statSvc = nullptr;
    }
    if ( m_chronoSvc ) {
      m_chronoSvc->release();
      m_chronoSvc = nullptr;
    }
    if ( m_updateSvc ) {
      m_updateSvc->release();
      m_updateSvc = nullptr;
    }
    //
    {
      /// subscribe the incident:
      IIncidentSvc* isvc = incidentSvc();
      if ( !isvc ) { return StatusCode::FAILURE; } // RETURN
      isvc->addListener( this, IncidentType::BeginEvent );
    }
    //
    { // static services:
      LoKi::Services& svc = LoKi::Services::instance();
      svc.releaseAll();
      if ( !svc.lokiSvc() ) { svc.setLoKi( this ); }
    }
    //
    // validate AuxFunBase
    if ( !LoKi::AuxFunBase::lokiSvc() ) { LoKi::AuxFunBase::setLoKiSvc( this ); }
    //
    if ( m_welcome.value() ) { // welcome message
      auto& log = always() << '\n';
      LoKi::Welcome::instance().welcome( log.stream() );
      log << endmsg;
    }
    //
    return StatusCode::SUCCESS;
  }
  // ==========================================================================
public:
  // ==========================================================================
  /** Query interfaces of Interface
   *  @param iid ID of Interface to be retrieved
   *  @param ppI Pointer to Location for interface pointer
   *  @see IInterface
   */
  StatusCode queryInterface( const InterfaceID& iid, void** ppI ) override {
    if ( !ppI ) { return StatusCode::FAILURE; }
    //
    *ppI = nullptr;
    if ( LoKi::ILoKiSvc::interfaceID() == iid ) {
      *ppI = static_cast<LoKi::ILoKiSvc*>( this );
    } else if ( IIncidentListener::interfaceID() == iid ) {
      *ppI = static_cast<IIncidentListener*>( this );
    }
    //
    // dispatch to the concrete service:
    //
    // Tool Service
    else if ( IToolSvc::interfaceID() == iid && toolSvc() ) {
      return toolSvc()->queryInterface( iid, ppI );
    }
    // Particle Property Service
    else if ( LHCb::IParticlePropertySvc::interfaceID() == iid && ppSvc() ) {
      return ppSvc()->queryInterface( iid, ppI );
    }
    // Algorithm  Context Service
    else if ( IAlgContextSvc::interfaceID() == iid && contextSvc() ) {
      return contextSvc()->queryInterface( iid, ppI );
    }
    // Incident Service
    else if ( IIncidentSvc::interfaceID() == iid && incidentSvc() ) {
      return incidentSvc()->queryInterface( iid, ppI );
    }
    // ServiceLocator
    else if ( ISvcLocator::interfaceID() == iid && serviceLocator() ) {
      return serviceLocator()->queryInterface( iid, ppI );
    }
    // IAlgManager
    else if ( IAlgManager::interfaceID() == iid && serviceLocator() ) {
      return serviceLocator()->queryInterface( iid, ppI );
    }
    // IHistogramSvc
    else if ( IHistogramSvc::interfaceID() == iid && histoSvc() ) {
      return histoSvc()->queryInterface( iid, ppI );
    }
    // IDataProviderSvc
    else if ( IDataProviderSvc::interfaceID() == iid && evtSvc() ) {
      return evtSvc()->queryInterface( iid, ppI );
    }
    // IRndmGenSvc
    else if ( IRndmGenSvc::interfaceID() == iid && rndmSvc() ) {
      return rndmSvc()->queryInterface( iid, ppI );
    }
    // IStatSvc
    else if ( IStatSvc::interfaceID() == iid && statSvc() ) {
      return statSvc()->queryInterface( iid, ppI );
    }
    // IChronoSvc
    else if ( IChronoSvc::interfaceID() == iid && chronoSvc() ) {
      return chronoSvc()->queryInterface( iid, ppI );
    }
    // IUpdateManagerSvc
    else if ( IUpdateManagerSvc::interfaceID() == iid && updateSvc() ) {
      return updateSvc()->queryInterface( iid, ppI );
    }
    // a bit more fun with the reporter
    else if ( LoKi::IReporter::interfaceID() == iid && reporter() ) {
      return reporter()->queryInterface( iid, ppI );
    }
    // message service ?
    else if ( IMessageSvc::interfaceID() == iid && msgSvc() ) {
      return msgSvc()->queryInterface( iid, ppI );
    }
    // message service ?
    else {
      return Service::queryInterface( iid, ppI );
    }
    //
    addRef();
    //
    return StatusCode::SUCCESS;
  }
  // ==========================================================================
public:
  // ==========================================================================
  /** standard constructor
   *  @param name service instance name
   *  @param pSvc pointer to the service locator
   */
  using Service::Service;
  // ==========================================================================
  /// virtual and protected destructor
  ~LoKiSvc() override {
    if ( m_reporter && m_toolSvc ) { m_toolSvc->releaseTool( m_reporter ); }
    //
    if ( m_toolSvc ) { m_toolSvc->release(); }
    if ( m_ppSvc ) { m_ppSvc->release(); }
    if ( m_contextSvc ) { m_contextSvc->release(); }
    if ( m_incidentSvc ) { m_incidentSvc->release(); }
    if ( m_histoSvc ) { m_histoSvc->release(); }
    if ( m_evtSvc ) { m_evtSvc->release(); }
    if ( m_rndmSvc ) { m_rndmSvc->release(); }
    if ( m_statSvc ) { m_statSvc->release(); }
    if ( m_chronoSvc ) { m_chronoSvc->release(); }
    if ( m_updateSvc ) { m_updateSvc->release(); }
  }
  // ==========================================================================
private:
  // ==========================================================================
  /// the particle property service
  mutable LHCb::IParticlePropertySvc* m_ppSvc = nullptr; // the particle property service
  /// the tool service
  mutable IToolSvc* m_toolSvc = nullptr; // the tool service
  /// the context service
  mutable IAlgContextSvc* m_contextSvc = nullptr; // the context service
  /// the incident service
  mutable IIncidentSvc* m_incidentSvc = nullptr; // the incident service
  /// the histogram service
  mutable IHistogramSvc* m_histoSvc = nullptr; //    histogram service
  /// the event data service
  mutable IDataProviderSvc* m_evtSvc = nullptr; //   event data service
  /// random numbers service
  mutable IRndmGenSvc* m_rndmSvc = nullptr; //       random numbers
  /// statistics service
  mutable IStatSvc* m_statSvc = nullptr; //           statistics
  /// chrono
  mutable IChronoSvc* m_chronoSvc = nullptr; //               chrono
  /// update
  mutable IUpdateManagerSvc* m_updateSvc = nullptr; //               update
  /// the default reporter
  mutable LoKi::IReporter* m_reporter = nullptr; // the default reporter
  /// the name of the default reporter
  Gaudi::Property<std::string> m_reporterName{this, "Reporter", "LoKi::Reporter/REPORT",
                                              "The type/name of default Reporter tool"};
  /// the event marker
  unsigned long long m_event = 0; // the event marker
  /// print welcome message
  Gaudi::Property<bool> m_welcome{this, "Welcome", true, "Show Welcome message"}; // print welcome message
  // ==========================================================================
};
// ============================================================================
DECLARE_COMPONENT( LoKiSvc )
// ============================================================================
// The END
// ============================================================================

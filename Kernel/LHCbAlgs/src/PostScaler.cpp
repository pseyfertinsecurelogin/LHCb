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
// Include files
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/RndmGenerators.h"
#include "Kernel/IEventCounter.h"
#include <string>

//-----------------------------------------------------------------------------
// Implementation file for class : PostScaler
//
// 2003-03-16 : Gloria Corti
//-----------------------------------------------------------------------------

/** @class PostScaler PostScaler.h
 *  Allows to prescale events at a given rate suing a random number (as Prescaler.cpp)
 *  or to force the fraction of accepted events to be as given by options (or both).
 *
 *  @author P. Koppenburg
 *  @date   2004-06-15
 */

class PostScaler final : public GaudiAlgorithm {

public:
  /// Standard constructor
  PostScaler( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  unsigned long long m_nEvents{0};    ///< Counter of events accepted
  unsigned long long m_nEventsAll{0}; ///< Counter of events entering
#ifdef GAUDI_PROPERTY_v2
  Gaudi::CheckedProperty<double>
#else
  DoubleProperty
#endif
                 m_percentPass;     ///< Minimal reduction rate to achieve (statistics mode)
  double         m_forcedReduction; ///< Percentage of events that should be passed (random number mode)
  double         m_margin;          ///< Safety margin (accept if acc events < m_event/m_forcedReduction + m_margin)
  IEventCounter* m_eventCounter = nullptr;
};

// Declaration of the Algorithm Factory

DECLARE_COMPONENT( PostScaler )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PostScaler::PostScaler( const std::string& name, ISvcLocator* pSvcLocator ) : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "PercentPass", m_percentPass = 100.0 );
  declareProperty( "ForcedReduction", m_forcedReduction = 1. );
  declareProperty( "SafetyMargin", m_margin = 0.5 );
  m_percentPass.verifier().setBounds( 0.0, 100.0 );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode PostScaler::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Initialize" << endmsg;

  if ( m_forcedReduction > 1. ) m_eventCounter = tool<IEventCounter>( "EvtCounter" );

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PostScaler::execute() {

  bool accepted = true;
  ++m_nEventsAll;

  // random number method
  if ( m_percentPass < 100. ) {
    Rndm::Numbers random( randSvc(), Rndm::Flat( 0.0, 100. ) );
    if ( random ) {
      const auto r = random();
      if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "Random number thrown: " << r << endmsg;
      if ( r > m_percentPass ) {
        if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "Random filter failed" << endmsg;
        accepted = false;
      } else {
        if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "Random filter passed" << endmsg;
      }
    }
  }

  // Event number fraction method
  if ( m_forcedReduction > 1. ) {
    const auto event = m_eventCounter->getEventCounter();
    if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "event number is now " << event << endmsg;

    const auto max_evts = (double)event / m_forcedReduction + m_margin;
    //  max_evts = max_evts + 3*sqrt(max_evts);  /// 3 sigma security
    if ( m_nEvents > max_evts ) {
      if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) )
        verbose() << "Filter not passed as " << m_nEvents << " > " << max_evts << " (reduction 1/" << m_forcedReduction
                  << " + " << m_margin << ")" << endmsg;
      accepted = false;
    } else {
      if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "Filter passed." << endmsg;
    }
  }

  // event passed
  if ( accepted ) ++m_nEvents;

  setFilterPassed( accepted );
  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode PostScaler::finalize() {
  info() << "Event Filtered " << m_nEventsAll << endmsg;
  info() << "Event Accepted " << m_nEvents << endmsg;
  return GaudiAlgorithm::finalize();
}

//=============================================================================

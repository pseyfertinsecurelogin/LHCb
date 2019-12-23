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
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "Kernel/IEventCounter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : EvtCounter
//
// 2004-06-15 : Patrick KOPPENBURG
//-----------------------------------------------------------------------------

/** @class EvtCounter EvtCounter.h
 *
 *  @author Patrick KOPPENBURG
 *  @date   2004-06-15
 */
class EvtCounter final : public extends<GaudiTool, IEventCounter, IIncidentListener> {

public:
  /// Standard constructor
  using extends::extends;

  /// IListener interface to be triggered at begin of event
  void handle( const Incident& ) override { this->incrementEventCounter(); }

  /// Initialize method
  StatusCode initialize() override {
    return GaudiTool::initialize().andThen( [&] {
      m_eventCounter = m_initialCount;
      if ( m_useIncident.value() ) {
        // Register to the Incident service to be notified at the begin of one event
        incSvc()->addListener( this, "BeginEvent", 101 );
        // take into account that the increment will be called once before the first event
        --m_eventCounter;
      }
    } );
  }

  /// Get Event counter
  unsigned long long getEventCounter() const noexcept override { return m_eventCounter; }

  /// Increment Event counter
  void incrementEventCounter() override { ++m_eventCounter; }

  /// Set Event counter
  void setEventCounter( unsigned long long n ) override { m_eventCounter = n; }

private:
  unsigned long long                  m_eventCounter{0}; ///< number of events processed
  Gaudi::Property<unsigned long long> m_initialCount{
      this, "InitialCount", 1, "Value to be used for the first event."}; ///< allow to change the number used for the
                                                                         ///< first event (property InitialCount)
  Gaudi::Property<bool> m_useIncident{
      this, "UseIncident", true,
      "Whether to increment the counter at every BeginEvent incident."}; ///< whether to increment of BeginEvent
                                                                         ///< incident or not
};

// Declaration of the Tool Factory

DECLARE_COMPONENT( EvtCounter )

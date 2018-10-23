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
#ifndef EVTCOUNTER_H
#define EVTCOUNTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "Kernel/IEventCounter.h"

/** @class EvtCounter EvtCounter.h
 *
 *  @author Patrick KOPPENBURG
 *  @date   2004-06-15
 */
class EvtCounter final : public extends2<GaudiTool,
                                         IEventCounter,
                                         IIncidentListener>
{

 public:

  /// Standard constructor
  EvtCounter( const std::string& type,
              const std::string& name,
              const IInterface* parent);

  virtual ~EvtCounter() = default; ///< Destructor

  /// IListener interface to be triggered at begin of event
  void handle(const Incident&) override { this->incrementEventCounter(); }

  /// Initialize method
  StatusCode initialize() override;

  /// Get Event counter
  unsigned long long getEventCounter() const noexcept override { return m_eventCounter; }

  /// Increment Event counter
  void incrementEventCounter() override { ++m_eventCounter; }

  /// Set Event counter
  void setEventCounter( unsigned long long n ) override { m_eventCounter = n; }

 private:

  unsigned long long m_eventCounter{0}; ///< number of events processed
  unsigned long long m_initialCount; ///< allow to change the number used for the first event (property InitialCount)
  bool m_useIncident; ///< whether to increment of BeginEvent incident or not

};

#endif // EVTCOUNTER_H

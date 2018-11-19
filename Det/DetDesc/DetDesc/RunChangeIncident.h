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
#pragma once

#include "GaudiKernel/Incident.h"
#include "GaudiKernel/Time.h"

namespace IncidentType
{
  const static std::string RunChange = "RunChange"; ///< Change of run number
}

/** @class RunChangeIncident
 *
 *  Specialized incident class for run number change incidents.
 *
 */
class RunChangeIncident final: public Incident {
public:
  /// Constructor
  RunChangeIncident(const std::string& source, // Source(service or alg) name)
                    unsigned long run,
                    const Gaudi::Time& evtTime = Gaudi::Time::max())
  : Incident(source, IncidentType::RunChange), m_run(run), m_eventTime(evtTime) {}

  /// Get the new run number
  inline unsigned long runNumber() const { return m_run; }
  inline const Gaudi::Time& eventTime() const { return m_eventTime; }

private:
  /// New run number
  unsigned long m_run;
  /// Current event time
  Gaudi::Time m_eventTime;
};

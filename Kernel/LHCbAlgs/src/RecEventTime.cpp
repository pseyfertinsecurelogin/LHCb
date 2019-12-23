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
#include "Event/RecHeader.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IEventTimeDecoder.h" // Interface

/** Simple implementation of IEventTimeDecoder that takes the event
 *  time from LHCb::RecHeader.
 *
 *  @author Marco Clemencic
 *  @date   2010-09-23
 */

class RecEventTime final : public extends<GaudiTool, IEventTimeDecoder> {

public:
  /// Standard constructor
  using extends::extends;

  // --- implementation of IEventTimeDecoder ---
  /// Retrieve the event time from RecHeader (if present).
  /// @return The time of current event.
  Gaudi::Time getTime() const override {
    // If the rec header is available, return its event time
    auto* h = m_recHeaderLoc.getIfExists();
    // return the time
    return ( h ? h->gpsTime() * 1000 : Gaudi::Time::epoch() );
  }

private:
  /// Location of the RecHeader in the transient store.
  DataObjectReadHandle<LHCb::RecHeader> m_recHeaderLoc{this, "RecHeaderLocation", LHCb::RecHeaderLocation::Default,
                                                       "Location for RecHeader"};
};

DECLARE_COMPONENT( RecEventTime )

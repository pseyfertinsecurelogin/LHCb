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
#include "Event/ODIN.h"
#include "GaudiAlg/FilterPredicate.h"
#include <algorithm>
#include <utility>
#include <vector>

//-----------------------------------------------------------------------------
// Implementation file for class : FilterByRunEvent
//
// 2007-01-18 : Patrick Spradlin
//-----------------------------------------------------------------------------

/** @class FilterByRunEvent FilterByRunEvent.h
 *
 *
 *  @author Patrick Spradlin
 *  @date   2007-01-18
 *
 *  @par Algorithm purpose
 *    @c FilterByRunEvent filters events based on the
 *    (run number, event number) pair of the event.
 *
 *  @par Algorithm properties
 *    @c FilterByRunEvent has two properties:
 *    - @c PassSelectedEvents:  A boolean with a default value of @c true.
 *      If @c PassSelectedEvents is set to @c true (the default),
 *      @c FilterByRunEvent will pass only the specified events and reject all
 *      other events. If @c PassSelectedEvents is set to @c false,
 *      @c FilterByRunEvent will reject the specified events and pass all
 *      other events.
 *    - @c RunEventNumList:  A vector of (run number, event number) pairs.
 *      By default, @c RunEventNumList is empty.  In python configurables,
 *      @c RunEventNumList is specified with a list of pairs (tuples of
 *      length 2).
 *
 *    The default configuration of @c FilterByRunEvent is a fail-all filter.
 *
 *  @par Example of usage
 *
 *    In order to configure a sequencer @c MySequence to skip the two events
 *    with (run number, event number) pairs (86456, 421) and (48621, 3):
 *    @verbatim
.....
MySequence = GaudiSequencer('MySequence')
MyFilterByRunEvent = FilterByRunEvent('MyFilterByRunEvent')
MySequence.Members += [ MyFilterByRunEvent, .... ]

# Set PassSelectedEvents to false, so that it rejects the selected events
MyFilterByRunEvent.PassSelectedEvents = 0
# std::vector< std::pair<int, int> > configured with a list of pairs
MyFilterByRunEvent.RunEventNumList = [ (86456, 421), (48621, 3) ]
.... @endverbatim
 */
class FilterByRunEvent final : public Gaudi::Functional::FilterPredicate<bool( const LHCb::ODIN& )> {
  // FIXME: needs to be: std::vector<std::pair<unsigned int,unsigned long long> > m_events; ///< Run/event number pairs
  Gaudi::Property<std::vector<std::pair<int, int>>> m_events{this, "RunEventNumList", {}, [=]( Property& ) {
                                                               std::sort( m_events.begin(), m_events.end() );
                                                             }}; ///< Run/event number pairs
  Gaudi::Property<bool> m_passSelect{this, "PassSelectedEvents", true, "If true, will pass list events; false, fail"};

public:
  /// Standard constructor
  FilterByRunEvent( const std::string& name, ISvcLocator* pSvcLocator )
      : FilterPredicate{name, pSvcLocator, {"ODINLocation", LHCb::ODINLocation::Default}} {}

  bool operator()( const LHCb::ODIN& odin ) const override {
    const bool lcl_sel = std::any_of(
        m_events.begin(), m_events.end(),
        [runEv = std::pair{(int)odin.runNumber(), (int)odin.eventNumber()}]( const auto& p ) { return runEv == p; } );

    const bool lcl_pass = ( m_passSelect.value() ? lcl_sel : !lcl_sel );
    if ( lcl_pass ) info() << "Passing Run " << odin.runNumber() << " event number " << odin.eventNumber() << endmsg;
    return lcl_pass;
  }
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FilterByRunEvent )

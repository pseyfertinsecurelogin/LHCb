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

//-----------------------------------------------------------------------------
// Implementation file for class : ODINTimeFilter
//
// 2009-10-07 : Olivier Deschamps
//-----------------------------------------------------------------------------

/** @class ODINTimeFilter ODINTimeFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2009-10-07
 */
class ODINTimeFilter : public Gaudi::Functional::FilterPredicate<bool( const LHCb::ODIN& )> {
public:
  /// Standard constructor
  ODINTimeFilter( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  bool       operator()( const LHCb::ODIN& ) const override;
  StatusCode finalize() override; ///< Algorithm finalization

private:
  void criteriaPrintOut();

  mutable Gaudi::Accumulators::Counter<> m_eventCount{this, "Event"};
  mutable Gaudi::Accumulators::Counter<> m_filteredEventCount{this, "Filtered Events"};

  // event ID intervall selector
  Gaudi::Property<bool>                      m_evt{this, "eventSelector", false};
  Gaudi::Property<std::pair<double, double>> m_eRange{this, "eRange", {-1, -1}};
  // time intervall selector
  Gaudi::Property<bool>                m_time{this, "timeSelector", true};
  Gaudi::Property<std::pair<int, int>> m_yRange{this, "yRange", {-1, -1}};
  Gaudi::Property<std::pair<int, int>> m_mRange{this, "mRange", {-1, -1}};
  Gaudi::Property<std::pair<int, int>> m_dRange{this, "dRange", {-1, -1}};
  Gaudi::Property<std::pair<int, int>> m_hRange{this, "hRange", {-1, -1}};
  Gaudi::Property<std::pair<int, int>> m_mnRange{this, "mnRange", {-1, -1}};
  Gaudi::Property<std::pair<int, int>> m_sRange{this, "sRange", {-1, -1}};
  Gaudi::Property<std::pair<int, int>> m_nsRange{this, "nsRange", {-1, -1}};
  // BCID interval Range
  Gaudi::Property<bool>                m_bx{this, "BCIDSelector", false};
  Gaudi::Property<std::pair<int, int>> m_bRange{this, "bRange", {-1, -1}};

  Gaudi::Property<bool> m_loc{this, "localTime", true};
  Gaudi::Property<bool> m_print{this, "eventTime", false};
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( ODINTimeFilter )

namespace {
  int val( int v1, int v2, int v3 = 0 ) {
    if ( v1 < 0 ) return v2;
    return ( v1 + v3 );
  }
  bool def( std::pair<double, double> range ) { return ( range.first >= 0 && range.second >= 0 ); }
  bool def( std::pair<int, int> range ) { return ( range.first >= 0 && range.second >= 0 ); }
  bool check( unsigned long long val, std::pair<double, double> range ) {
    if ( !def( range ) ) return true;
    return val >= (unsigned long long)range.first && val <= (unsigned long long)range.second;
  }

  bool check( int val, std::pair<int, int> range ) {
    if ( !def( range ) ) return true;
    return val >= range.first && val <= range.second;
  }
} // namespace

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ODINTimeFilter::ODINTimeFilter( const std::string& name, ISvcLocator* pSvcLocator )
    : FilterPredicate{name, pSvcLocator, {"ODINLocation", LHCb::ODINLocation::Default}} {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode ODINTimeFilter::initialize() {
  return FilterPredicate::initialize().andThen( [&] { criteriaPrintOut(); } );
}

//=============================================================================
// Main execution
//=============================================================================
bool ODINTimeFilter::operator()( LHCb::ODIN const& odin ) const {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // get time & eventID
  Gaudi::Time        time  = odin.eventTime();
  unsigned long long event = odin.eventNumber();
  int                run   = odin.runNumber();
  int                bx    = odin.bunchId();

  // timing check
  // WARNING : month from 1->12 (Gaudi::Time uses 0->11)

  // timing selection
  bool pass = true;
  ++m_eventCount;
  if ( m_time.value() ) {
    if ( def( m_yRange ) || def( m_mRange ) || def( m_dRange ) || def( m_hRange ) || def( m_mnRange ) ||
         def( m_sRange ) || def( m_sRange ) ) {

      Gaudi::Time minTime = Gaudi::Time(
          val( m_yRange.value().first, time.year( m_loc ) ), val( m_mRange.value().first, time.month( m_loc ), -1 ),
          val( m_dRange.value().first, time.day( m_loc ) ), val( m_hRange.value().first, time.hour( m_loc ) ),
          val( m_mnRange.value().first, time.minute( m_loc ) ), val( m_sRange.value().first, time.second( m_loc ) ),
          val( m_nsRange.value().first, time.nsecond() ), m_loc );

      Gaudi::Time maxTime = Gaudi::Time(
          val( m_yRange.value().second, time.year( m_loc ) ), val( m_mRange.value().second, time.month( m_loc ), -1 ),
          val( m_dRange.value().second, time.day( m_loc ) ), val( m_hRange.value().second, time.hour( m_loc ) ),
          val( m_mnRange.value().second, time.minute( m_loc ) ), val( m_sRange.value().second, time.second( m_loc ) ),
          val( m_nsRange.value().second, time.nsecond() ), m_loc );

      if ( minTime > time || maxTime < time ) pass = false;
    }
  }
  if ( m_evt.value() && !check( event, m_eRange ) ) pass = false;
  if ( m_bx.value() && !check( bx, m_bRange ) ) pass = false;

  if ( pass ) ++m_filteredEventCount;

  if ( m_print.value() )
    info() << "[Run : " << run << ", EventId  : " << event << ", BCID : " << bx << "]"
           << " @ EventTime : " << time.year( m_loc.value() ) << "/" << time.month( m_loc.value() ) + 1 << "/"
           << time.day( m_loc.value() ) << " " << time.hour( m_loc.value() ) << ":" << time.minute( m_loc.value() )
           << ":" << time.second( m_loc.value() ) << "::" << time.nsecond() << " (accepted :  " << pass << ")"
           << endmsg;
  return pass;
}

void ODINTimeFilter::criteriaPrintOut() {
  bool ok1 = true;
  bool ok2 = true;
  bool ok3 = true;
  if ( !def( m_yRange ) && !def( m_mRange ) && !def( m_dRange ) && !def( m_hRange ) && !def( m_mnRange ) &&
       !def( m_sRange ) && !def( m_nsRange ) )
    ok1 = false;

  if ( !def( m_eRange ) ) ok2 = false;
  if ( !def( m_bRange ) ) ok3 = false;

  if ( !ok1 && !ok2 && !ok3 )
    info() << "Empty filtering criteria - all events passing " << endmsg;
  else {
    if ( !ok1 ) info() << "No event intervall defined" << endmsg;
    if ( !ok2 ) info() << "No time intervall defined" << endmsg;
    if ( !ok3 ) info() << "No BCID intervall defined" << endmsg;
    info() << "Filter event in : " << endmsg;
    if ( def( m_yRange ) )
      info() << "year    Range [" << m_yRange.value().first << "," << m_yRange.value().second << "]" << endmsg;
    if ( def( m_mRange ) )
      info() << "month   Range [" << m_mRange.value().first << "," << m_mRange.value().second << "]" << endmsg;
    if ( def( m_dRange ) )
      info() << "day     Range [" << m_dRange.value().first << "," << m_dRange.value().second << "]" << endmsg;
    if ( def( m_hRange ) )
      info() << "hour    Range [" << m_hRange.value().first << "," << m_hRange.value().second << "]" << endmsg;
    if ( def( m_mnRange ) )
      info() << "minute  Range [" << m_mnRange.value().first << "," << m_mnRange.value().second << "]" << endmsg;
    if ( def( m_sRange ) )
      info() << "second  Range [" << m_sRange.value().first << "," << m_sRange.value().second << "]" << endmsg;
    if ( def( m_sRange ) )
      info() << "nsecond  Range [" << m_nsRange.value().first << "," << m_nsRange.value().second << "]" << endmsg;
    if ( def( m_eRange ) )
      info() << "eventID    Range [" << m_eRange.value().first << "," << m_eRange.value().second << "]" << endmsg;
    if ( def( m_bRange ) )
      info() << "BCID    Range [" << m_bRange.value().first << "," << m_bRange.value().second << "]" << endmsg;
  }
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode ODINTimeFilter::finalize() {
  criteriaPrintOut();
  return FilterPredicate::finalize(); // must be called after all other actions
}

//=============================================================================

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
#include <algorithm>
// from LHCb
#include "Event/ODIN.h"
// local
#include "OdinTypesFilter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : OdinTypesFilter
//
// 2008-02-05 : Olivier Deschamps
//-----------------------------------------------------------------------------

namespace {
  using namespace OdinTypesFilter_details;

  static const auto  s_And = std::string{"ODIN Filter ("} + toString( Log_t::And ) + ")";
  static const auto  s_Or  = std::string{"ODIN Filter ("} + toString( Log_t::Or ) + ")";
  const std::string& odin_filter_name( Log_t l ) {
    switch ( l ) {
    case Log_t::And:
      return s_And;
    case Log_t::Or:
      return s_Or;
    }
    throw std::runtime_error( std::string{"IMPOSSIBLE: "} + __func__ );
  }

  template <typename... Args>
  bool boolean_combine( Log_t op, Args&&... args ) {
    switch ( op ) {
    case Log_t::And:
      return ( ... && std::forward<Args>( args ) );
    case Log_t::Or:
      return ( ... || std::forward<Args>( args ) );
    }
    throw std::runtime_error( std::string{"IMPOSSIBLE: "} + __func__ );
  }

} // namespace

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( OdinTypesFilter )

//=============================================================================
// Initialization
//=============================================================================
StatusCode OdinTypesFilter::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  // selection :
  info() << "Accepted BXTypes : " << m_bxs.value() << endmsg;
  info() << toString( m_log.value() ) << endmsg;
  info() << "Accepted TriggerTypes : " << m_trs.value() << endmsg;
  info() << toString( m_log.value() ) << endmsg;
  info() << "Accepted ReadoutTypes : " << m_ros.value() << endmsg;
  info() << toString( m_log.value() ) << endmsg;
  info() << "Accepted CalibrationTypes : " << m_cls.value() << endmsg;
  info() << toString( m_log.value() ) << endmsg;
  info() << "TAE Window in [" << m_winmin + 1 << "," << m_winmax - 1 << "]" << endmsg;

  // warn about trivial requests
  if ( m_log == Log_t::And &&
       boolean_combine( Log_t::Or, m_bxs.empty(), m_trs.empty(), m_ros.empty(), m_winmin >= m_winmax, m_cls.empty() ) )
    Warning( "BXTypes, TriggerTypes, ReadoutTypes or TAEWindow is empty : ALL events will be rejected !!",
             StatusCode::SUCCESS )
        .ignore();
  if ( m_log == Log_t::Or &&
       boolean_combine( Log_t::And, m_bxs.empty(), m_trs.empty(), m_ros.empty(), m_winmin >= m_winmax, m_cls.empty() ) )
    Warning( "BXTypes, TriggerTypes, ReadoutTypes and TAEWindow are empties : ALL events will be rejected !!",
             StatusCode::SUCCESS )
        .ignore();
  if ( boolean_combine( m_log, m_bxs.value().front() == odin_bx_type_t::All(),
                        m_cls.value().front() == odin_calibration_type_t::All(),
                        m_trs.value().front() == odin_trigger_type_t::All(),
                        m_ros.value().front() == odin_readout_type_t::All(), m_winmin<0, m_winmax> 7 ) )
    Warning( "OdinTypesFilter has no effect : ALL events will be accepted !!", StatusCode::SUCCESS ).ignore();

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode OdinTypesFilter::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  ++m_all;

  // treat trivial requests
  setFilterPassed( true );
  if ( boolean_combine( m_log, m_bxs.value().front() == odin_bx_type_t::All(),
                        m_cls.value().front() == odin_calibration_type_t::All(),
                        m_trs.value().front() == odin_trigger_type_t::All(),
                        m_ros.value().front() == odin_readout_type_t::All(), m_winmin < 0 && m_winmax > 7 ) ) {
    m_acc++;
    return StatusCode::SUCCESS;
  }

  setFilterPassed( false );
  if ( boolean_combine( m_log,
                        ( m_bxs.empty(), m_cls.empty(), m_trs.empty(), m_ros.empty(), m_winmin >= m_winmax ) ) ) {
    return StatusCode::SUCCESS;
  }

  // get ODIN
  LHCb::ODIN* odin = getIfExists<LHCb::ODIN>( LHCb::ODINLocation::Default );
  if ( !odin ) return Error( "ODIN cannot be loaded - FilterPassed = false", StatusCode::SUCCESS );

  if ( msgLevel( MSG::DEBUG ) )
    debug() << " Trigger Type : " << odin->triggerType() << " BXType : " << odin->bunchCrossingType() << endmsg;

  auto equals_ = []( auto val ) { return [=]( const auto& i ) { return val == i; }; };
  bool clPass  = std::any_of( m_cls.begin(), m_cls.end(), equals_( odin->calibrationType() ) );
  bool bxPass  = std::any_of( m_bxs.begin(), m_bxs.end(), equals_( odin->bunchCrossingType() ) );
  bool trPass  = std::any_of( m_trs.begin(), m_trs.end(), equals_( odin->triggerType() ) );
  bool roPass  = std::any_of( m_ros.begin(), m_ros.end(), equals_( odin->readoutType() ) );
  bool taePass =
      ( (int)odin->timeAlignmentEventWindow() > m_winmin && (int)odin->timeAlignmentEventWindow() < m_winmax );

  setFilterPassed( boolean_combine( m_log, trPass, bxPass, roPass, taePass, clPass ) );

  if ( filterPassed() ) ++m_acc;

  counter( odin_filter_name( m_log.value() ) ) += int( filterPassed() );

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode OdinTypesFilter::finalize() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;

  info() << "Accepted BXTypes : " << m_bxs.value() << endmsg;
  info() << toString( m_log.value() ) << endmsg;
  info() << "Accepted TriggerTypes : " << m_trs.value() << endmsg;
  info() << toString( m_log.value() ) << endmsg;
  info() << "Accepted CalibrationTypes : " << m_cls.value() << endmsg;
  info() << toString( m_log.value() ) << endmsg;
  info() << "Accepted ReadoutTypes : " << m_ros.value() << endmsg;
  info() << toString( m_log.value() ) << endmsg;
  info() << "TAE Window in [" << m_winmin.value() << "," << m_winmax.value() << "]" << endmsg;
  info() << "   ---> " << m_acc << " accepted events among " << m_all << endmsg;

  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

//=============================================================================

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
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToStream.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/L0.h"
// ============================================================================
/** @file
 *  Implementation file for namespace LoKi::L0
 *  @date 2008-09-19
 *  @author Vanya BELYAEV Ivan.Belyaev@niklhef.nl
 */
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::L0::Valid* LoKi::L0::Valid::clone() const { return new LoKi::L0::Valid( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::Valid::operator()( const LHCb::L0DUReport* a ) const { return a && a->valid(); }
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::Valid::fillStream( std::ostream& s ) const { return s << "L0_VALID"; }
// ============================================================================

// ============================================================================
// constructor from bx-id L
// ============================================================================
LoKi::L0::SumEt::SumEt( const int bx ) : LoKi::AuxFunBase( std::tie( bx ) ), m_bx( bx ) {}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::SumEt::fillStream( std::ostream& s ) const {
  if ( 0 == m_bx ) { return s << "L0_SUMET0"; }
  return s << "L0_SUMET(" << m_bx << ")";
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::L0::SumEt::operator()( const LHCb::L0DUReport* a ) const {
  Assert( a, "L0DUReport* point to NULL!" );
  return a->sumEt( m_bx );
}

// ============================================================================
// constructor from data name
// ============================================================================
LoKi::L0::DataValue::DataValue( const std::string& name ) : LoKi::AuxFunBase( std::tie( name ) ), m_name( name ) {}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::DataValue::fillStream( std::ostream& s ) const { return s << "L0_DATA('" << name() << "')"; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::L0::DataValue::operator()( const LHCb::L0DUReport* a ) const {
  Assert( a, "L0DUReport* point to NULL!" );
  return a->dataValue( name() );
}

// ============================================================================
// constructor from data name
// ============================================================================
LoKi::L0::DataDigit::DataDigit( const std::string& name )
    : LoKi::AuxFunBase( std::tie( name ) ), LoKi::L0::DataValue( name ) {}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::DataDigit::fillStream( std::ostream& s ) const { return s << "L0_DIGIT('" << name() << "')"; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::L0::DataDigit::operator()( const LHCb::L0DUReport* a ) const {
  Assert( a, "L0DUReport* point to NULL!" );
  return a->dataDigit( name() );
}

// ============================================================================
// constructor from decision mask
// ============================================================================
LoKi::L0::Decision::Decision( const int mask ) : LoKi::AuxFunBase( std::tie( mask ) ), m_mask( mask ) {}
// ============================================================================
// MANDATORY: clone method ('virtual constructor')
// ============================================================================
LoKi::L0::Decision* LoKi::L0::Decision::clone() const { return new LoKi::L0::Decision( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::Decision::operator()( const LHCb::L0DUReport* a ) const {
  Assert( a, "L0DUReport* point to NULL!" );
  return a->decision( m_mask );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::Decision::fillStream( std::ostream& s ) const {
  switch ( m_mask ) {
  case LHCb::L0DUDecision::Physics:
    return s << " L0_DECISION_PHYSICS ";
  case LHCb::L0DUDecision::Beam1:
    return s << " L0_DECISION_BEAM1 ";
  case LHCb::L0DUDecision::Beam2:
    return s << " L0_DECISION_BEAM2 ";
  case LHCb::L0DUDecision::Disable:
    return s << " L0_DECISION_DISABLE ";
  default:
    return s << " L0_DECISION( " << m_mask << ") ";
  }
  // This is unreachable
  //  return s << " L0_DECISION( " << m_mask << ") " ;
}
// ============================================================================

// ============================================================================
// constructor from decision mask
// ============================================================================
LoKi::L0::SumDecision::SumDecision( const int mask, const int bx )
    : LoKi::AuxFunBase( std::tie( mask, bx ) ), m_mask( mask ), m_bx( bx ) {}

// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::SumDecision::operator()( const LHCb::L0DUReport* a ) const {
  Assert( a, "L0DUReport* point to NULL!" );
  return a->decisionFromSummary( m_mask, m_bx );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::SumDecision::fillStream( std::ostream& s ) const {
  if ( 0 == m_bx && LHCb::L0DUDecision::Physics == m_mask ) {
    return s << " L0_SUMDECISION_PHYSICS ";
  } else if ( 0 == m_bx && LHCb::L0DUDecision::Beam1 == m_mask ) {
    return s << " L0_SUMDECISION_BEAM1   ";
  } else if ( 0 == m_bx && LHCb::L0DUDecision::Beam2 == m_mask ) {
    return s << " L0_SUMDECISION_BEAM2   ";
  }
  //
  return s << " L0_SUMDECISION ( " << m_mask << " , " << m_bx << " ) ";
}

// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::ForceBit::operator()( const LHCb::L0DUReport* a ) const {
  Assert( a, "L0DUReport* point to NULL!" );
  return a->forceBit();
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::L0::Tck::operator()( const LHCb::L0DUReport* a ) const {
  Assert( a, "L0DUReport* point to NULL!" );
  return a->tck();
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::TimingBit::operator()( const LHCb::L0DUReport* a ) const {
  Assert( a, "L0DUReport* point to NULL!" );
  return a->timingTriggerBit();
}
// ============================================================================

// ============================================================================
// channel decision by channel name
// ============================================================================
LoKi::L0::ConditionValue::ConditionValue( const std::string& channel, const int bx )
    : LoKi::AuxFunBase( std::tie( channel, bx ) ), LoKi::L0::ChannelDecision( channel, bx ) {}
// ============================================================================
// channel decision by channel names ("OR")
// ============================================================================
LoKi::L0::ConditionValue::ConditionValue( const std::vector<std::string>& channels, const int bx )
    : LoKi::AuxFunBase( std::tie( channels, bx ) ), LoKi::L0::ChannelDecision( channels, bx ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::ConditionValue::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    // convert names into conditions
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( config, "LHCb::L0DUConfig* points to NULL" );
    const LHCb::L0DUElementaryCondition::Map& m = config->conditions();
    for ( const auto& name : names() ) {
      auto ifind = m.find( name );
      if ( m.end() == ifind ) {
        Error( "Non-existing condition '" + name + "' has been requested" );
        continue;
      }
      const auto* channel = ifind->second;
      Assert( channel, "LHCb::L0DUElementaryCondition* points to NULL" );
      addChannel( channel->id() );
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined channels
  return std::any_of( std::begin( channels() ), std::end( channels() ),
                      [&]( Channels::const_reference c ) { return a->conditionValue( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::ConditionValue::fillStream( std::ostream& s ) const {
  // ==========================================================================
  if ( 1 == names().size() ) { return s << "L0_CONDITION('" << names().front() << "'," << bx() << ")"; }
  // ==========================================================================
  return s << "L0_CONDITION(" << Gaudi::Utils::toString( names() ) << "," << bx() << ")";
  // ==========================================================================
}

// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::ForceBit::fillStream( std::ostream& s ) const { return s << "L0_FORCEBIT"; }
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::Tck::fillStream( std::ostream& s ) const { return s << "L0_TCK"; }
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::TimingBit::fillStream( std::ostream& s ) const { return s << "L0_TIMINGBIT"; }
// ============================================================================

// ============================================================================
// channel decision by channel names ("OR")
// ============================================================================
LoKi::L0::ChannelDecision::ChannelDecision( const std::vector<std::string>& channels, const int bx )
    : LoKi::AuxFunBase( std::tie( channels, bx ) ), m_names( channels ), m_bx( bx ), m_tckPrev( 0 ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::ChannelDecision::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( 0 != a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    // convert names into channels
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( config, "LHCb::L0DUConfig* points to NULL" );
    const LHCb::L0DUChannel::Map& m = config->channels();
    for ( const auto& name : names() ) {
      auto ifind = m.find( name );
      if ( m.end() == ifind ) {
        Error( "Non-existing channel '" + name + "' has been requested" );
        continue;
      }
      const auto* channel = ifind->second;
      Assert( channel, "LHCb::L0DUChannel* points to NULL" );
      if ( channel->decisionType() != 0 ) addChannel( channel->id() );
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined channels
  return std::any_of( std::begin( channels() ), std::end( channels() ),
                      [&]( Channels::const_reference c ) { return a->channelDecision( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::ChannelDecision::fillStream( std::ostream& s ) const {
  // ==========================================================================
  if ( 1 == names().size() ) { return s << "L0_CHANNEL('" << names().front() << "'," << bx() << ")"; }
  // ==========================================================================
  return s << "L0_CHANNEL(" << Gaudi::Utils::toString( names() ) << "," << bx() << ")";
  // ==========================================================================
}

// ============================================================================
// channel decision by channel name
// ============================================================================
LoKi::L0::ChannelPreDecision::ChannelPreDecision( const std::string& channel, const int bx )
    : LoKi::AuxFunBase( std::tie( channel, bx ) ), LoKi::L0::ChannelDecision( channel, bx ) {}
// ============================================================================
// channel decision by channel names ("OR")
// ============================================================================
LoKi::L0::ChannelPreDecision::ChannelPreDecision( const std::vector<std::string>& channels, const int bx )
    : LoKi::AuxFunBase( std::tie( channels, bx ) ), LoKi::L0::ChannelDecision( channels, bx ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::ChannelPreDecision::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( 0 != a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    // convert names into channels
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( config, "LHCb::L0DUConfig* points to NULL" );
    const LHCb::L0DUChannel::Map& m = config->channels();
    for ( const auto& name : names() ) {
      LHCb::L0DUChannel::Map::const_iterator ifind = m.find( name );
      if ( m.end() == ifind ) {
        Error( "Non-existing channel '" + name + "' has been requested" );
        continue;
      }
      const auto* channel = ifind->second;
      Assert( channel, "LHCb::L0DUChannel* points to NULL" );
      if ( channel->decisionType() != 0 ) addChannel( channel->id() );
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined channels
  return std::any_of( std::begin( channels() ), std::end( channels() ),
                      [&]( Channels::const_reference c ) { return a->channelPreDecision( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::ChannelPreDecision::fillStream( std::ostream& s ) const {
  // ==========================================================================
  if ( 1 == names().size() ) { return s << "L0_CHPREDECISION('" << names().front() << "'," << bx() << ")"; }
  // ==========================================================================
  return s << "L0_CHPREDECISION(" << Gaudi::Utils::toString( names() ) << "," << bx() << ")";
  // ==========================================================================
}

// ============================================================================
// channel decision by channel name
// ============================================================================
LoKi::L0::TriggerDecision::TriggerDecision( const std::string& channel, const int bx )
    : LoKi::AuxFunBase( std::tie( channel, bx ) ), LoKi::L0::ChannelDecision( channel, bx ) {}
// ============================================================================
// channel decision by channel names ("OR")
// ============================================================================
LoKi::L0::TriggerDecision::TriggerDecision( const std::vector<std::string>& channels, const int bx )
    : LoKi::AuxFunBase( std::tie( channels, bx ) ), LoKi::L0::ChannelDecision( channels, bx ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::TriggerDecision::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    // CHECK THE EXISTENCE OF TRIGGERS
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( config, "LHCb::L0DUConfig* points to NULL" );
    const LHCb::L0DUTrigger::Map& m = config->triggers();
    for ( const auto& name : names() ) {
      LHCb::L0DUTrigger::Map::const_iterator ifind = m.find( name );
      if ( m.end() == ifind ) {
        Error( "Unknown trigger '" + name + "' has been requested" );
        continue;
      }
      const auto* trigger = ifind->second;
      Assert( trigger, "LHCb::L0DUTrigger* points to NULL" );
      addChannel( trigger->id() );
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined triggers
  return std::any_of( std::begin( names() ), std::end( names() ),
                      [&]( Names::const_reference c ) { return a->triggerDecisionByName( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::TriggerDecision::fillStream( std::ostream& s ) const {
  // ==========================================================================
  if ( 1 == names().size() ) { return s << "L0_TRIGGER('" << names().front() << "'," << bx() << ")"; }
  // ==========================================================================
  return s << "L0_TRIGGER(" << Gaudi::Utils::toString( names() ) << "," << bx() << ")";
  // ==========================================================================
}

// ============================================================================
// channel decision by channel name
// ============================================================================
LoKi::L0::ChannelDecisionSubString::ChannelDecisionSubString( const std::string& channel, const int bx )
    : LoKi::AuxFunBase( std::tie( channel, bx ) )
    , LoKi::L0::ChannelDecision( LoKi::L0::ChannelDecision::Names(), bx )
    , m_substr( channel ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::ChannelDecisionSubString::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    clearNames();
    // convert names into channels
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( config, "LHCb::L0DUConfig* points to NULL" );
    for ( const auto& c : config->channels() ) {
      if ( std::string::npos == c.first.find( substr() ) ) { continue; }
      const auto* channel = c.second;
      Assert( channel, "LHCb::L0DUChannel* points to NULL" );
      if ( channel->decisionType() != 0 ) {
        addChannel( channel->id() );
        addName( c.first );
      }
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined channels
  return std::any_of( std::begin( channels() ), std::end( channels() ),
                      [&]( Channels::const_reference c ) { return a->channelDecision( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::ChannelDecisionSubString::fillStream( std::ostream& s ) const {
  return s << "L0_CHANNEL_SUBSTR ('" << substr() << "'," << bx() << ")";
}

// ============================================================================
// channel decision by channel name
// ============================================================================
LoKi::L0::ChannelDecisionRegex::ChannelDecisionRegex( const std::string& channel, const int bx )
    : LoKi::AuxFunBase( std::tie( channel, bx ) )
    , LoKi::L0::ChannelDecisionSubString( channel, bx )
    , m_expression( channel ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::ChannelDecisionRegex::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( 0 != a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    clearNames();
    // convert names into channels
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( config, "LHCb::L0DUConfig* points to NULL" );
    for ( const auto& c : config->channels() ) {
      if ( !boost::regex_match( c.first, expression() ) ) { continue; }
      const auto* channel = c.second;
      Assert( channel, "LHCb::L0DUChannel* points to NULL" );
      if ( channel->decisionType() != 0 ) {
        addChannel( channel->id() );
        addName( c.first );
      }
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined channels
  return std::any_of( std::begin( channels() ), std::end( channels() ),
                      [&]( Channels::const_reference c ) { return a->channelDecision( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::ChannelDecisionRegex::fillStream( std::ostream& s ) const {
  return s << "L0_CHANNEL_RE ('" << substr() << "'," << bx() << ")";
}

// ============================================================================
// channel decision by channel name
// ============================================================================
LoKi::L0::ChannelPreDecisionSubString::ChannelPreDecisionSubString( const std::string& channel, const int bx )
    : LoKi::AuxFunBase( std::tie( channel, bx ) ), LoKi::L0::ChannelDecisionSubString( channel, bx ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::ChannelPreDecisionSubString::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( 0 != a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    clearNames();
    // convert names into channels
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( 0 != config, "LHCb::L0DUConfig* points to NULL" );
    const LHCb::L0DUChannel::Map& m = config->channels();
    for ( const auto& c : m ) {
      if ( std::string::npos == c.first.find( substr() ) ) { continue; }
      const LHCb::L0DUChannel* channel = c.second;
      Assert( 0 != channel, "LHCb::L0DUChannel* points to NULL" );
      if ( channel->decisionType() != 0 ) {
        addChannel( channel->id() );
        addName( c.first );
      }
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined channels
  return std::any_of( std::begin( channels() ), std::end( channels() ),
                      [&]( Channels::const_reference c ) { return a->channelPreDecision( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::ChannelPreDecisionSubString::fillStream( std::ostream& s ) const {
  return s << "L0_CHPREDECISION_SUBSTR ('" << substr() << "'," << bx() << ")";
}

// ============================================================================
// channel decision by channel name
// ============================================================================
LoKi::L0::ChannelPreDecisionRegex::ChannelPreDecisionRegex( const std::string& channel, const int bx )
    : LoKi::AuxFunBase( std::tie( channel, bx ) ), LoKi::L0::ChannelDecisionRegex( channel, bx ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::ChannelPreDecisionRegex::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    clearNames();
    // convert names into channels
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( 0 != config, "LHCb::L0DUConfig* points to NULL" );
    for ( const auto& c : config->channels() ) {
      if ( !boost::regex_match( c.first, expression() ) ) { continue; }
      const LHCb::L0DUChannel* channel = c.second;
      Assert( 0 != channel, "LHCb::L0DUChannel* points to NULL" );
      if ( channel->decisionType() != 0 ) {
        addChannel( channel->id() );
        addName( c.first );
      }
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined channels
  return std::any_of( std::begin( channels() ), std::end( channels() ),
                      [&]( Channels::const_reference c ) { return a->channelPreDecision( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::ChannelPreDecisionRegex::fillStream( std::ostream& s ) const {
  return s << "L0_CHPREDECISION_RE ('" << substr() << "'," << bx() << ")";
}

// ============================================================================
// channel decision by channel name
// ============================================================================
LoKi::L0::TriggerDecisionSubString::TriggerDecisionSubString( const std::string& channel, const int bx )
    : LoKi::AuxFunBase( std::tie( channel, bx ) ), LoKi::L0::ChannelDecisionSubString( channel, bx ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::TriggerDecisionSubString::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    clearNames();
    // convert names into channels
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( config, "LHCb::L0DUConfig* points to NULL" );
    for ( const auto& c : config->channels() ) {
      if ( std::string::npos == c.first.find( substr() ) ) { continue; }
      const LHCb::L0DUChannel* channel = c.second;
      Assert( channel, "LHCb::L0DUChannel* points to NULL" );
      if ( channel->decisionType() != 0 ) {
        addChannel( channel->id() );
        addName( c.first );
      }
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined triggers
  return std::any_of( std::begin( names() ), std::end( names() ),
                      [&]( Names::const_reference c ) { return a->triggerDecisionByName( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::TriggerDecisionSubString::fillStream( std::ostream& s ) const {
  return s << "L0_TRIGGER_SUBSTR ('" << substr() << "'," << bx() << ")";
}

// ============================================================================
// channel decision by channel name
// ============================================================================
LoKi::L0::TriggerDecisionRegex::TriggerDecisionRegex( const std::string& channel, const int bx )
    : LoKi::AuxFunBase( std::tie( channel, bx ) ), LoKi::L0::ChannelDecisionRegex( channel, bx ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::L0::TriggerDecisionRegex::operator()( const LHCb::L0DUReport* a ) const {
  //
  Assert( a, "LHCb::L0DUReport* points to null" );
  //
  // update the system:
  if ( 0 == tckPrev() || tckPrev() != a->tck() ) {
    clearChannels();
    clearNames();
    // convert names into channels
    const LHCb::L0DUConfig* config = a->configuration();
    Assert( config, "LHCb::L0DUConfig* points to NULL" );
    for ( const auto& c : config->channels() ) {
      if ( !boost::regex_match( c.first, expression() ) ) { continue; }
      const LHCb::L0DUChannel* channel = c.second;
      Assert( channel, "LHCb::L0DUChannel* points to NULL" );
      if ( channel->decisionType() != 0 ) {
        addChannel( channel->id() );
        addName( c.first );
      }
    }
    // store the tck of successful configuration
    setTckPrev( a->tck() );
  }

  // loop over the defined triggers
  return std::any_of( std::begin( names() ), std::end( names() ),
                      [&]( Names::const_reference c ) { return a->triggerDecisionByName( c, bx() ); } );
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream& LoKi::L0::TriggerDecisionRegex::fillStream( std::ostream& s ) const {
  return s << "L0_TRIGGER_RE ('" << substr() << "'," << bx() << ")";
}

// ============================================================================
// The END
// ============================================================================

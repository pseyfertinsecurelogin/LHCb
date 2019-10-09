/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "Event/L0DUChannel.h"
#include "Event/L0DUElementaryCondition.h"
#include "Event/L0DUElementaryData.h"
#include "Event/L0DUTemplateConfig.h"
#include "Event/L0DUTrigger.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include <algorithm>
#include <numeric>
#include <ostream>
#include <string>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Namespace for locations in TDS
  namespace L0DUConfigLocation {
    inline const std::string Default  = "Trig/L0/L0DUConfig";
    inline const std::string Emulated = "Emu/L0/L0DUConfig";
  } // namespace L0DUConfigLocation

  /** @class L0DUConfig L0DUConfig.h
   *
   * This class represents the keyed L0DU Configuration
   *
   * @author Olivier Deschamps
   *
   */

  class L0DUConfig final : public KeyedObject<unsigned int> {
  public:
    /// typedef for KeyedContainer of L0DUConfig
    typedef KeyedContainer<L0DUConfig, Containers::HashMap> Container;

    /// Default constructor
    L0DUConfig() : Base( 0x0000 ), m_completed( false ) {}

    /// non-default constructor
    L0DUConfig( unsigned int tck ) : Base( tck ), m_completed( false ) {}

    /// Copy Constructor
    L0DUConfig( const LHCb::L0DUConfig& rh )
        : Base( rh.key() )
        , m_recipe( rh.recipe() )
        , m_definition( rh.definition() )
        , m_data( rh.data() )
        , m_conditions( rh.conditions() )
        , m_channels( rh.channels() )
        , m_triggers( rh.triggers() )
        , m_completed( rh.completed() ) {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// return Trigger Configuration Key @alias of key()
    unsigned int tck() const;

    /// delete MAPs content
    void release();

    /// L0DUConfig  short description
    std::string description() const;

    /// L0DUConfig full summary after processing
    std::string summary();

    /// Count the number channels with a given decisionType (default ANY)
    int numberOfChannels( int mask = L0DUDecision::Any ) const;

    /// Count the number trigger with a given decisionType (default ANY)
    int numberOfTriggers( int mask = L0DUDecision::Any ) const;

    /// enable/disable all the channels downscaling counters
    void updateCounters( bool update = true );

    /// reset all channel counters
    void resetChannelCounters();

    /// increment all channels counter
    void incrementCounters();

    /// decrement all channels counter
    void decrementCounters();

    /// reset all channels emulation flags
    void resetEmulated( bool value = false );

    /// return true when all channels are emulated
    bool emulated() const;

    /// clear all ElementaryData operands
    void clearDataValue();

    /// emulate all channels
    L0DUConfig* emulate();

    /// global emulated decision (incl. downscaling) for a given decision
    bool emulatedDecision( int mask = L0DUDecision::Physics ) const;

    /// global emulated decision (incl. downscaling)
    int emulatedDecisionValue() const;

    /// global emulated decision (without downscaling)
    bool emulatedPreDecision( int mask = L0DUDecision::Physics ) const;

    /// global emulated decision (without downscaling)
    int emulatedPreDecisionValue() const;

    /// return the downscaling decision as (emulatedDecision == emulatedPreDecision)
    bool isDownscaled() const;

    /// return the  trigger(s) the channel belongs to
    std::vector<std::string> triggerByChannelName( const std::string& name ) const;

    /// return the  elementaryCondition from channel and data names
    const LHCb::L0DUElementaryCondition* elementaryCondition( const std::string& channelName,
                                                              const std::string& dataName );

    /// Retrieve const   L0DU Channel recipe name
    const std::string& recipe() const;

    /// Update   L0DU Channel recipe name
    void setRecipe( const std::string& value );

    /// Retrieve const   L0DU Channel key definition
    const std::string& definition() const;

    /// Update   L0DU Channel key definition
    void setDefinition( const std::string& value );

    /// Retrieve const   L0DU Elementary condition
    const LHCb::L0DUElementaryData::Map& data() const;

    /// Retrieve   L0DU Elementary condition
    LHCb::L0DUElementaryData::Map& data();

    /// Update   L0DU Elementary condition
    void setData( const LHCb::L0DUElementaryData::Map& value );

    /// Retrieve const   L0DU Elementary condition
    const LHCb::L0DUElementaryCondition::Map& conditions() const;

    /// Retrieve   L0DU Elementary condition
    LHCb::L0DUElementaryCondition::Map& conditions();

    /// Update   L0DU Elementary condition
    void setConditions( const LHCb::L0DUElementaryCondition::Map& value );

    /// Retrieve const   L0DU channels
    const LHCb::L0DUChannel::Map& channels() const;

    /// Retrieve   L0DU channels
    LHCb::L0DUChannel::Map& channels();

    /// Update   L0DU channels
    void setChannels( const LHCb::L0DUChannel::Map& value );

    /// Retrieve const  Set of L0DU channels
    const LHCb::L0DUTrigger::Map& triggers() const;

    /// Retrieve  Set of L0DU channels
    LHCb::L0DUTrigger::Map& triggers();

    /// Update  Set of L0DU channels
    void setTriggers( const LHCb::L0DUTrigger::Map& value );

    /// Retrieve const  Complete Configuration (i.e. contain Processor Data Value -> ready for emulation)
    bool completed() const;

    /// Update  Complete Configuration (i.e. contain Processor Data Value -> ready for emulation)
    void setCompleted( bool value );

    /// Retrieve const  Muon FOI x
    const std::vector<int>& muonFOIx() const;

    /// Update  Muon FOI x
    void setMuonFOIx( const std::vector<int>& value );

    /// Retrieve const  Muon FOI y
    const std::vector<int>& muonFOIy() const;

    /// Update  Muon FOI y
    void setMuonFOIy( const std::vector<int>& value );

    friend std::ostream& operator<<( std::ostream& str, const L0DUConfig& obj ) { return obj.fillStream( str ); }

  protected:
    /// L0DU Trigger Configuration Key
    typedef KeyedObject<unsigned int> Base;

  private:
    std::string                        m_recipe;     ///<  L0DU Channel recipe name
    std::string                        m_definition; ///<  L0DU Channel key definition
    LHCb::L0DUElementaryData::Map      m_data;       ///<  L0DU Elementary condition
    LHCb::L0DUElementaryCondition::Map m_conditions; ///<  L0DU Elementary condition
    LHCb::L0DUChannel::Map             m_channels;   ///<  L0DU channels
    LHCb::L0DUTrigger::Map             m_triggers;   ///< Set of L0DU channels
    bool             m_completed; ///< Complete Configuration (i.e. contain Processor Data Value -> ready for emulation)
    std::vector<int> m_muonFOIx;  ///< Muon FOI x
    std::vector<int> m_muonFOIy;  ///< Muon FOI y

  }; // class L0DUConfig

  /// Definition of Keyed Container for L0DUConfig
  typedef KeyedContainer<L0DUConfig, Containers::HashMap> L0DUConfigs;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::L0DUConfig::fillStream( std::ostream& s ) const {
  char l_completed = ( m_completed ) ? 'T' : 'F';
  s << "{ "
    << "recipe :	" << m_recipe << std::endl
    << "definition :	" << m_definition << std::endl
    << "data :	" << m_data << std::endl
    << "conditions :	" << m_conditions << std::endl
    << "channels :	" << m_channels << std::endl
    << "triggers :	" << m_triggers << std::endl
    << "completed :	" << l_completed << std::endl
    << "muonFOIx :	" << m_muonFOIx << std::endl
    << "muonFOIy :	" << m_muonFOIy << std::endl
    << " }";
  return s;
}

inline const std::string& LHCb::L0DUConfig::recipe() const { return m_recipe; }

inline void LHCb::L0DUConfig::setRecipe( const std::string& value ) { m_recipe = value; }

inline const std::string& LHCb::L0DUConfig::definition() const { return m_definition; }

inline void LHCb::L0DUConfig::setDefinition( const std::string& value ) { m_definition = value; }

inline const LHCb::L0DUElementaryData::Map& LHCb::L0DUConfig::data() const { return m_data; }

inline LHCb::L0DUElementaryData::Map& LHCb::L0DUConfig::data() { return m_data; }

inline void LHCb::L0DUConfig::setData( const LHCb::L0DUElementaryData::Map& value ) { m_data = value; }

inline const LHCb::L0DUElementaryCondition::Map& LHCb::L0DUConfig::conditions() const { return m_conditions; }

inline LHCb::L0DUElementaryCondition::Map& LHCb::L0DUConfig::conditions() { return m_conditions; }

inline void LHCb::L0DUConfig::setConditions( const LHCb::L0DUElementaryCondition::Map& value ) { m_conditions = value; }

inline const LHCb::L0DUChannel::Map& LHCb::L0DUConfig::channels() const { return m_channels; }

inline LHCb::L0DUChannel::Map& LHCb::L0DUConfig::channels() { return m_channels; }

inline void LHCb::L0DUConfig::setChannels( const LHCb::L0DUChannel::Map& value ) { m_channels = value; }

inline const LHCb::L0DUTrigger::Map& LHCb::L0DUConfig::triggers() const { return m_triggers; }

inline LHCb::L0DUTrigger::Map& LHCb::L0DUConfig::triggers() { return m_triggers; }

inline void LHCb::L0DUConfig::setTriggers( const LHCb::L0DUTrigger::Map& value ) { m_triggers = value; }

inline bool LHCb::L0DUConfig::completed() const { return m_completed; }

inline void LHCb::L0DUConfig::setCompleted( bool value ) { m_completed = value; }

inline const std::vector<int>& LHCb::L0DUConfig::muonFOIx() const { return m_muonFOIx; }

inline void LHCb::L0DUConfig::setMuonFOIx( const std::vector<int>& value ) { m_muonFOIx = value; }

inline const std::vector<int>& LHCb::L0DUConfig::muonFOIy() const { return m_muonFOIy; }

inline void LHCb::L0DUConfig::setMuonFOIy( const std::vector<int>& value ) { m_muonFOIy = value; }

inline unsigned int LHCb::L0DUConfig::tck() const { return key(); }

inline void LHCb::L0DUConfig::release() {

  for ( auto& i : m_triggers ) delete i.second;
  m_triggers.clear();
  for ( auto& i : m_channels ) delete i.second;
  m_channels.clear();
  for ( auto& i : m_conditions ) delete i.second;
  m_conditions.clear();
  for ( auto& i : m_data ) delete i.second;
  m_data.clear();
}

inline int LHCb::L0DUConfig::numberOfChannels( int mask ) const {

  return std::count_if( m_channels.begin(), m_channels.end(), [&]( LHCb::L0DUChannel::Map::const_reference c ) {
    const auto& channel = c.second;
    if ( mask == 0 && channel->decisionType() == 0 )
      return true;
    else if ( ( channel->decisionType() & mask ) == 0 )
      return false;
    else
      return true;
  } );
}

inline int LHCb::L0DUConfig::numberOfTriggers( int mask ) const {

  return std::count_if( m_triggers.begin(), m_triggers.end(), [&]( LHCb::L0DUTrigger::Map::const_reference i ) {
    const auto& trigger = i.second;
    if ( mask == 0 && trigger->decisionType() == 0 )
      return true;
    else if ( ( trigger->decisionType() & mask ) == 0 )
      return false;
    else
      return true;
  } );
}

inline void LHCb::L0DUConfig::updateCounters( bool update ) {

  for ( auto& i : m_channels ) i.second->setUpdateCounter( update );
}

inline void LHCb::L0DUConfig::resetChannelCounters() {

  for ( auto& i : m_channels ) i.second->setCounter( 0 );
}

inline void LHCb::L0DUConfig::incrementCounters() {

  for ( auto& c : m_channels ) {
    unsigned int count = c.second->counter() + c.second->rate();
    if ( count >= LHCb::L0DUCounter::Scale ) count -= LHCb::L0DUCounter::Scale;
    c.second->setCounter( count );
  }
}

inline void LHCb::L0DUConfig::decrementCounters() {

  for ( auto& i : m_channels ) {
    int count = i.second->counter() - i.second->rate();
    if ( count < 0 ) count += LHCb::L0DUCounter::Scale;
    i.second->setCounter( count );
  }
}

inline void LHCb::L0DUConfig::resetEmulated( bool value ) {

  for ( auto& i : m_channels ) i.second->setEmulated( value );
}

inline bool LHCb::L0DUConfig::emulated() const {

  return std::all_of( m_channels.begin(), m_channels.end(), []( const auto& i ) { return i.second->emulated(); } );
}

inline void LHCb::L0DUConfig::clearDataValue() {

  std::for_each( m_data.begin(), m_data.end(), []( const auto& i ) {
    if ( i.second->type() != L0DUElementaryData::Type::Constant ) i.second->clear();
  } );
  resetEmulated();
}

inline LHCb::L0DUConfig* LHCb::L0DUConfig::emulate() {

  for ( auto& i : m_channels ) i.second->emulate();
  return this;
}

inline bool LHCb::L0DUConfig::emulatedDecision( int mask ) const { return ( emulatedDecisionValue() & mask ) != 0; }

inline int LHCb::L0DUConfig::emulatedDecisionValue() const {

  return std::accumulate( m_channels.begin(), m_channels.end(), 0,
                          [&]( int value, LHCb::L0DUChannel::Map::const_reference m ) {
                            LHCb::L0DUChannel* channel = m.second;
                            if ( channel->emulatedDecision() ) value |= channel->decisionType();
                            return value;
                          } );
}

inline bool LHCb::L0DUConfig::emulatedPreDecision( int mask ) const {

  return ( emulatedPreDecisionValue() & mask ) != 0;
}

inline int LHCb::L0DUConfig::emulatedPreDecisionValue() const {

  return std::accumulate( m_channels.begin(), m_channels.end(), 0,
                          [&]( int value, LHCb::L0DUChannel::Map::const_reference m ) {
                            LHCb::L0DUChannel* channel = m.second;
                            if ( channel->emulatedPreDecision() ) value |= channel->decisionType();
                            return value;
                          } );
}

inline bool LHCb::L0DUConfig::isDownscaled() const { return ( emulatedPreDecisionValue() == emulatedDecisionValue() ); }

inline std::vector<std::string> LHCb::L0DUConfig::triggerByChannelName( const std::string& name ) const {

  std::vector<std::string> list;
  for ( const auto& i : m_triggers ) {
    const auto& channels = i.second->channels();
    if ( std::any_of( channels.begin(), channels.end(),
                      [&]( LHCb::L0DUChannel::Map::const_reference m ) { return m.second->name() == name; } ) ) {
      list.push_back( i.first );
    }
  }
  return list;
}

inline const LHCb::L0DUElementaryCondition* LHCb::L0DUConfig::elementaryCondition( const std::string& channelName,
                                                                                   const std::string& dataName ) {

  const LHCb::L0DUChannel* channel = channels()[channelName];
  if ( !channel ) return nullptr;
  const LHCb::L0DUElementaryCondition::Map& conds = channel->elementaryConditions();
  auto ic = std::find_if( conds.begin(), conds.end(), [&]( LHCb::L0DUElementaryCondition::Map::const_reference c ) {
    const auto& condition = c.second;
    if ( !condition ) return false;
    const LHCb::L0DUElementaryData* data = condition->data();
    return data && data->name() == dataName;
  } );
  return ic != conds.end() ? ic->second : nullptr;
}

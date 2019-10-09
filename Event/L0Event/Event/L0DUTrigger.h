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
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SerializeSTL.h"
#include <map>
#include <ostream>
#include <string>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Namespace for locations in TDS
  namespace L0DUTriggerLocation {
    inline const std::string Default  = "Trig/L0/L0DUTriggers";
    inline const std::string Emulated = "Emu/L0/L0DUTriggers";
  } // namespace L0DUTriggerLocation

  /** @class L0DUTrigger L0DUTrigger.h
   *
   * This class represents individual L0DU Trigger (i.e. L0DUChannel set)
   *
   * @author Olivier Deschamps
   *
   */

  class L0DUTrigger final : public ContainedObject {
  public:
    /// typedef for ObjectVector of L0DUTrigger
    typedef ObjectVector<L0DUTrigger> Container;

    /// std::map Container
    typedef std::map<std::string, L0DUTrigger*> Map;

    /// non-default constructor
    L0DUTrigger( int id, const std::string& name, int type = L0DUDecision::Physics )
        : m_id( id ), m_name( name ), m_decisionType( type ) {}

    /// default constructor
    L0DUTrigger() : m_id( 0 ), m_name(), m_decisionType( L0DUDecision::Physics ) {}

    /// Copy Constructor
    L0DUTrigger( const L0DUTrigger& rh );

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// get the trigger emulated decision
    bool emulatedDecision() const;

    /// push_back L0DU channel
    void addChannel( LHCb::L0DUChannel* channel );

    /// Clear channels
    void resetChannels();

    /// clone this trigger
    virtual L0DUTrigger* clone();

    /// L0DUTrigger short description
    std::string description();

    /// L0DUTrigger summary
    std::string summary();

    /// Retrieve const   L0DU SubTrigger index
    unsigned int id() const;

    /// Update   L0DU SubTrigger index
    void setId( unsigned int value );

    /// Retrieve const   L0DU SubTrigger name
    const std::string& name() const;

    /// Update   L0DU SubTrigger name
    void setName( const std::string& value );

    /// Retrieve const   L0DU Channels
    const LHCb::L0DUChannel::Map& channels() const;

    /// Retrieve   L0DU Channels
    LHCb::L0DUChannel::Map& channels();

    /// Update   L0DU Channels
    void setChannels( const LHCb::L0DUChannel::Map& value );

    /// Retrieve const   Associated decision type
    int decisionType() const;

    /// Update   Associated decision type
    void setDecisionType( int value );

    friend std::ostream& operator<<( std::ostream& str, const L0DUTrigger& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned int           m_id;           ///<  L0DU SubTrigger index
    std::string            m_name;         ///<  L0DU SubTrigger name
    LHCb::L0DUChannel::Map m_channels;     ///<  L0DU Channels
    int                    m_decisionType; ///<  Associated decision type

  }; // class L0DUTrigger

  /// Definition of vector container type for L0DUTrigger
  typedef ObjectVector<L0DUTrigger> L0DUTriggers;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::L0DUTrigger::L0DUTrigger( const LHCb::L0DUTrigger& rh )
    : ContainedObject( rh )
    , m_id( rh.m_id )
    , m_name( rh.m_name )
    , m_channels( rh.m_channels )
    , m_decisionType( rh.m_decisionType ) {}

inline std::ostream& LHCb::L0DUTrigger::fillStream( std::ostream& s ) const {
  s << "{ "
    << "id :	" << m_id << std::endl
    << "name :	" << m_name << std::endl
    << "channels :	" << m_channels << std::endl
    << "decisionType :	" << m_decisionType << std::endl
    << " }";
  return s;
}

inline unsigned int LHCb::L0DUTrigger::id() const { return m_id; }

inline void LHCb::L0DUTrigger::setId( unsigned int value ) { m_id = value; }

inline const std::string& LHCb::L0DUTrigger::name() const { return m_name; }

inline void LHCb::L0DUTrigger::setName( const std::string& value ) { m_name = value; }

inline const LHCb::L0DUChannel::Map& LHCb::L0DUTrigger::channels() const { return m_channels; }

inline LHCb::L0DUChannel::Map& LHCb::L0DUTrigger::channels() { return m_channels; }

inline void LHCb::L0DUTrigger::setChannels( const LHCb::L0DUChannel::Map& value ) { m_channels = value; }

inline int LHCb::L0DUTrigger::decisionType() const { return m_decisionType; }

inline void LHCb::L0DUTrigger::setDecisionType( int value ) { m_decisionType = value; }

inline bool LHCb::L0DUTrigger::emulatedDecision() const {

  for ( LHCb::L0DUChannel::Map::const_iterator ic = m_channels.begin(); ic != m_channels.end(); ic++ ) {
    if ( ( ( *ic ).second->decisionType() & decisionType() ) == 0 ) continue;
    if ( ( *ic ).second->emulatedDecision() ) return true;
  }
  return false;
}

inline void LHCb::L0DUTrigger::addChannel( LHCb::L0DUChannel* channel ) { m_channels[channel->name()] = channel; }

inline void LHCb::L0DUTrigger::resetChannels() { m_channels.clear(); }

inline LHCb::L0DUTrigger* LHCb::L0DUTrigger::clone() {

  L0DUTrigger* trigger = new L0DUTrigger( *this );
  return trigger;
}

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
#include "Event/L0DUCounter.h"
#include "Event/L0DUDecision.h"
#include "Event/L0DUElementaryCondition.h"
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
  namespace L0DUChannelLocation {
    inline const std::string Default  = "Trig/L0/L0DUChannels";
    inline const std::string Emulated = "Emu/L0/L0DUChannels";
  } // namespace L0DUChannelLocation

  /** @class L0DUChannel L0DUChannel.h
   *
   * This class represents individual L0DU Channel
   *
   * @author Olivier Deschamps
   *
   */

  class L0DUChannel final : public ContainedObject {
  public:
    /// typedef for ObjectVector of L0DUChannel
    typedef ObjectVector<L0DUChannel> Container;

    /// counter type
    typedef unsigned int count;
    /// std::map Container
    typedef std::map<std::string, L0DUChannel*> Map;

    /// non-default constructor
    L0DUChannel( int id, const std::string& name, const count& rate, int decisionType = L0DUDecision::Physics )
        : m_id( id )
        , m_name( name )
        , m_rate( rate )
        , m_decisionType( decisionType )
        , m_counter( 0 )
        , m_emulatedDecision( false )
        , m_emulatedPreDecision( false )
        , m_emulated( false )
        , m_updateCounter( true )
        , m_accept( false ) {}

    /// default constructor
    L0DUChannel()
        : m_id( 0 )
        , m_name()
        , m_rate( 100 )
        , m_decisionType( L0DUDecision::Physics )
        , m_counter( 0 )
        , m_emulatedDecision( false )
        , m_emulatedPreDecision( false )
        , m_emulated( false )
        , m_updateCounter( true )
        , m_accept( false ) {}

    /// Copy Constructor
    L0DUChannel( const L0DUChannel& rh );

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// does the channel participate to the global OR'ed decision (for a given decision type - default is Any)
    bool enable( int mask = L0DUDecision::Any ) const;

    /// get the channel emulated decision (incl. downscaling decision)
    bool emulatedDecision() const;

    /// get the channel emulated trigger value (NOT applying downscaling)
    bool emulatedPreDecision() const;

    /// return the downscaling decision as (emulatedDecision == emulatedPreDecision)
    bool isDownscaled() const;

    /// push_back Elementary Condition
    void addElementaryCondition( LHCb::L0DUElementaryCondition* condition );

    /// Clear Elementary Conditions
    void resetElementaryConditions();

    /// clone this channel
    virtual L0DUChannel* clone();

    /// Process Channel emulated Decision
    L0DUChannel* emulate();

    /// L0DUChannel short description
    std::string description() const;

    /// L0DUChannel summary
    std::string summary() const;

    /// Retrieve const   L0DU Channel index
    unsigned int id() const;

    /// Update   L0DU Channel index
    void setId( unsigned int value );

    /// Retrieve const   L0DU Channel name
    const std::string& name() const;

    /// Update   L0DU Channel name
    void setName( const std::string& value );

    /// Retrieve const  Accept rate (%)
    unsigned int rate() const;

    /// Update  Accept rate (%)
    void setRate( unsigned int value );

    /// Retrieve const  decision type
    int decisionType() const;

    /// Update  decision type
    void setDecisionType( int value );

    /// Retrieve const   L0DU Elementary condition
    const LHCb::L0DUElementaryCondition::Map& elementaryConditions() const;

    /// Update   L0DU Elementary condition
    void setElementaryConditions( const LHCb::L0DUElementaryCondition::Map& value );

    /// Retrieve const   counter value
    unsigned int counter() const;

    /// Update   counter value
    void setCounter( unsigned int value );

    /// Retrieve const  L0DU Channel emulation flag
    bool emulated() const;

    /// Update  L0DU Channel emulation flag
    void setEmulated( bool value );

    /// Retrieve const  enable/disable the downscaling counter
    bool updateCounter() const;

    /// Update  enable/disable the downscaling counter
    void setUpdateCounter( bool value );

    /// Retrieve const  downscaling decision
    bool accept() const;

    friend std::ostream& operator<<( std::ostream& str, const L0DUChannel& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned int                       m_id;                   ///<  L0DU Channel index
    std::string                        m_name;                 ///<  L0DU Channel name
    unsigned int                       m_rate;                 ///< Accept rate (%)
    int                                m_decisionType;         ///< decision type
    LHCb::L0DUElementaryCondition::Map m_elementaryConditions; ///<  L0DU Elementary condition
    unsigned int                       m_counter;              ///<  counter value
    bool m_emulatedDecision;    ///< L0DU Channel emulated Decision (incl. downscaling decision)
    bool m_emulatedPreDecision; ///< L0DU Channel emulated trigger value (NOT applying downscaling)
    bool m_emulated;            ///< L0DU Channel emulation flag
    bool m_updateCounter;       ///< enable/disable the downscaling counter
    bool m_accept;              ///< downscaling decision

  }; // class L0DUChannel

  /// Definition of vector container type for L0DUChannel
  typedef ObjectVector<L0DUChannel> L0DUChannels;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::L0DUChannel::L0DUChannel( const LHCb::L0DUChannel& rh )
    : ContainedObject( rh )
    , m_id( rh.m_id )
    , m_name( rh.m_name )
    , m_rate( rh.m_rate )
    , m_decisionType( rh.m_decisionType )
    , m_elementaryConditions( rh.m_elementaryConditions )
    , m_counter( rh.m_counter )
    , m_emulatedDecision( rh.m_emulatedDecision )
    , m_emulatedPreDecision( rh.m_emulatedPreDecision )
    , m_emulated( rh.m_emulated )
    , m_updateCounter( rh.m_updateCounter )
    , m_accept( rh.m_accept ) {}

inline std::ostream& LHCb::L0DUChannel::fillStream( std::ostream& s ) const {
  char l_emulatedDecision    = ( m_emulatedDecision ) ? 'T' : 'F';
  char l_emulatedPreDecision = ( m_emulatedPreDecision ) ? 'T' : 'F';
  char l_emulated            = ( m_emulated ) ? 'T' : 'F';
  char l_updateCounter       = ( m_updateCounter ) ? 'T' : 'F';
  char l_accept              = ( m_accept ) ? 'T' : 'F';
  s << "{ "
    << "id :	" << m_id << std::endl
    << "name :	" << m_name << std::endl
    << "rate :	" << m_rate << std::endl
    << "decisionType :	" << m_decisionType << std::endl
    << "elementaryConditions :	" << m_elementaryConditions << std::endl
    << "counter :	" << m_counter << std::endl
    << "emulatedDecision :	" << l_emulatedDecision << std::endl
    << "emulatedPreDecision :	" << l_emulatedPreDecision << std::endl
    << "emulated :	" << l_emulated << std::endl
    << "updateCounter :	" << l_updateCounter << std::endl
    << "accept :	" << l_accept << std::endl
    << " }";
  return s;
}

inline unsigned int LHCb::L0DUChannel::id() const { return m_id; }

inline void LHCb::L0DUChannel::setId( unsigned int value ) { m_id = value; }

inline const std::string& LHCb::L0DUChannel::name() const { return m_name; }

inline void LHCb::L0DUChannel::setName( const std::string& value ) { m_name = value; }

inline unsigned int LHCb::L0DUChannel::rate() const { return m_rate; }

inline void LHCb::L0DUChannel::setRate( unsigned int value ) { m_rate = value; }

inline int LHCb::L0DUChannel::decisionType() const { return m_decisionType; }

inline void LHCb::L0DUChannel::setDecisionType( int value ) { m_decisionType = value; }

inline const LHCb::L0DUElementaryCondition::Map& LHCb::L0DUChannel::elementaryConditions() const {
  return m_elementaryConditions;
}

inline void LHCb::L0DUChannel::setElementaryConditions( const LHCb::L0DUElementaryCondition::Map& value ) {
  m_elementaryConditions = value;
}

inline unsigned int LHCb::L0DUChannel::counter() const { return m_counter; }

inline void LHCb::L0DUChannel::setCounter( unsigned int value ) { m_counter = value; }

inline bool LHCb::L0DUChannel::emulated() const { return m_emulated; }

inline void LHCb::L0DUChannel::setEmulated( bool value ) { m_emulated = value; }

inline bool LHCb::L0DUChannel::updateCounter() const { return m_updateCounter; }

inline void LHCb::L0DUChannel::setUpdateCounter( bool value ) { m_updateCounter = value; }

inline bool LHCb::L0DUChannel::accept() const { return m_accept; }

inline bool LHCb::L0DUChannel::enable( int mask ) const {

  return mask == L0DUDecision::Disable ? m_decisionType == L0DUDecision::Disable : ( m_decisionType & mask );
}

inline bool LHCb::L0DUChannel::emulatedDecision() const {

  return const_cast<L0DUChannel*>( this )->emulate()->m_emulatedDecision;
}

inline bool LHCb::L0DUChannel::emulatedPreDecision() const {

  return const_cast<L0DUChannel*>( this )->emulate()->m_emulatedPreDecision;
}

inline bool LHCb::L0DUChannel::isDownscaled() const { return emulatedDecision() == emulatedPreDecision(); }

inline void LHCb::L0DUChannel::addElementaryCondition( LHCb::L0DUElementaryCondition* condition ) {
  m_elementaryConditions[condition->name()] = condition;
}

inline void LHCb::L0DUChannel::resetElementaryConditions() { m_elementaryConditions.clear(); }

inline LHCb::L0DUChannel* LHCb::L0DUChannel::clone() { return new L0DUChannel( *this ); }

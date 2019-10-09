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
#include "Event/L0DUConfig.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/SmartRef.h"
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_L0DUReport = 6004;

  // Namespace for locations in TDS
  namespace L0DUReportLocation {
    inline const std::string Default  = "Trig/L0/L0DUReport";
    inline const std::string Emulated = "Emu/L0/L0DUReport";
  } // namespace L0DUReportLocation

  /** @class L0DUReport L0DUReport.h
   *
   * L0DU summary class
   *
   * @author Olivier Deschamps
   *
   */

  class L0DUReport final : public DataObject {
  public:
    /// Default constructor
    L0DUReport();

    /// Non-default constructor
    L0DUReport( const SmartRef<LHCb::L0DUConfig>& config );

    /// Copy Constructor
    L0DUReport( const L0DUReport& rh );

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// clear the summaries MAPs
    void clear();

    /// decision for a given type
    bool decision( int mask = L0DUDecision::Physics ) const;

    /// add decision for a given type
    void addDecisionValue( int decision );

    /// set the configuration
    void setConfiguration( const SmartRef<LHCb::L0DUConfig>& config );

    /// set the sumEt value for a given BX
    void setSumEt( int value, int bx = 0 );

    /// get the sumEt value for a given BX
    int sumEt( int bx = 0 ) const;

    /// return the data evaluation for a given bx
    bool hasData( const std::string& name, int bx = 0 ) const;

    /// return L0DU elementary data value (digit) from m_configuration->data()
    unsigned int dataDigit( const std::string& name, int bx = 0 ) const;

    /// return L0DU elementary data value (converted) from m_configuration->data()
    double dataValue( const std::string& name, int bx = 0 ) const;

    /// add value/scale pair to the embedded data map
    void addToData( const std::string& name, int value, double scale );

    /// add a 32-bit L0DUChannels firing summary (FCS)
    void setChannelsPreDecisionSummary( const unsigned int summary, unsigned int loc = 0, int bx = 0 );

    /// add a 32-bit L0DUChannels decision summary (TCS) [== FCS + downscaling decision]
    void setChannelsDecisionSummary( const unsigned int summary, unsigned int loc = 0, int bx = 0 );

    /// add a 32-bit Conditions summary
    void setConditionsValueSummary( const unsigned int summary, unsigned int loc = 0, int bx = 0 );

    /// add a channel pre-decision bit in the FCS summary
    void setChannelPreDecision( const std::string& name, bool triggered, int bx = 0 );

    /// add a decision bit in the TCS
    void setChannelDecision( const std::string& name, bool decision, int bx = 0 );

    /// add a onditions bit in the ECS
    void setConditionValue( const std::string& name, bool decision, int bx = 0 );

    /// 32-bit summary for the channels decision
    unsigned int channelsDecisionSummary( unsigned int index = 0, int bx = 0 ) const;

    /// 32-bit summary for the channels pre-decision
    unsigned int channelsPreDecisionSummary( unsigned int index = 0, int bx = 0 ) const;

    /// 32-bit summary for the elementary conditions
    unsigned int conditionsValueSummary( unsigned int index = 0, int bx = 0 ) const;

    /// return the channel decision by channel number (incl. the downscaling decision)
    bool channelDecision( const unsigned int channel, int bx = 0 ) const;

    /// return the channel decision by channel number (NOT incl. the downscaling decision)
    bool channelPreDecision( const unsigned int channel, int bx = 0 ) const;

    /// return the condition value by condition number
    bool conditionValue( const unsigned int condition, int bx = 0 ) const;

    /// return channel firing by channel name
    bool channelPreDecisionByName( const std::string& name, int bx = 0 ) const;

    /// return channel decision by channel name
    bool channelDecisionByName( const std::string& name, int bx = 0 ) const;

    /// return condition value by condition name
    bool conditionValueByName( const std::string& name, int bx = 0 ) const;

    /// return channel name by index
    std::string channelName( const unsigned int channel ) const;

    /// return condition name by index
    std::string conditionName( const unsigned int cond ) const;

    /// return the L0-channel decision participating to the global decision (for a given decision type)
    bool globalDecisionFromChannel( const std::string& name, int type = L0DUDecision::Physics ) const;

    /// return the L0 decision (for a given type) evaluated from the ChannelsDecisionSummaries
    bool decisionFromSummary( int type = L0DUDecision::Physics, int bx = 0 ) const;

    /// return the L0 predDecision (for a given type) evaluated from the ChannelsDecisionSummaries
    bool preDecisionFromSummary( int type = L0DUDecision::Physics, int bx = 0 ) const;

    /// return the L0 predDecision value evaluated from the ChannelsDecisionSummaries
    int preDecisionValueFromSummary( int bx = 0 ) const;

    /// return the L0 decision for any set of channels as defined in the configuration
    bool triggerDecisionByName( const std::string& name, int bx = 0 ) const;

    /// return the L0 decision for any set of channels as defined in the configuration
    bool triggerDecision( unsigned int index, int bx = 0 ) const;

    /// Retrieve const   global decision value
    int decisionValue() const;

    /// Update   global decision value
    void setDecisionValue( int value );

    /// Retrieve const   Timing Trigger Decision
    bool timingTriggerBit() const;

    /// Update   Timing Trigger Decision
    void setTimingTriggerBit( bool value );

    /// Retrieve const   Force Bit
    bool forceBit() const;

    /// Update   Force Bit
    void setForceBit( bool value );

    /// Retrieve const   L0DU algorithm configuration Key
    unsigned int tck() const;

    /// Update   L0DU algorithm configuration Key
    void setTck( unsigned int value );

    /// Retrieve const  'channels pre-decision' Summaries   (FCS)
    const std::map<std::pair<int, unsigned int>, unsigned int>& channelsPreDecisionSummaries() const;

    /// Update  'channels pre-decision' Summaries   (FCS)
    void setChannelsPreDecisionSummaries( const std::map<std::pair<int, unsigned int>, unsigned int>& value );

    /// Retrieve const  'Channel decisions' Summaries  (TCS)
    const std::map<std::pair<int, unsigned int>, unsigned int>& channelsDecisionSummaries() const;

    /// Update  'Channel decisions' Summaries  (TCS)
    void setChannelsDecisionSummaries( const std::map<std::pair<int, unsigned int>, unsigned int>& value );

    /// Retrieve const  'Elementary conditions' Summaries (ECS)
    const std::map<std::pair<int, unsigned int>, unsigned int>& conditionsValueSummaries() const;

    /// Update  'Elementary conditions' Summaries (ECS)
    void setConditionsValueSummaries( const std::map<std::pair<int, unsigned int>, unsigned int>& value );

    /// Update   sum(Et) value over several BX
    void setSumEt( const std::map<int, unsigned int>& value );

    /// Retrieve const   validity of the report
    bool valid() const;

    /// Update   validity of the report
    void setValid( bool value );

    /// Retrieve const   L0DU bank version number
    int bankVersion() const;

    /// Update   L0DU bank version number
    void setBankVersion( int value );

    /// Retrieve const   embeded data map
    const std::map<std::string, std::pair<int, double>>& dataMap() const;

    /// Update   embeded data map
    void setDataMap( const std::map<std::string, std::pair<int, double>>& value );

    /// Retrieve const   L0DU BCID
    int bcid() const;

    /// Update   L0DU BCID
    void setBcid( int value );

    /// Retrieve (const)  Reference to possible L0DUConfig
    const LHCb::L0DUConfig* configuration() const;

    /// Retrieve  Reference to possible L0DUConfig
    LHCb::L0DUConfig* configuration();

    friend std::ostream& operator<<( std::ostream& str, const L0DUReport& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    int                                                  m_decisionValue;    ///<  global decision value
    bool                                                 m_timingTriggerBit; ///<  Timing Trigger Decision
    bool                                                 m_forceBit;         ///<  Force Bit
    unsigned int                                         m_tck;              ///<  L0DU algorithm configuration Key
    std::map<std::pair<int, unsigned int>, unsigned int> m_channelsPreDecisionSummaries; ///< 'channels pre-decision'
                                                                                         ///< Summaries   (FCS)
    std::map<std::pair<int, unsigned int>, unsigned int> m_channelsDecisionSummaries; ///< 'Channel decisions' Summaries
                                                                                      ///< (TCS)
    std::map<std::pair<int, unsigned int>, unsigned int> m_conditionsValueSummaries;  ///< 'Elementary conditions'
                                                                                      ///< Summaries (ECS)
    std::map<int, unsigned int>                   m_sumEt;         ///<  sum(Et) value over several BX
    bool                                          m_valid;         ///<  validity of the report
    int                                           m_bankVersion;   ///<  L0DU bank version number
    std::map<std::string, std::pair<int, double>> m_dataMap;       ///<  embeded data map
    int                                           m_bcid;          ///<  L0DU BCID
    SmartRef<LHCb::L0DUConfig>                    m_configuration; ///< Reference to possible L0DUConfig

  }; // class L0DUReport

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::L0DUReport::L0DUReport() : m_tck( 0x0000 ), m_configuration() { clear(); }

inline LHCb::L0DUReport::L0DUReport( const SmartRef<LHCb::L0DUConfig>& config ) : m_configuration( config ) {

  m_tck = config->key();
  clear();
}

inline LHCb::L0DUReport::L0DUReport( const LHCb::L0DUReport& rh )
    : DataObject( rh )
    , m_decisionValue( rh.m_decisionValue )
    , m_timingTriggerBit( rh.m_timingTriggerBit )
    , m_forceBit( rh.m_forceBit )
    , m_tck( rh.m_tck )
    , m_channelsPreDecisionSummaries( rh.m_channelsPreDecisionSummaries )
    , m_channelsDecisionSummaries( rh.m_channelsDecisionSummaries )
    , m_conditionsValueSummaries( rh.m_conditionsValueSummaries )
    , m_sumEt( rh.m_sumEt )
    , m_valid( rh.m_valid )
    , m_bankVersion( rh.m_bankVersion )
    , m_dataMap( rh.m_dataMap )
    , m_bcid( rh.m_bcid )
    , m_configuration( rh.m_configuration ) {}

inline const CLID& LHCb::L0DUReport::clID() const { return LHCb::L0DUReport::classID(); }

inline const CLID& LHCb::L0DUReport::classID() { return CLID_L0DUReport; }

inline std::ostream& LHCb::L0DUReport::fillStream( std::ostream& s ) const {
  char l_timingTriggerBit = ( m_timingTriggerBit ) ? 'T' : 'F';
  char l_forceBit         = ( m_forceBit ) ? 'T' : 'F';
  char l_valid            = ( m_valid ) ? 'T' : 'F';
  s << "{ "
    << "decisionValue :	" << m_decisionValue << std::endl
    << "timingTriggerBit :	" << l_timingTriggerBit << std::endl
    << "forceBit :	" << l_forceBit << std::endl
    << "tck :	" << m_tck << std::endl
    << "channelsPreDecisionSummaries :	" << m_channelsPreDecisionSummaries << std::endl
    << "channelsDecisionSummaries :	" << m_channelsDecisionSummaries << std::endl
    << "conditionsValueSummaries :	" << m_conditionsValueSummaries << std::endl
    << "sumEt :	" << m_sumEt << std::endl
    << "valid :	" << l_valid << std::endl
    << "bankVersion :	" << m_bankVersion << std::endl
    << "dataMap :	" << m_dataMap << std::endl
    << "bcid :	" << m_bcid << std::endl
    << " }";
  return s;
}

inline int LHCb::L0DUReport::decisionValue() const { return m_decisionValue; }

inline void LHCb::L0DUReport::setDecisionValue( int value ) { m_decisionValue = value; }

inline bool LHCb::L0DUReport::timingTriggerBit() const { return m_timingTriggerBit; }

inline void LHCb::L0DUReport::setTimingTriggerBit( bool value ) { m_timingTriggerBit = value; }

inline bool LHCb::L0DUReport::forceBit() const { return m_forceBit; }

inline void LHCb::L0DUReport::setForceBit( bool value ) { m_forceBit = value; }

inline unsigned int LHCb::L0DUReport::tck() const { return m_tck; }

inline void LHCb::L0DUReport::setTck( unsigned int value ) { m_tck = value; }

inline const std::map<std::pair<int, unsigned int>, unsigned int>&
LHCb::L0DUReport::channelsPreDecisionSummaries() const {
  return m_channelsPreDecisionSummaries;
}

inline void
LHCb::L0DUReport::setChannelsPreDecisionSummaries( const std::map<std::pair<int, unsigned int>, unsigned int>& value ) {
  m_channelsPreDecisionSummaries = value;
}

inline const std::map<std::pair<int, unsigned int>, unsigned int>& LHCb::L0DUReport::channelsDecisionSummaries() const {
  return m_channelsDecisionSummaries;
}

inline void
LHCb::L0DUReport::setChannelsDecisionSummaries( const std::map<std::pair<int, unsigned int>, unsigned int>& value ) {
  m_channelsDecisionSummaries = value;
}

inline const std::map<std::pair<int, unsigned int>, unsigned int>& LHCb::L0DUReport::conditionsValueSummaries() const {
  return m_conditionsValueSummaries;
}

inline void
LHCb::L0DUReport::setConditionsValueSummaries( const std::map<std::pair<int, unsigned int>, unsigned int>& value ) {
  m_conditionsValueSummaries = value;
}

inline void LHCb::L0DUReport::setSumEt( const std::map<int, unsigned int>& value ) { m_sumEt = value; }

inline bool LHCb::L0DUReport::valid() const { return m_valid; }

inline void LHCb::L0DUReport::setValid( bool value ) { m_valid = value; }

inline int LHCb::L0DUReport::bankVersion() const { return m_bankVersion; }

inline void LHCb::L0DUReport::setBankVersion( int value ) { m_bankVersion = value; }

inline const std::map<std::string, std::pair<int, double>>& LHCb::L0DUReport::dataMap() const { return m_dataMap; }

inline void LHCb::L0DUReport::setDataMap( const std::map<std::string, std::pair<int, double>>& value ) {
  m_dataMap = value;
}

inline int LHCb::L0DUReport::bcid() const { return m_bcid; }

inline void LHCb::L0DUReport::setBcid( int value ) { m_bcid = value; }

inline const LHCb::L0DUConfig* LHCb::L0DUReport::configuration() const { return m_configuration; }

inline LHCb::L0DUConfig* LHCb::L0DUReport::configuration() { return m_configuration; }

inline void LHCb::L0DUReport::clear() {

  m_channelsDecisionSummaries.clear();
  m_channelsPreDecisionSummaries.clear();
  m_conditionsValueSummaries.clear();
  m_sumEt.clear();
  m_decisionValue    = 0x0;
  m_timingTriggerBit = false;
  m_forceBit         = false;
  m_tck              = 0x0000;
  m_configuration    = 0;
  m_valid            = true;
  m_bcid             = 0;
  m_bankVersion      = -1;
  m_dataMap.clear();
}

inline bool LHCb::L0DUReport::decision( int mask ) const { return ( m_decisionValue & mask ) != 0; }

inline void LHCb::L0DUReport::addDecisionValue( int decision ) { m_decisionValue |= decision; }

inline void LHCb::L0DUReport::setConfiguration( const SmartRef<LHCb::L0DUConfig>& config ) {

  m_configuration = config;
  m_tck           = config->key();
}

inline void LHCb::L0DUReport::setSumEt( int value, int bx ) { m_sumEt[bx] = value; }

inline int LHCb::L0DUReport::sumEt( int bx ) const {

  auto it = m_sumEt.find( bx );
  if ( it == m_sumEt.end() ) return 0;
  return it->second;
}

inline bool LHCb::L0DUReport::hasData( const std::string& name, int bx ) const {

  // first check the embedded data map
  auto ib = m_dataMap.find( name );
  if ( ( bx == 0 || ( std::abs( bx ) <= 2 && name == "Sum(Et)" ) ) && m_dataMap.end() != ib ) return true;
  // else check the configuration
  if ( 0 == m_configuration ) return false;
  const LHCb::L0DUElementaryData::Map& data = m_configuration->data();
  auto                                 it   = data.find( name );
  if ( it == data.end() ) return false;
  return ( it->second )->hasData( bx );
}

inline unsigned int LHCb::L0DUReport::dataDigit( const std::string& name, int bx ) const {

  if ( name == "Sum(Et)" && std::abs( bx ) <= 2 ) return sumEt( bx );
  // first check the embedded data map
  auto ib = m_dataMap.find( name );
  if ( bx == 0 && m_dataMap.end() != ib ) return ( ib->second ).first;

  // else check the configuration
  if ( 0 == m_configuration ) return 0;
  const LHCb::L0DUElementaryData::Map& data = m_configuration->data();
  auto                                 it   = data.find( name );
  if ( it == data.end() ) return 0;
  return ( it->second )->digit( bx );
}

inline double LHCb::L0DUReport::dataValue( const std::string& name, int bx ) const {

  // first check the embedded data map
  auto ib = m_dataMap.find( name );
  if ( bx == 0 && m_dataMap.end() != ib ) return double( ( ib->second ).first ) * ( ib->second ).second;

  // else check the configuration
  if ( 0 == m_configuration ) return 0.;
  const LHCb::L0DUElementaryData::Map& data = m_configuration->data();
  auto                                 it   = data.find( name );
  if ( it == data.end() ) return 0.;
  if ( name == "Sum(Et)" && std::abs( bx ) <= 2 ) return sumEt( bx ) * ( ( *it ).second )->scale();
  return ( ( *it ).second )->value( bx );
}

inline void LHCb::L0DUReport::addToData( const std::string& name, int value, double scale ) {

  auto ib = m_dataMap.find( name );
  if ( m_dataMap.end() == ib ) m_dataMap[name] = {value, scale};
}

inline void LHCb::L0DUReport::setChannelsPreDecisionSummary( const unsigned int summary, unsigned int loc, int bx ) {

  auto key                            = std::pair{bx, loc};
  m_channelsPreDecisionSummaries[key] = summary;
}

inline void LHCb::L0DUReport::setChannelsDecisionSummary( const unsigned int summary, unsigned int loc, int bx ) {

  auto key                         = std::pair{bx, loc};
  m_channelsDecisionSummaries[key] = summary;
}

inline void LHCb::L0DUReport::setConditionsValueSummary( const unsigned int summary, unsigned int loc, int bx ) {

  auto key                        = std::pair{bx, loc};
  m_conditionsValueSummaries[key] = summary;
}

inline void LHCb::L0DUReport::setChannelPreDecision( const std::string& name, bool triggered, int bx ) {

  if ( 0 == m_configuration ) return;

  const LHCb::L0DUChannel::Map& channels = m_configuration->channels();
  auto                          it       = channels.find( name );
  if ( it == channels.end() ) return;
  unsigned int pos     = ( ( *it ).second )->id();
  unsigned int index   = pos / 32;
  auto         key     = std::pair{bx, index};
  unsigned int summary = m_channelsPreDecisionSummaries[key];
  unsigned int loc     = pos - index * 32;
  triggered ? summary |= ( 1 << loc ) : summary &= ~( 1 << loc );
  m_channelsPreDecisionSummaries[key] = summary;
}

inline void LHCb::L0DUReport::setChannelDecision( const std::string& name, bool decision, int bx ) {

  if ( 0 == m_configuration ) return;
  const LHCb::L0DUChannel::Map& channels = m_configuration->channels();
  auto                          it       = channels.find( name );
  if ( it == channels.end() ) return;
  unsigned int pos     = ( ( *it ).second )->id();
  unsigned int index   = pos / 32;
  auto         key     = std::pair{bx, index};
  unsigned int summary = m_channelsDecisionSummaries[key];
  unsigned int loc     = pos - index * 32;
  decision ? summary |= ( 1 << loc ) : summary &= ~( 1 << loc );
  m_channelsDecisionSummaries[key] = summary;
}

inline void LHCb::L0DUReport::setConditionValue( const std::string& name, bool decision, int bx ) {

  if ( 0 == m_configuration ) return;
  const LHCb::L0DUElementaryCondition::Map& conditions = m_configuration->conditions();
  auto                                      it         = conditions.find( name );
  if ( it == conditions.end() ) return;
  unsigned int pos     = ( ( *it ).second )->reportBit();
  unsigned int index   = pos / 32;
  auto         key     = std::pair{bx, index};
  unsigned int summary = m_conditionsValueSummaries[key];
  unsigned int loc     = pos - index * 32;
  decision ? summary |= ( 1 << loc ) : summary &= ~( 1 << loc );
  m_conditionsValueSummaries[key] = summary;
}

inline unsigned int LHCb::L0DUReport::channelsDecisionSummary( unsigned int index, int bx ) const {

  auto key = std::pair{bx, index};
  auto it  = m_channelsDecisionSummaries.find( key );
  if ( it == m_channelsDecisionSummaries.end() ) return 0;
  return it->second;
}

inline unsigned int LHCb::L0DUReport::channelsPreDecisionSummary( unsigned int index, int bx ) const {

  auto key = std::pair{bx, index};
  auto it  = m_channelsPreDecisionSummaries.find( key );
  if ( it == m_channelsPreDecisionSummaries.end() ) return 0;
  return it->second;
}

inline unsigned int LHCb::L0DUReport::conditionsValueSummary( unsigned int index, int bx ) const {

  auto key = std::pair{bx, index};
  auto it  = m_conditionsValueSummaries.find( key );
  if ( it == m_conditionsValueSummaries.end() ) return 0;
  return it->second;
}

inline bool LHCb::L0DUReport::channelDecision( const unsigned int channel, int bx ) const {

  unsigned int index = channel / 32;
  auto         key   = std::pair{bx, index};
  auto         it    = m_channelsDecisionSummaries.find( key );
  if ( it == m_channelsDecisionSummaries.end() ) return 0;
  unsigned int summary = ( *it ).second;
  unsigned int loc     = channel - index * 32;
  return ( 1 & ( summary >> loc ) );
}

inline bool LHCb::L0DUReport::channelPreDecision( const unsigned int channel, int bx ) const {

  unsigned int index = channel / 32;
  auto         key   = std::pair{bx, index};
  auto         it    = m_channelsPreDecisionSummaries.find( key );
  if ( it == m_channelsPreDecisionSummaries.end() ) return 0;
  unsigned int summary = ( *it ).second;
  unsigned int loc     = channel - index * 32;
  return ( 1 & ( summary >> loc ) );
}

inline bool LHCb::L0DUReport::conditionValue( const unsigned int condition, int bx ) const {

  unsigned int index = condition / 32;
  auto         key   = std::pair{bx, index};
  auto         it    = m_conditionsValueSummaries.find( key );
  if ( it == m_conditionsValueSummaries.end() ) return 0;
  unsigned int summary = ( *it ).second;
  unsigned int loc     = condition - index * 32;
  return ( 1 & ( summary >> loc ) );
}

inline bool LHCb::L0DUReport::channelPreDecisionByName( const std::string& name, int bx ) const {

  if ( 0 == m_configuration ) return false;
  const LHCb::L0DUChannel::Map&          channels = m_configuration->channels();
  LHCb::L0DUChannel::Map::const_iterator it       = channels.find( name );
  if ( it == channels.end() ) return false;
  const unsigned int pos = it->second->id();
  return channelPreDecision( pos, bx );
}

inline bool LHCb::L0DUReport::channelDecisionByName( const std::string& name, int bx ) const {

  if ( 0 == m_configuration ) return false;
  const LHCb::L0DUChannel::Map& channels = m_configuration->channels();
  auto                          it       = channels.find( name );
  if ( it == channels.end() ) return false;
  const unsigned int pos = it->second->id();
  return channelDecision( pos, bx );
}

inline bool LHCb::L0DUReport::conditionValueByName( const std::string& name, int bx ) const {

  if ( 0 == m_configuration ) return false;
  const LHCb::L0DUElementaryCondition::Map&          conditions = m_configuration->conditions();
  LHCb::L0DUElementaryCondition::Map::const_iterator it         = conditions.find( name );
  if ( it == conditions.end() ) return false;
  if ( !( it->second )->reported() ) return false;
  const unsigned int pos = it->second->reportBit();
  return conditionValue( pos, bx );
}

inline std::string LHCb::L0DUReport::channelName( const unsigned int channel ) const {

  std::string bad = "LHCb::L0DUConfig is missing";
  if ( 0 == m_configuration ) return bad;
  const LHCb::L0DUChannel::Map&          channels = m_configuration->channels();
  LHCb::L0DUChannel::Map::const_iterator it;
  for ( it = channels.begin(); it != channels.end(); it++ ) {
    unsigned int id = ( ( *it ).second )->id();
    if ( channel == id ) return ( ( *it ).second )->name();
  }
  bad = " Channel no ";
  std::ostringstream os( " " );
  os << channel;
  bad += os.str();
  bad += " is not defined in the loaded configuration - Please check ";
  return bad;
}

inline std::string LHCb::L0DUReport::conditionName( const unsigned int cond ) const {

  std::string bad = "LHCb::L0DUConfig is missing";
  if ( 0 == m_configuration ) return bad;
  const LHCb::L0DUElementaryCondition::Map&          conditions = m_configuration->conditions();
  LHCb::L0DUElementaryCondition::Map::const_iterator it;
  for ( it = conditions.begin(); it != conditions.end(); it++ ) {
    unsigned int id = ( ( *it ).second )->id();
    if ( cond == id ) return ( ( *it ).second )->name();
  }
  bad = " Condition no ";
  std::ostringstream os( "" );
  os << cond;
  bad += os.str();
  bad += " is not defined in the loaded configuration - Please check ";
  return bad;
}

inline bool LHCb::L0DUReport::globalDecisionFromChannel( const std::string& name, int type ) const {

  if ( 0 == m_configuration ) return false;
  const LHCb::L0DUChannel::Map&          channels = m_configuration->channels();
  LHCb::L0DUChannel::Map::const_iterator it       = channels.find( name );
  if ( it == channels.end() ) return false;
  LHCb::L0DUChannel* channel = ( *it ).second;
  if ( channelDecisionByName( name ) && ( channel->decisionType() & type ) != 0 ) return true;
  return false;
}

inline bool LHCb::L0DUReport::decisionFromSummary( int type, int bx ) const {

  if ( 0 == m_configuration ) return false;
  const LHCb::L0DUChannel::Map& channels = m_configuration->channels();
  for ( LHCb::L0DUChannel::Map::const_iterator it = channels.begin(); it != channels.end(); it++ ) {
    LHCb::L0DUChannel* channel = ( *it ).second;
    if ( ( channel->decisionType() & type ) == 0 ) continue;
    if ( channelDecision( channel->id(), bx ) ) return true;
  }
  return false;
}

inline bool LHCb::L0DUReport::preDecisionFromSummary( int type, int bx ) const {

  if ( 0 == m_configuration ) return false;
  const LHCb::L0DUChannel::Map& channels = m_configuration->channels();
  for ( LHCb::L0DUChannel::Map::const_iterator it = channels.begin(); it != channels.end(); it++ ) {
    LHCb::L0DUChannel* channel = ( *it ).second;
    if ( ( channel->decisionType() & type ) == 0 ) continue;
    if ( channelPreDecision( channel->id(), bx ) ) return true;
  }
  return false;
}

inline int LHCb::L0DUReport::preDecisionValueFromSummary( int bx ) const {

  int value = 0;
  if ( 0 == m_configuration ) return value;
  const LHCb::L0DUChannel::Map& channels = m_configuration->channels();
  for ( LHCb::L0DUChannel::Map::const_iterator it = channels.begin(); it != channels.end(); it++ ) {
    LHCb::L0DUChannel* channel = ( *it ).second;
    int                type    = channel->decisionType();
    if ( channelPreDecision( channel->id(), bx ) ) value |= type;
  }
  return value;
}

inline bool LHCb::L0DUReport::triggerDecisionByName( const std::string& name, int bx ) const {

  if ( 0 == m_configuration ) return false;
  const LHCb::L0DUTrigger::Map&          triggers = m_configuration->triggers();
  LHCb::L0DUTrigger::Map::const_iterator it       = triggers.find( name );
  if ( it == triggers.end() ) {
    if ( "L0" == name || "Physics" == name ) {
      return decisionFromSummary( L0DUDecision::Physics, bx );
    } else if ( "Beam1" == name ) {
      return decisionFromSummary( L0DUDecision::Beam1, bx );
    } else if ( "Beam2" == name ) {
      return decisionFromSummary( L0DUDecision::Beam2, bx );
    } else {
      return false;
    }
  }
  const LHCb::L0DUChannel::Map& channels = ( *it ).second->channels();
  for ( LHCb::L0DUChannel::Map::const_iterator ic = channels.begin(); ic != channels.end(); ++ic ) {
    if ( channelDecisionByName( ( *ic ).second->name(), bx ) ) return true;
  }
  return false;
}

inline bool LHCb::L0DUReport::triggerDecision( unsigned int index, int bx ) const {

  if ( 0 == m_configuration ) return false;
  const LHCb::L0DUTrigger::Map& triggers = m_configuration->triggers();
  for ( LHCb::L0DUTrigger::Map::const_iterator it = triggers.begin(); it != triggers.end(); ++it ) {
    if ( index == ( *it ).second->id() ) return triggerDecisionByName( ( *it ).first, bx );
  }
  return false;
}

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
#include "Event/HltDecReport.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_HltDecReports = 7521;

  // Namespace for locations in TDS
  namespace HltDecReportsLocation {
    inline const std::string Default     = "Hlt/DecReports";
    inline const std::string Hlt1Default = "Hlt1/DecReports";
    inline const std::string Hlt2Default = "Hlt2/DecReports";
    inline const std::string Emulated    = "Hlt/Emu/DecReports";
  } // namespace HltDecReportsLocation

  /** @class HltDecReports HltDecReports.h
   *
   * managed container of Hlt Trigger Decision Reports
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltDecReports : public DataObject {
  public:
    /// container of HltDecReport-s keyed by trigger decision name
    typedef GaudiUtils::VectorMap<std::string, LHCb::HltDecReport> Container;
    /// const iterator over of HltDecReport
    typedef Container::const_iterator const_iterator;
    /// const reference to contained object
    typedef Container::const_reference const_reference;
    /// type of contained object
    typedef Container::value_type value_type;

    /// Default Constructor
    HltDecReports() : m_configuredTCK( 0 ), m_taskID( 0 ), m_decReports() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// return pointer to Hlt Decision Report for given trigger decision name (==0 if not found)
    const LHCb::HltDecReport* decReport( const std::string& decisionName ) const;

    /// check if the trigger decision name is present in the container
    bool hasDecisionName( const std::string& decisionName ) const;

    /// return names of the decisions stored in the container
    std::vector<std::string> decisionNames() const;

    /// begin iterator
    HltDecReports::Container::const_iterator begin() const;

    /// end iterator
    HltDecReports::Container::const_iterator end() const;

    /// find HltDecReport in the container given its trigger decision name
    HltDecReports::Container::const_iterator find( const std::string& decisionName ) const;

    /// size of the HltDecReports container (i.e. number of decision reports stored)
    HltDecReports::Container::size_type size() const;

    /// reserve space for (at least) num elements
    void reserve( const Container::size_type& num );

    /// insert HltDecReport for given decisionName, returns StatusCode::FAILURE if duplicate decision name
    StatusCode insert( const std::string& decisionName, const LHCb::HltDecReport& decReport );

    /// insert HltDecReport for given decisionName
    std::pair<LHCb::HltDecReports::Container::const_iterator, bool> insert( const Container::value_type& value );

    /// insert HltDecReport for given decisionName
    std::pair<LHCb::HltDecReports::Container::const_iterator, bool> insert( const Container::const_iterator& hint,
                                                                            const Container::value_type&     value );

    /// insert HltDecReport for given decisionName
    std::pair<LHCb::HltDecReports::Container::const_iterator, bool> insert( const Container::const_iterator& hint,
                                                                            const Container::key_type&       key,
                                                                            const Container::mapped_type&    mapped );

    /// intelligent printout
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Trigger Configuration Key used for Configuration
    unsigned int configuredTCK() const;

    /// Update  Trigger Configuration Key used for Configuration
    void setConfiguredTCK( unsigned int value );

    /// Retrieve const  Reserved for online Task / Node ID
    unsigned int taskID() const;

    /// Update  Reserved for online Task / Node ID
    void setTaskID( unsigned int value );

    /// Retrieve const  container of HltDecReport-s keyed by trigger decision name
    const LHCb::HltDecReports::Container& decReports() const;

    /// Update  container of HltDecReport-s keyed by trigger decision name
    void setDecReports( const LHCb::HltDecReports::Container& value );

    friend std::ostream& operator<<( std::ostream& str, const HltDecReports& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned int                   m_configuredTCK; ///< Trigger Configuration Key used for Configuration
    unsigned int                   m_taskID;        ///< Reserved for online Task / Node ID
    LHCb::HltDecReports::Container m_decReports;    ///< container of HltDecReport-s keyed by trigger decision name

  }; // class HltDecReports

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::HltDecReports::clID() const { return LHCb::HltDecReports::classID(); }

inline const CLID& LHCb::HltDecReports::classID() { return CLID_HltDecReports; }

inline unsigned int LHCb::HltDecReports::configuredTCK() const { return m_configuredTCK; }

inline void LHCb::HltDecReports::setConfiguredTCK( unsigned int value ) { m_configuredTCK = value; }

inline unsigned int LHCb::HltDecReports::taskID() const { return m_taskID; }

inline void LHCb::HltDecReports::setTaskID( unsigned int value ) { m_taskID = value; }

inline const LHCb::HltDecReports::Container& LHCb::HltDecReports::decReports() const { return m_decReports; }

inline void LHCb::HltDecReports::setDecReports( const LHCb::HltDecReports::Container& value ) { m_decReports = value; }

inline const LHCb::HltDecReport* LHCb::HltDecReports::decReport( const std::string& decisionName ) const {

  auto rep = m_decReports.find( decisionName );
  if ( rep == m_decReports.end() ) return nullptr;
  return &( rep->second );
}

inline bool LHCb::HltDecReports::hasDecisionName( const std::string& decisionName ) const {

  return m_decReports.find( decisionName ) != m_decReports.end();
}

inline std::vector<std::string> LHCb::HltDecReports::decisionNames() const {

  std::vector<std::string> names;
  names.reserve( m_decReports.size() );
  std::transform( begin(), end(), std::back_inserter( names ), []( const_reference i ) { return i.first; } );
  return names;
}

inline LHCb::HltDecReports::Container::const_iterator LHCb::HltDecReports::begin() const {

  return m_decReports.begin();
}

inline LHCb::HltDecReports::Container::const_iterator LHCb::HltDecReports::end() const { return m_decReports.end(); }

inline LHCb::HltDecReports::Container::const_iterator
LHCb::HltDecReports::find( const std::string& decisionName ) const {

  return m_decReports.find( decisionName );
}

inline LHCb::HltDecReports::Container::size_type LHCb::HltDecReports::size() const { return m_decReports.size(); }

inline void LHCb::HltDecReports::reserve( const Container::size_type& num ) { m_decReports.reserve( num ); }

inline StatusCode LHCb::HltDecReports::insert( const std::string& decisionName, const LHCb::HltDecReport& decReport ) {

  return m_decReports.insert( decisionName, decReport ).second ? StatusCode::SUCCESS : StatusCode::FAILURE;
}

inline std::pair<LHCb::HltDecReports::Container::const_iterator, bool>
LHCb::HltDecReports::insert( const Container::value_type& value ) {

  return m_decReports.insert( value );
}

inline std::pair<LHCb::HltDecReports::Container::const_iterator, bool>
LHCb::HltDecReports::insert( const Container::const_iterator& hint, const Container::value_type& value ) {

  return m_decReports.insert( hint, value );
}

inline std::pair<LHCb::HltDecReports::Container::const_iterator, bool>
LHCb::HltDecReports::insert( const Container::const_iterator& hint, const Container::key_type& key,
                             const Container::mapped_type& mapped ) {

  return m_decReports.insert( hint, key, mapped );
}

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
#include "Event/VertexBase.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/SmartRefVector.h"
#include "GaudiKernel/VectorMap.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_HltVertexReports = 7551;

  // Namespace for locations in TDS
  namespace HltVertexReportsLocation {
    inline const std::string Default  = "Hlt/VertexReports";
    inline const std::string Emulated = "Hlt/Emu/VertexReports";
  } // namespace HltVertexReportsLocation

  /** @class HltVertexReports HltVertexReports.h
   *
   * managed container of Hlt Trigger Vertex Reports
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltVertexReports final : public DataObject {
  public:
    /// list of vertices for given trigger selection
    typedef SmartRefVector<LHCb::VertexBase> HltVertexReport;
    /// container of HltVertexReport-s keyed by trigger selection name
    typedef GaudiUtils::VectorMap<std::string, HltVertexReport> Container;

    /// Default Constructor
    HltVertexReports() : m_vertexReports() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// return pointer to Hlt Vertex Report for given trigger selection name (==0 if not found)
    const LHCb::HltVertexReports::HltVertexReport* vertexReport( const std::string& selectionName ) const;

    /// check if the trigger selection name is present in the container
    bool hasSelectionName( const std::string& selectionName ) const;

    /// return names of the selections stored in the container
    std::vector<std::string> selectionNames() const;

    /// begin iterator
    HltVertexReports::Container::const_iterator begin() const;

    /// end iterator
    HltVertexReports::Container::const_iterator end() const;

    /// find HltVertexReport in the container given its trigger selection name
    HltVertexReports::Container::const_iterator find( const std::string& selectionName ) const;

    /// size of the HltVertexReports container (i.e. number of vertex reports stored)
    HltVertexReports::Container::size_type size() const;

    /// insert HltVertexReport for given selectionName, returns StatusCode::FAILURE if duplicate selection name
    StatusCode insert( const std::string& selectionName, const LHCb::HltVertexReports::HltVertexReport& vertexReport );

    /// intelligent printout
    std::ostream& fillStream( std::ostream& ss ) const override;

    /// Retrieve const  container of HltVertexReport-s keyed by trigger selection name
    const LHCb::HltVertexReports::Container& vertexReports() const;

    /// Update  container of HltVertexReport-s keyed by trigger selection name
    void setVertexReports( const LHCb::HltVertexReports::Container& value );

    friend std::ostream& operator<<( std::ostream& str, const HltVertexReports& obj ) { return obj.fillStream( str ); }

  protected:
    LHCb::HltVertexReports::Container m_vertexReports; ///< container of HltVertexReport-s keyed by trigger selection
                                                       ///< name

  private:
  }; // class HltVertexReports

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::HltVertexReports::clID() const { return LHCb::HltVertexReports::classID(); }

inline const CLID& LHCb::HltVertexReports::classID() { return CLID_HltVertexReports; }

inline const LHCb::HltVertexReports::Container& LHCb::HltVertexReports::vertexReports() const {
  return m_vertexReports;
}

inline void LHCb::HltVertexReports::setVertexReports( const LHCb::HltVertexReports::Container& value ) {
  m_vertexReports = value;
}

inline const LHCb::HltVertexReports::HltVertexReport*
LHCb::HltVertexReports::vertexReport( const std::string& selectionName ) const {

  Container::const_iterator rep = m_vertexReports.find( selectionName );
  if ( rep == m_vertexReports.end() ) return ( (const LHCb::HltVertexReports::HltVertexReport*)( 0 ) );
  return &( rep->second );
}

inline bool LHCb::HltVertexReports::hasSelectionName( const std::string& selectionName ) const {

  return ( m_vertexReports.find( selectionName ) != m_vertexReports.end() );
}

inline std::vector<std::string> LHCb::HltVertexReports::selectionNames() const {

  std::vector<std::string>  names;
  Container::const_iterator it = m_vertexReports.begin();
  while ( it != m_vertexReports.end() ) names.push_back( ( it++ )->first );
  return names;
}

inline LHCb::HltVertexReports::Container::const_iterator LHCb::HltVertexReports::begin() const {

  return m_vertexReports.begin();
}

inline LHCb::HltVertexReports::Container::const_iterator LHCb::HltVertexReports::end() const {

  return m_vertexReports.end();
}

inline LHCb::HltVertexReports::Container::const_iterator
LHCb::HltVertexReports::find( const std::string& selectionName ) const {

  return m_vertexReports.find( selectionName );
}

inline LHCb::HltVertexReports::Container::size_type LHCb::HltVertexReports::size() const {

  return m_vertexReports.size();
}

inline StatusCode LHCb::HltVertexReports::insert( const std::string&                             selectionName,
                                                  const LHCb::HltVertexReports::HltVertexReport& vertexReport ) {

  if ( m_vertexReports.insert( selectionName, vertexReport ).second ) return StatusCode::SUCCESS;
  return StatusCode::FAILURE;
}

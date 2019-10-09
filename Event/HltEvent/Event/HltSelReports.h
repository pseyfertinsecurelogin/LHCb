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
#include "Event/HltObjectSummary.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/VectorMap.h"
#include "boost/format.hpp"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_HltSelReports = 7531;

  // Namespace for locations in TDS
  namespace HltSelReportsLocation {
    inline const std::string Default  = "Hlt/SelReports";
    inline const std::string Emulated = "Hlt/Emu/SelReports";
  } // namespace HltSelReportsLocation

  /** @class HltSelReports HltSelReports.h
   *
   * managed container of Hlt Selection Reports
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltSelReports final : public DataObject {
  public:
    /// container of HltObjectSummary-s keyed by trigger selection name
    typedef GaudiUtils::VectorMap<std::string, LHCb::HltObjectSummary> Container;

    /// Default Constructor
    HltSelReports() : m_selReports() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// returns a pointer to Hlt Selection Report for given trigger selection name (==0 if not found)
    const LHCb::HltObjectSummary* selReport( const std::string& selectionName ) const;

    /// check if the trigger selection name is present in the container
    bool hasSelectionName( const std::string& selectionName ) const;

    /// return names of the selections stored in the container
    std::vector<std::string> selectionNames() const;

    /// begin iterator
    HltSelReports::Container::const_iterator begin() const;

    /// end iterator
    HltSelReports::Container::const_iterator end() const;

    /// find HltObjectSummary in the container given its trigger selection name
    HltSelReports::Container::const_iterator find( const std::string& selectionName ) const;

    /// size of the HltSelReports container (i.e. number of selection reports stored)
    HltSelReports::Container::size_type size() const;

    /// insert HltObjectSummary for given selectionName, returns StatusCode::FAILURE if duplicate selection name
    StatusCode insert( const std::string& selectionName, const LHCb::HltObjectSummary& selReport );

    /// returns names of all selections in the report to which the given summarized candidate belongs to (must be among
    /// top level objects of the selection)
    std::vector<std::string> selectedAsCandidateBy( const LHCb::HltObjectSummary* selectedCandidate ) const;

    /// returns name of Primary Vertex selection used in cuts employed for given trigger selection name ("" if none or
    /// not saved)
    std::string pvSelectionName( const std::string& selectionName ) const;

    /// returns key of the Primary Vertex in its container used in selection of the given summarized candidate (=-3
    /// illegal selection,=-2 illegal candi.=-1 if none or not saved)
    int pvKey( const std::string& selectionName, const LHCb::HltObjectSummary* selectedCandidate ) const;

    /// returns name of PV selection and PV key used in selection of the given summarized candidate. Goes via 1st
    /// candidate selection found. ("",-1 if none).
    std::pair<std::string, int> pvSelectionNameAndKey( const LHCb::HltObjectSummary* selectedCandidate ) const;

    /// intelligent printout
    std::ostream& fillStream( std::ostream& ss ) const override;

    /// Retrieve const  container of HltObjectSummary-s keyed by trigger selection name
    const LHCb::HltSelReports::Container& selReports() const;

    /// Update  container of HltObjectSummary-s keyed by trigger selection name
    void setSelReports( const LHCb::HltSelReports::Container& value );

    friend std::ostream& operator<<( std::ostream& str, const HltSelReports& obj ) { return obj.fillStream( str ); }

  protected:
    LHCb::HltSelReports::Container m_selReports; ///< container of HltObjectSummary-s keyed by trigger selection name

  private:
  }; // class HltSelReports

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::HltSelReports::clID() const { return LHCb::HltSelReports::classID(); }

inline const CLID& LHCb::HltSelReports::classID() { return CLID_HltSelReports; }

inline const LHCb::HltSelReports::Container& LHCb::HltSelReports::selReports() const { return m_selReports; }

inline void LHCb::HltSelReports::setSelReports( const LHCb::HltSelReports::Container& value ) { m_selReports = value; }

inline const LHCb::HltObjectSummary* LHCb::HltSelReports::selReport( const std::string& selectionName ) const {

  auto rep = m_selReports.find( selectionName );
  return rep != m_selReports.end() ? &( rep->second ) : nullptr;
}

inline bool LHCb::HltSelReports::hasSelectionName( const std::string& selectionName ) const {

  return m_selReports.find( selectionName ) != m_selReports.end();
}

inline std::vector<std::string> LHCb::HltSelReports::selectionNames() const {

  std::vector<std::string> names;
  names.reserve( m_selReports.size() );
  std::transform( m_selReports.begin(), m_selReports.end(), std::back_inserter( names ),
                  []( const auto& p ) { return p.first; } );
  return names;
}

inline LHCb::HltSelReports::Container::const_iterator LHCb::HltSelReports::begin() const {

  return m_selReports.begin();
}

inline LHCb::HltSelReports::Container::const_iterator LHCb::HltSelReports::end() const { return m_selReports.end(); }

inline LHCb::HltSelReports::Container::const_iterator
LHCb::HltSelReports::find( const std::string& selectionName ) const {

  return m_selReports.find( selectionName );
}

inline LHCb::HltSelReports::Container::size_type LHCb::HltSelReports::size() const { return m_selReports.size(); }

inline StatusCode LHCb::HltSelReports::insert( const std::string&            selectionName,
                                               const LHCb::HltObjectSummary& selReport ) {

  return m_selReports.insert( selectionName, selReport ).second ? StatusCode::SUCCESS : StatusCode::FAILURE;
}

inline std::vector<std::string>
LHCb::HltSelReports::selectedAsCandidateBy( const LHCb::HltObjectSummary* selectedCandidate ) const {

  std::vector<std::string> selNamesV;
  for ( const auto& r : m_selReports ) {
    const auto& candidates = r.second.substructure();
    if ( std::any_of( candidates.begin(), candidates.end(),
                      [&]( const auto& c ) { return c.target() == selectedCandidate; } ) )
      selNamesV.push_back( r.first );
  }
  return selNamesV;
}

inline std::string LHCb::HltSelReports::pvSelectionName( const std::string& selectionName ) const {

  const LHCb::HltObjectSummary* summaryOfSelection = selReport( selectionName );
  if ( !summaryOfSelection ) return std::string{};
  for ( const auto& i : summaryOfSelection->numericalInfo() ) {
    const std::string& nInfoKey = i.first;
    if ( nInfoKey.compare( 0, 3, "10#" ) == 0 ) return nInfoKey.substr( 3 );
  }
  return std::string{};
}

inline int LHCb::HltSelReports::pvKey( const std::string&            selectionName,
                                       const LHCb::HltObjectSummary* selectedCandidate ) const {

  const LHCb::HltObjectSummary* summaryOfSelection = selReport( selectionName );
  if ( !summaryOfSelection ) return -3;
  // make sure that the candidate in fact in this selection
  const SmartRefVector<LHCb::HltObjectSummary>& candidates = summaryOfSelection->substructure();
  int                                           iKey( 0 );
  for ( auto iObj = candidates.begin(); iObj != candidates.end(); ++iObj, ++iKey ) {
    if ( iObj->target() == selectedCandidate ) {
      const HltObjectSummary::Info& nInfo = summaryOfSelection->numericalInfo();
      for ( auto i = nInfo.begin(); i != nInfo.end(); ++i ) {
        const std::string& nInfoKey = i->first;
        if ( nInfoKey.compare( 0, 3, "10#" ) == 0 ) {
          // std::string pvSelectionName = nInfoKey.substr(3); // not used
          // now unpack the key
          int               j   = iKey / 4;
          int               inj = iKey - j * 4;
          std::stringstream ss;
          ss << "11#" << boost::format( "%1$=08X" ) % j;
          if ( nInfo.find( ss.str() ) != nInfo.end() ) {
            union {
              unsigned int mInt;
              float        mFloat;
            };
            mFloat             = nInfo( ss.str() );
            unsigned int pWord = mInt;
            return ( ( pWord >> inj * 8 ) & 255 );
          }
          return 0; // that means all keys were ==0
        }
      }
      return -1;
    }
  }
  return -2;
}

inline std::pair<std::string, int>
LHCb::HltSelReports::pvSelectionNameAndKey( const LHCb::HltObjectSummary* selectedCandidate ) const {

  const auto& selectedBy = selectedAsCandidateBy( selectedCandidate );
  if ( !selectedBy.empty() ) { return {pvSelectionName( selectedBy[0] ), pvKey( selectedBy[0], selectedCandidate )}; }
  return {"", -1};
}

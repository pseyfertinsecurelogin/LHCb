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
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include <map>
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_IntLink = 500;

  /** @class IntLink IntLink.h
   *
   * Link to int
   *
   * @author Jose A. Hernando
   *
   */

  class IntLink final : public DataObject {
  public:
    /// Default Constructor
    IntLink() : m_link() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Sets the int value asociated to this key
    void setLink( int key, int value );

    /// Returns the link associated to this key
    int link( int key ) const;

    /// Returns the index of the key. True if key exist, else inserting position
    bool hasKey( int key ) const;

    /// Retrieve const  list of linked ints
    const std::map<int, int>& link() const;

    /// Update  list of linked ints
    void setLink( const std::map<int, int>& value );

    friend std::ostream& operator<<( std::ostream& str, const IntLink& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    std::map<int, int> m_link; ///< list of linked ints

  }; // class IntLink

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::IntLink::clID() const { return LHCb::IntLink::classID(); }

inline const CLID& LHCb::IntLink::classID() { return CLID_IntLink; }

inline std::ostream& LHCb::IntLink::fillStream( std::ostream& s ) const {
  s << "{ "
    << "link :	" << m_link << std::endl
    << " }";
  return s;
}

inline const std::map<int, int>& LHCb::IntLink::link() const { return m_link; }

inline void LHCb::IntLink::setLink( const std::map<int, int>& value ) { m_link = value; }

inline void LHCb::IntLink::setLink( int key, int value ) { m_link[key] = value; }

inline int LHCb::IntLink::link( int key ) const {

  if ( m_link.find( key ) == m_link.end() ) return 0;
  return m_link.find( key )->second;
}

inline bool LHCb::IntLink::hasKey( int key ) const { return ( m_link.find( key ) != m_link.end() ); }

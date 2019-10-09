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
#include "Event/MCParticle.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include <ostream>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_MCProperty = 300;

  // Namespace for locations in TDS
  namespace MCPropertyLocation {
    inline const std::string TrackInfo = "MC/TrackInfo";
  }

  /** @class MCProperty MCProperty.h
   *
   * Give an int attribute to an MC particle
   *
   * @author OlivierCallot
   *
   */

  class MCProperty final : public DataObject {
  public:
    /// Default Constructor
    MCProperty() : m_property() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Sets the property for the specified particle
    void setProperty( const MCParticle* part, int property );

    /// Returns the property for the specified particle
    int property( const MCParticle* part ) const;

    friend std::ostream& operator<<( std::ostream& str, const MCProperty& obj ) { return obj.fillStream( str ); }

  protected:
    /// Returns the index of the key. True if key exist, else inserting position
    bool findIndex( int key, int& index ) const;

  private:
    std::vector<std::pair<int, int>> m_property; ///< List of linked objects

  }; // class MCProperty

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCProperty::clID() const { return LHCb::MCProperty::classID(); }

inline const CLID& LHCb::MCProperty::classID() { return CLID_MCProperty; }

inline std::ostream& LHCb::MCProperty::fillStream( std::ostream& s ) const {
  s << "{ "
    << "property :	" << m_property << std::endl
    << " }";
  return s;
}

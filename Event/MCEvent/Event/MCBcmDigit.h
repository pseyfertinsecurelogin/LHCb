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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_MCBcmDigit = 3000;

  // Namespace for locations in TDS
  namespace MCBcmDigitLocation {
    inline const std::string Default = "MC/Bcm/Digits";
  }

  /** @class MCBcmDigit MCBcmDigit.h
   *
   * BCM MC digit class
   *
   * @author M Lieng
   *
   */

  class MCBcmDigit final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of MCBcmDigit
    typedef KeyedContainer<MCBcmDigit, Containers::HashMap> Container;

    /// Default Constructor
    MCBcmDigit() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override;
    static const CLID&        classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Keys to MCHits
    [[nodiscard]] const std::vector<int>& mcHitKeys() const;

    /// Update  Keys to MCHits
    MCBcmDigit& setMCHitKeys( std::vector<int> value );

    friend std::ostream& operator<<( std::ostream& str, const MCBcmDigit& obj ) { return obj.fillStream( str ); }

  private:
    std::vector<int> m_MCHitKeys; ///< Keys to MCHits

  }; // class MCBcmDigit

  /// Definition of Keyed Container for MCBcmDigit
  using MCBcmDigits = KeyedContainer<MCBcmDigit, Containers::HashMap>;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCBcmDigit::clID() const { return LHCb::MCBcmDigit::classID(); }

inline const CLID& LHCb::MCBcmDigit::classID() { return CLID_MCBcmDigit; }

inline std::ostream& LHCb::MCBcmDigit::fillStream( std::ostream& s ) const {
  return s << "{ "
           << "MCHitKeys :	" << m_MCHitKeys << std::endl
           << " }";
}

inline const std::vector<int>& LHCb::MCBcmDigit::mcHitKeys() const { return m_MCHitKeys; }

inline LHCb::MCBcmDigit& LHCb::MCBcmDigit::setMCHitKeys( std::vector<int> value ) {
  m_MCHitKeys = std::move( value );
  return *this;
}

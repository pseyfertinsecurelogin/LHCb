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
#include "Kernel/CaloCellID.h"
#include <ostream>
#include <string>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_L0PrsSpdHit = 6007;

  // Namespace for locations in TDS
  namespace L0PrsSpdHitLocation {
    inline const std::string Prs = "Trig/L0/PrsRaw";
    inline const std::string Spd = "Trig/L0/SpdRaw";
  } // namespace L0PrsSpdHitLocation

  /** @class L0PrsSpdHit L0PrsSpdHit.h
   *
   * Fired cells in Prs or Spd.
   *
   * @author Olivier Callot
   *
   */

  class L0PrsSpdHit final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of L0PrsSpdHit
    typedef KeyedContainer<L0PrsSpdHit, Containers::HashMap> Container;

    /// Usual constructor, specifies the CellID
    L0PrsSpdHit( const CaloCellID& id ) : KeyedObject<int>( id.all() ) {}

    /// Default Constructor
    L0PrsSpdHit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve cell identifier/key @attention alias to Base::key() method!
    const LHCb::CaloCellID cellID() const;

    friend std::ostream& operator<<( std::ostream& str, const L0PrsSpdHit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
  }; // class L0PrsSpdHit

  /// Definition of Keyed Container for L0PrsSpdHit
  typedef KeyedContainer<L0PrsSpdHit, Containers::HashMap> L0PrsSpdHits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::L0PrsSpdHit::clID() const { return LHCb::L0PrsSpdHit::classID(); }

inline const CLID& LHCb::L0PrsSpdHit::classID() { return CLID_L0PrsSpdHit; }

inline std::ostream& LHCb::L0PrsSpdHit::fillStream( std::ostream& s ) const { return s; }

inline const LHCb::CaloCellID LHCb::L0PrsSpdHit::cellID() const { return LHCb::CaloCellID( key() ); }

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
#include "Event/static_vector_streamer.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include "Kernel/MuonTileID.h"
#include "boost/container/static_vector.hpp"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_MuonCoord = 11040;

  // Namespace for locations in TDS
  namespace MuonCoordLocation {
    inline const std::string MuonCoords = "Raw/Muon/Coords";
  }

  /** @class MuonCoord MuonCoord.h
   *
   * Reconstructed muon hit coordinates
   *
   * @author David Hutchcroft
   *
   */

  class MuonCoord final : public KeyedObject<LHCb::MuonTileID> {
  public:
    /// typedef for KeyedContainer of MuonCoord
    typedef KeyedContainer<MuonCoord, Containers::HashMap> Container;

    /// Constructor
    MuonCoord( const MuonTileID& key, const LHCb::MuonTileID& tile1, const LHCb::MuonTileID& tile2, unsigned int tdc1,
               unsigned int tdc2 )
        : KeyedObject<MuonTileID>{key}, m_digitTile{tile1, tile2}, m_digitTDC1{tdc1}, m_digitTDC2{tdc2} {}

    /// Constructor
    MuonCoord( const MuonTileID& tile, unsigned int tdc )
        : KeyedObject<MuonTileID>{tile}, m_digitTile{tile}, m_digitTDC1{tdc}, m_digitTDC2{0} {}

    /// Move constructor
    MuonCoord( MuonCoord&& rhs )
        : KeyedObject<MuonTileID>{rhs.key()}
        , m_digitTile{std::move( rhs.m_digitTile )}
        , m_digitTDC1{rhs.m_digitTDC1}
        , m_digitTDC2{rhs.m_digitTDC2} {}

    /// Default Constructor
    MuonCoord() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override { return LHCb::MuonCoord::classID(); }
    static const CLID&        classID() { return CLID_MuonCoord; }

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// true if this pad should have crossed another logic channel but did not
    [[nodiscard]] bool uncrossed() const { return m_digitTile.size() < 2; }

    /// Average Time of the coord in TDC bit unit
    [[nodiscard]] float AverageTime() const {
      return uncrossed() ? float( digitTDC1() ) : ( float( digitTDC1() + digitTDC2() ) / 2.0F );
    }

    /// Average Time of the coord in ns
    [[nodiscard]] float AverageTime_ns() const { return 25.0F / 16.0F * AverageTime(); };

    /// Time spread of the coord in TDC bit unit
    [[nodiscard]] float timeSpread() const { return uncrossed() ? 0.F : ( float( digitTDC1() - digitTDC2() ) / 2.0F ); }

    /// Time spread of the coord in ns
    [[nodiscard]] float timeSpread_ns() const { return 25.0F / 16.0F * timeSpread(); }

    /// Retrieve const  list of address of MuonDgit used to cretate this MuonCoord
    [[nodiscard]] const std::vector<LHCb::MuonTileID>& digitTile() const { return m_digitTile; }

    /// Retrieve const  TDC time of the first digit
    [[nodiscard]] unsigned int digitTDC1() const { return m_digitTDC1; }

    /// Retrieve const  TDC time of the second (if present) digit
    [[nodiscard]] unsigned int digitTDC2() const { return m_digitTDC2; }

    friend std::ostream& operator<<( std::ostream& str, const MuonCoord& obj ) { return obj.fillStream( str ); }

  private:
    std::vector<LHCb::MuonTileID> m_digitTile;    ///< list of address of MuonDgit used to cretate this MuonCoord
    unsigned int                  m_digitTDC1{0}; ///< TDC time of the first digit
    unsigned int                  m_digitTDC2{0}; ///< TDC time of the second (if present) digit

  }; // class MuonCoord

  /// Definition of Keyed Container for MuonCoord
  using MuonCoords = KeyedContainer<MuonCoord, Containers::HashMap>;

  inline std::ostream& MuonCoord::fillStream( std::ostream& s ) const {
    s << "{ "
      << "digitTile :	" << m_digitTile << std::endl
      << "digitTDC1 :	" << m_digitTDC1 << std::endl
      << "digitTDC2 :	" << m_digitTDC2 << std::endl
      << " }";
    return s;
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

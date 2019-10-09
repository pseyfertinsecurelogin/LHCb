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
#include "Kernel/MuonTileID.h"
#include <ostream>
#include <string>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_L0MuonData = 6003;

  // Namespace for locations in TDS
  namespace L0MuonDataLocation {
    inline const std::string Default = "Trig/L0/MuonData";
  }

  /** @class L0MuonData L0MuonData.h
   *
   * L0Muon trigger data * Lists of muon tiles received by a L0Muon processing
   * unit.
   *
   * @author J. Cogan
   *
   */

  class L0MuonData final : public KeyedObject<LHCb::MuonTileID> {
  public:
    /// typedef for KeyedContainer of L0MuonData
    typedef KeyedContainer<L0MuonData, Containers::HashMap> Container;

    /// Constructor with full information
    L0MuonData( const LHCb::MuonTileID& pu, const std::vector<LHCb::MuonTileID>& ols,
                const std::vector<LHCb::MuonTileID>& neighs )
        : KeyedObject<LHCb::MuonTileID>( pu ), m_ols( ols ), m_neighs( neighs ) {}

    /// Default Constructor
    L0MuonData() : m_ols(), m_neighs() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  MuonTileIDs of the hit received by optical links
    const std::vector<LHCb::MuonTileID>& ols() const;

    /// Retrieve const  MuonTileIDs of the hit send by neihgbouring PUs
    const std::vector<LHCb::MuonTileID>& neighs() const;

    friend std::ostream& operator<<( std::ostream& str, const L0MuonData& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    std::vector<LHCb::MuonTileID> m_ols;    ///< MuonTileIDs of the hit received by optical links
    std::vector<LHCb::MuonTileID> m_neighs; ///< MuonTileIDs of the hit send by neihgbouring PUs

  }; // class L0MuonData

  /// Definition of Keyed Container for L0MuonData
  typedef KeyedContainer<L0MuonData, Containers::HashMap> L0MuonDatas;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::L0MuonData::clID() const { return LHCb::L0MuonData::classID(); }

inline const CLID& LHCb::L0MuonData::classID() { return CLID_L0MuonData; }

inline std::ostream& LHCb::L0MuonData::fillStream( std::ostream& s ) const {
  s << "{ "
    << "ols :	" << m_ols << std::endl
    << "neighs :	" << m_neighs << std::endl
    << " }";
  return s;
}

inline const std::vector<LHCb::MuonTileID>& LHCb::L0MuonData::ols() const { return m_ols; }

inline const std::vector<LHCb::MuonTileID>& LHCb::L0MuonData::neighs() const { return m_neighs; }

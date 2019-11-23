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
#include "Event/PackMuonDigit.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "Kernel/MuonTileID.h"
#include "Kernel/Packer.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MuonDigit = 11021;

  // Namespace for locations in TDS
  namespace MuonDigitLocation {
    inline const std::string MuonDigit = "Raw/Muon/Digits";
  }

  /** @class MuonDigit MuonDigit.h
   *
   * Information on fired logical channel, this class represents the information
   * that the DAQ outputs on the real data The key associated to each object is
   * the channel ID coded as a MuonTileID object
   *
   * @author Alessia Satta
   *
   */

  class MuonDigit final : public KeyedObject<MuonTileID> {
  public:
    /// typedef for KeyedContainer of MuonDigit
    typedef KeyedContainer<MuonDigit, Containers::HashMap> Container;

    /// description of constructor
    MuonDigit( const MuonTileID& tileID ) : m_MuonDAQ() { setKey( tileID ); }

    /// Default Constructor
    MuonDigit() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override { return MuonDigit::classID(); }
    static const CLID&        classID() { return CLID_MuonDigit; }

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override {
      s << "{ "
        << "MuonDAQ :	" << m_MuonDAQ << std::endl
        << " }";
      return s;
    }

    /// time stamp of the digit inside the BX length  (3 bits reserved -> 3ns resolution)
    unsigned int TimeStamp() const {
      return Packer::getBit( m_MuonDAQ, PackMuonDigit::maskTimeStamp, PackMuonDigit::shiftTimeStamp );
    }

    /// set the Time  Stamp in the DAQ word
    MuonDigit& setTimeStamp( unsigned int timeStamp ) {
      Packer::setBit( m_MuonDAQ, PackMuonDigit::shiftTimeStamp, PackMuonDigit::maskTimeStamp, timeStamp );
      return *this;
    }

    friend std::ostream& operator<<( std::ostream& str, const MuonDigit& obj ) { return obj.fillStream( str ); }

  private:
    unsigned int m_MuonDAQ{0}; ///< DAQ output bit pattern

  }; // class MuonDigit

  /// Definition of Keyed Container for MuonDigit
  using MuonDigits = KeyedContainer<MuonDigit, Containers::HashMap>;

} // namespace LHCb

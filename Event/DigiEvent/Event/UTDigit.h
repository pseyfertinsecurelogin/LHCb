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
#include "Kernel/UTChannelID.h"
#include "Kernel/UTNames.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_UTDigit = 9003;

  // Namespace for locations in TDS
  namespace UTDigitLocation {
    inline const std::string UTDigits = "Raw/UT/Digits";
  }

  /** @class UTDigit UTDigit.h
   *
   * Upstream tracker digitization class
   *
   * @author Andy Beiter (based on code by Matthew Needham)
   *
   */

  class UTDigit final : public KeyedObject<LHCb::UTChannelID> {
  public:
    /// typedef for KeyedContainer of UTDigit
    typedef KeyedContainer<UTDigit, Containers::HashMap> Container;

    /// special constructor
    UTDigit( double charge ) : m_depositedCharge( charge ) {}

    /// copy constructor
    UTDigit( const LHCb::UTDigit& digit )
        : KeyedObject<LHCb::UTChannelID>(), m_depositedCharge( digit.m_depositedCharge ) {}

    /// Default Constructor
    UTDigit() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override;
    static const CLID&        classID();

    /// clone method
    [[nodiscard]] UTDigit* clone() const;

    /// channel = key
    [[nodiscard]] UTChannelID channelID() const;

    /// Print the digit in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// check if UT type
    [[nodiscard]] bool isUT() const;

    /// short cut for station
    [[nodiscard]] unsigned int station() const;

    /// shortcut for layer
    [[nodiscard]] unsigned int layer() const;

    /// short cut for detRegion
    [[nodiscard]] unsigned int detRegion() const;

    /// short cut for sector
    [[nodiscard]] unsigned int sector() const;

    /// short cut for strip
    [[nodiscard]] unsigned int strip() const;

    /// Print the unique sector name
    [[nodiscard]] std::string sectorName() const;

    /// Print the unique layer name
    [[nodiscard]] std::string layerName() const;

    /// Print the unique det region name
    [[nodiscard]] std::string detRegionName() const;

    /// Print the station name
    [[nodiscard]] std::string stationName() const;

    /// Retrieve const  charge deposited on strip
    [[nodiscard]] double depositedCharge() const;

    /// Update  charge deposited on strip
    void setDepositedCharge( double value );

    friend std::ostream& operator<<( std::ostream& str, const UTDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    double m_depositedCharge{0.0}; ///< charge deposited on strip

  }; // class UTDigit

  /// Definition of Keyed Container for UTDigit
  typedef KeyedContainer<UTDigit, Containers::HashMap> UTDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::UTDigit::clID() const { return LHCb::UTDigit::classID(); }

inline const CLID& LHCb::UTDigit::classID() { return CLID_UTDigit; }

inline double LHCb::UTDigit::depositedCharge() const { return m_depositedCharge; }

inline void LHCb::UTDigit::setDepositedCharge( double value ) { m_depositedCharge = value; }

inline LHCb::UTDigit* LHCb::UTDigit::clone() const { return new LHCb::UTDigit( *this ); }

inline LHCb::UTChannelID LHCb::UTDigit::channelID() const { return key(); }

inline std::ostream& LHCb::UTDigit::fillStream( std::ostream& s ) const {

  s << "{ UTDigit with key: " << (int)key() << " channel: " << sectorName() << " strip " << strip()
    << " depositedCharge :" << (float)m_depositedCharge << std::endl
    << " }";
  return s;
}

inline bool LHCb::UTDigit::isUT() const { return channelID().isUT(); }

inline unsigned int LHCb::UTDigit::station() const { return channelID().station(); }

inline unsigned int LHCb::UTDigit::layer() const { return channelID().layer(); }

inline unsigned int LHCb::UTDigit::detRegion() const { return channelID().detRegion(); }

inline unsigned int LHCb::UTDigit::sector() const { return channelID().sector(); }

inline unsigned int LHCb::UTDigit::strip() const { return channelID().strip(); }

inline std::string LHCb::UTDigit::sectorName() const { return ( UTNames().UniqueSectorToString( channelID() ) ); }

inline std::string LHCb::UTDigit::layerName() const { return ( UTNames().UniqueLayerToString( channelID() ) ); }

inline std::string LHCb::UTDigit::detRegionName() const { return ( UTNames().UniqueRegionToString( channelID() ) ); }

inline std::string LHCb::UTDigit::stationName() const { return ( UTNames().StationToString( channelID() ) ); }

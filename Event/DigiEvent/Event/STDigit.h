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
#include "Kernel/ITNames.h"
#include "Kernel/STChannelID.h"
#include "Kernel/TTNames.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_STDigit = 9003;

  // Namespace for locations in TDS
  namespace STDigitLocation {
    inline const std::string TTDigits = "Raw/TT/Digits";
    inline const std::string ITDigits = "Raw/IT/Digits";
  } // namespace STDigitLocation

  /** @class STDigit STDigit.h
   *
   * Silicon tracker digitization class
   *
   * @author Matthew Needham
   *
   */

  class STDigit final : public KeyedObject<LHCb::STChannelID> {
  public:
    /// typedef for KeyedContainer of STDigit
    typedef KeyedContainer<STDigit, Containers::HashMap> Container;

    /// special constructor
    STDigit( double charge ) : m_depositedCharge( charge ) {}

    /// copy constructor
    STDigit( const LHCb::STDigit& digit )
        : KeyedObject<LHCb::STChannelID>(), m_depositedCharge( digit.m_depositedCharge ) {}

    /// Default Constructor
    STDigit() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override;
    static const CLID&        classID();

    /// clone method
    [[nodiscard]] STDigit* clone() const;

    /// channel = key
    [[nodiscard]] STChannelID channelID() const;

    /// Print the digit in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// check if TT type
    [[nodiscard]] bool isTT() const;

    /// check if IT type
    [[nodiscard]] bool isIT() const;

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

    friend std::ostream& operator<<( std::ostream& str, const STDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    double m_depositedCharge{0.0}; ///< charge deposited on strip

  }; // class STDigit

  /// Definition of Keyed Container for STDigit
  typedef KeyedContainer<STDigit, Containers::HashMap> STDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::STDigit::clID() const { return LHCb::STDigit::classID(); }

inline const CLID& LHCb::STDigit::classID() { return CLID_STDigit; }

inline double LHCb::STDigit::depositedCharge() const { return m_depositedCharge; }

inline void LHCb::STDigit::setDepositedCharge( double value ) { m_depositedCharge = value; }

inline LHCb::STDigit* LHCb::STDigit::clone() const { return new LHCb::STDigit( *this ); }

inline LHCb::STChannelID LHCb::STDigit::channelID() const { return key(); }

inline std::ostream& LHCb::STDigit::fillStream( std::ostream& s ) const {

  s << "{ STDigit with key: " << (int)key() << " channel: " << sectorName() << " strip " << strip()
    << " depositedCharge :" << (float)m_depositedCharge << std::endl
    << " }";
  return s;
}

inline bool LHCb::STDigit::isTT() const { return channelID().isTT(); }

inline bool LHCb::STDigit::isIT() const { return channelID().isIT(); }

inline unsigned int LHCb::STDigit::station() const { return channelID().station(); }

inline unsigned int LHCb::STDigit::layer() const { return channelID().layer(); }

inline unsigned int LHCb::STDigit::detRegion() const { return channelID().detRegion(); }

inline unsigned int LHCb::STDigit::sector() const { return channelID().sector(); }

inline unsigned int LHCb::STDigit::strip() const { return channelID().strip(); }

inline std::string LHCb::STDigit::sectorName() const {

  return ( isTT() ? TTNames().UniqueSectorToString( channelID() ) : ITNames().UniqueSectorToString( channelID() ) );
}

inline std::string LHCb::STDigit::layerName() const {

  return ( isTT() ? TTNames().UniqueLayerToString( channelID() ) : ITNames().UniqueLayerToString( channelID() ) );
}

inline std::string LHCb::STDigit::detRegionName() const {

  return ( isTT() ? TTNames().UniqueRegionToString( channelID() ) : ITNames().UniqueBoxToString( channelID() ) );
}

inline std::string LHCb::STDigit::stationName() const {

  return ( isTT() ? TTNames().StationToString( channelID() ) : ITNames().StationToString( channelID() ) );
}

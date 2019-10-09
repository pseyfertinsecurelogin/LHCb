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
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_BcmDigit = 3001;

  // Namespace for locations in TDS
  namespace BcmDigitLocation {
    inline const std::string Default = "Raw/Bcm/Digits";
  }

  /** @class BcmDigit BcmDigit.h
   *
   * BCM digit class
   *
   * @author M Lieng
   *
   */

  class BcmDigit final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of BcmDigit
    typedef KeyedContainer<BcmDigit, Containers::HashMap> Container;

    /// Default Constructor
    BcmDigit() : m_station( 0 ), m_sensor( 0 ), m_signal( 0.0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Station number
    unsigned int station() const;

    /// Update  Station number
    void setStation( unsigned int value );

    /// Retrieve const  Sensor number
    unsigned int sensor() const;

    /// Update  Sensor number
    void setSensor( unsigned int value );

    /// Retrieve const  Signal value
    double signal() const;

    /// Update  Signal value
    void setSignal( double value );

    friend std::ostream& operator<<( std::ostream& str, const BcmDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned int m_station; ///< Station number
    unsigned int m_sensor;  ///< Sensor number
    double       m_signal;  ///< Signal value

  }; // class BcmDigit

  /// Definition of Keyed Container for BcmDigit
  typedef KeyedContainer<BcmDigit, Containers::HashMap> BcmDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::BcmDigit::clID() const { return LHCb::BcmDigit::classID(); }

inline const CLID& LHCb::BcmDigit::classID() { return CLID_BcmDigit; }

inline std::ostream& LHCb::BcmDigit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "station :	" << m_station << std::endl
    << "sensor :	" << m_sensor << std::endl
    << "signal :	" << (float)m_signal << std::endl
    << " }";
  return s;
}

inline unsigned int LHCb::BcmDigit::station() const { return m_station; }

inline void LHCb::BcmDigit::setStation( unsigned int value ) { m_station = value; }

inline unsigned int LHCb::BcmDigit::sensor() const { return m_sensor; }

inline void LHCb::BcmDigit::setSensor( unsigned int value ) { m_sensor = value; }

inline double LHCb::BcmDigit::signal() const { return m_signal; }

inline void LHCb::BcmDigit::setSignal( double value ) { m_signal = value; }

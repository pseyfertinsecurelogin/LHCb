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
#include "Kernel/VeloChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  // Namespace for locations in TDS
  namespace VeloDigitLocation {
    inline const std::string Default = "Raw/Velo/Digits";
  }

  /** @class VeloDigit VeloDigit.h
   *
   * VeloDigit is a temporary object to store the ADC value obtained during one
   * of the FPGA processing stages. The object is much more simple than
   * VeloFullFPGADigit (has only one data member)
   *
   * @author Tomasz Szumlak
   *
   */

  class VeloDigit final : public KeyedObject<LHCb::VeloChannelID> {
  public:
    /// typedef for KeyedContainer of VeloDigit
    typedef KeyedContainer<VeloDigit, Containers::HashMap> Container;

    /// constructor with key
    VeloDigit( const LHCb::VeloChannelID& key ) : KeyedObject<LHCb::VeloChannelID>( key ), m_ADCValue( 0 ) {}

    /// copy constructor
    VeloDigit( const LHCb::VeloDigit& digit ) : KeyedObject<LHCb::VeloChannelID>(), m_ADCValue( digit.m_ADCValue ) {}

    /// Default Constructor
    VeloDigit() : m_ADCValue( 0 ) {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// clone method
    VeloDigit* clone() const;

    /// combined sensor and strip number ID
    VeloChannelID channelID() const;

    /// Sensor Number
    long sensor() const;

    ///
    long strip() const;

    /// Retrieve const
    short int adcValue() const;

    /// Update
    void setADCValue( short int value );

    friend std::ostream& operator<<( std::ostream& str, const VeloDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    short int m_ADCValue; ///<

  }; // class VeloDigit

  /// Definition of Keyed Container for VeloDigit
  typedef KeyedContainer<VeloDigit, Containers::HashMap> VeloDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::VeloDigit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "ADCValue :	" << m_ADCValue << std::endl
    << " }";
  return s;
}

inline short int LHCb::VeloDigit::adcValue() const { return m_ADCValue; }

inline void LHCb::VeloDigit::setADCValue( short int value ) { m_ADCValue = value; }

inline LHCb::VeloDigit* LHCb::VeloDigit::clone() const { return new LHCb::VeloDigit( *this ); }

inline LHCb::VeloChannelID LHCb::VeloDigit::channelID() const { return key(); }

inline long LHCb::VeloDigit::sensor() const { return key().sensor(); }

inline long LHCb::VeloDigit::strip() const { return key().strip(); }

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
#include "Kernel/HCCellID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_HCDigit = 13001;

  // Namespace for locations in TDS
  namespace HCDigitLocation {
    inline const std::string Default   = "Raw/HC/Digits";
    inline const std::string L0        = "Raw/HC/L0Digits";
    inline const std::string Corrected = "Raw/HC/CorrectedDigits";
  } // namespace HCDigitLocation

  /** @class HCDigit HCDigit.h
   *
   * This class represents a Herschel PMT measurement
   *
   * @author Victor Coco
   *
   */

  class HCDigit final : public KeyedObject<LHCb::HCCellID> {
  public:
    /// typedef for KeyedContainer of HCDigit
    typedef KeyedContainer<HCDigit, Containers::HashMap> Container;

    /// Constructor
    HCDigit( int adc ) : m_adc( adc ) {}

    /// Copy constructor
    HCDigit( const LHCb::HCDigit& digit )
        : KeyedObject<LHCb::HCCellID>(), m_adc( digit.m_adc ), m_ctrlWord( digit.m_ctrlWord ), m_card( digit.m_card ) {}

    /// Default Constructor
    HCDigit() : m_adc( 0 ), m_ctrlWord( 0 ), m_card( 0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Clone method
    HCDigit* clone() const;

    /// Return the digit cellID = key
    HCCellID cellID() const;

    /// Print the digit key = cellID
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  PMT adc
    int adc() const;

    /// Update  PMT adc
    void setAdc( int value );

    /// Retrieve const  control word
    int ctrlWord() const;

    /// Update  control word
    void setCtrlWord( int value );

    /// Retrieve const  card number
    int card() const;

    /// Update  card number
    void setCard( int value );

    friend std::ostream& operator<<( std::ostream& str, const HCDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    int m_adc;      ///< PMT adc
    int m_ctrlWord; ///< control word
    int m_card;     ///< card number

  }; // class HCDigit

  /// Definition of Keyed Container for HCDigit
  typedef KeyedContainer<HCDigit, Containers::HashMap> HCDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::HCDigit::clID() const { return LHCb::HCDigit::classID(); }

inline const CLID& LHCb::HCDigit::classID() { return CLID_HCDigit; }

inline int LHCb::HCDigit::adc() const { return m_adc; }

inline void LHCb::HCDigit::setAdc( int value ) { m_adc = value; }

inline int LHCb::HCDigit::ctrlWord() const { return m_ctrlWord; }

inline void LHCb::HCDigit::setCtrlWord( int value ) { m_ctrlWord = value; }

inline int LHCb::HCDigit::card() const { return m_card; }

inline void LHCb::HCDigit::setCard( int value ) { m_card = value; }

inline LHCb::HCDigit* LHCb::HCDigit::clone() const { return new LHCb::HCDigit( *this ); }

inline LHCb::HCCellID LHCb::HCDigit::cellID() const { return key(); }

inline std::ostream& LHCb::HCDigit::fillStream( std::ostream& s ) const {

  s << "{HCDigit's key: " << (int)key() << "}";
  return s;
}

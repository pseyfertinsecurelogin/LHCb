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
#include "Kernel/CaloCellIDKeyTraits.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_CaloAdc = 2007;

  // Namespace for locations in TDS
  namespace CaloAdcLocation {
    inline const std::string Spd      = "Raw/Spd/Adcs";
    inline const std::string Prs      = "Raw/Prs/Adcs";
    inline const std::string Ecal     = "Raw/Ecal/Adcs";
    inline const std::string Hcal     = "Raw/Hcal/Adcs";
    inline const std::string EcalPin  = "Raw/Ecal/PinAdcs";
    inline const std::string HcalPin  = "Raw/Hcal/PinAdcs";
    inline const std::string FullEcal = "Raw/Ecal/FullAdcs";
    inline const std::string FullHcal = "Raw/Hcal/FullAdcs";
  } // namespace CaloAdcLocation

  /** @class CaloAdc CaloAdc.h
   *
   * @brief The ADC content for given cell * * * The class represents the
   * digitisez value * in a calorimeter cell * *
   *
   * @author Olivier Callot
   *
   */

  class CaloAdc final : public KeyedObject<LHCb::CaloCellID> {
  public:
    /// typedef for KeyedContainer of CaloAdc
    typedef KeyedContainer<CaloAdc, Containers::HashMap> Container;

    /// Non-default constructor
    CaloAdc( const LHCb::CaloCellID& id, int adc ) : KeyedObject<LHCb::CaloCellID>( id ), m_adc( adc ) {}

    /// Copy Constructor
    CaloAdc( const CaloAdc& src ) : KeyedObject<LHCb::CaloCellID>( src.cellID() ), m_adc( src.adc() ) {}

    /// Default Constructor
    CaloAdc() : m_adc( 0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve cell identifier/key @attention alias to Base::key() method!
    const LHCb::CaloCellID& cellID() const;

    /// Retrieve const  ADC value for the given cell
    int adc() const;

    /// Update  ADC value for the given cell
    void setAdc( int value );

    friend std::ostream& operator<<( std::ostream& str, const CaloAdc& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    int m_adc; ///< ADC value for the given cell

  }; // class CaloAdc

  /// Definition of Keyed Container for CaloAdc
  typedef KeyedContainer<CaloAdc, Containers::HashMap> CaloAdcs;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::CaloAdc::clID() const { return LHCb::CaloAdc::classID(); }

inline const CLID& LHCb::CaloAdc::classID() { return CLID_CaloAdc; }

inline std::ostream& LHCb::CaloAdc::fillStream( std::ostream& s ) const {
  s << "{ "
    << "adc :	" << m_adc << std::endl
    << " }";
  return s;
}

inline int LHCb::CaloAdc::adc() const { return m_adc; }

inline void LHCb::CaloAdc::setAdc( int value ) { m_adc = value; }

inline const LHCb::CaloCellID& LHCb::CaloAdc::cellID() const { return key(); }

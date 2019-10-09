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
  static const CLID CLID_L0CaloAdc = 6006;

  // Namespace for locations in TDS
  namespace L0CaloAdcLocation {
    inline const std::string Ecal    = "Trig/L0/EcalRaw";
    inline const std::string Hcal    = "Trig/L0/HcalRaw";
    inline const std::string EcalPin = "Trig/L0/EcalPin";
    inline const std::string HcalPin = "Trig/L0/HcalPin";
  } // namespace L0CaloAdcLocation

  /** @class L0CaloAdc L0CaloAdc.h
   *
   * Information for L0 Calo Trigger: CellID + 8-bit Et
   *
   * @author Olivier Callot
   *
   */

  class L0CaloAdc final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of L0CaloAdc
    typedef KeyedContainer<L0CaloAdc, Containers::HashMap> Container;

    /// Usual constructor, specifies the cell and Et
    L0CaloAdc( const CaloCellID& ID, int adc ) : KeyedObject<int>( ID.all() ), m_adc( adc ) {}

    /// Copy Constructor
    L0CaloAdc( const L0CaloAdc& src ) : KeyedObject<int>( src.cellID().all() ), m_adc( src.adc() ) {}

    /// Default Constructor
    L0CaloAdc() : m_adc( 0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve cell identifier/key @attention alias to Base::key() method!
    const LHCb::CaloCellID cellID() const;

    /// Retrieve const  8-bit transverse energy
    int adc() const;

    friend std::ostream& operator<<( std::ostream& str, const L0CaloAdc& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    int m_adc; ///< 8-bit transverse energy

  }; // class L0CaloAdc

  /// Definition of Keyed Container for L0CaloAdc
  typedef KeyedContainer<L0CaloAdc, Containers::HashMap> L0CaloAdcs;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::L0CaloAdc::clID() const { return LHCb::L0CaloAdc::classID(); }

inline const CLID& LHCb::L0CaloAdc::classID() { return CLID_L0CaloAdc; }

inline std::ostream& LHCb::L0CaloAdc::fillStream( std::ostream& s ) const {
  s << "{ "
    << "adc :	" << m_adc << std::endl
    << " }";
  return s;
}

inline int LHCb::L0CaloAdc::adc() const { return m_adc; }

inline const LHCb::CaloCellID LHCb::L0CaloAdc::cellID() const { return LHCb::CaloCellID( key() ); }

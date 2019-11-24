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
#include <set>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_CaloDigit = 2002;

  // Namespace for locations in TDS
  namespace CaloDigitLocation {
    inline const std::string Default        = "Raw/Calo/Digits";
    inline const std::string Spd            = "Raw/Spd/Digits";
    inline const std::string Prs            = "Raw/Prs/Digits";
    inline const std::string Ecal           = "Raw/Ecal/Digits";
    inline const std::string Hcal           = "Raw/Hcal/Digits";
    inline const std::string UnfilteredEcal = "Raw/Ecal/UnfilteredDigits";
    inline const std::string UnfilteredHcal = "Raw/Hcal/UnfilteredDigits";
    inline const std::string FullSpd        = "Raw/Spd/AllDigits";
    inline const std::string FullPrs        = "Raw/Prs/AllDigits";
    inline const std::string FullEcal       = "Raw/Ecal/AllDigits";
    inline const std::string FullHcal       = "Raw/Hcal/AllDigits";
    inline const std::string Hlt1Spd        = "Raw/Spd/Hlt1Digits";
    inline const std::string Hlt1Prs        = "Raw/Prs/Hlt1Digits";
    inline const std::string Hlt1Ecal       = "Raw/Ecal/Hlt1Digits";
    inline const std::string Hlt1Hcal       = "Raw/Hcal/Hlt1Digits";
  } // namespace CaloDigitLocation

  /** @class CaloDigit CaloDigit.h
   *
   * @brief The energy deposition for given cell * * * The class represents the
   * energy deposition * in the calorimeter cell * *
   *
   * @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *
   */

  class CaloDigit final : public KeyedObject<LHCb::CaloCellID> {
  public:
    /// typedef for KeyedContainer of CaloDigit
    typedef KeyedContainer<CaloDigit, Containers::HashMap> Container;

    /// vector of (const) Calo-digits
    using Vector = std::vector<const LHCb::CaloDigit*>;
    /// the set of unique (const) Calo-digits
    using Set = std::set<const LHCb::CaloDigit*>;

    /// Non-default constructor
    CaloDigit( const LHCb::CaloCellID& id, double e ) : Base( id ), m_e( e ) {}

    /// Default constructor
    CaloDigit() = default;

    /// Copy Constructor
    CaloDigit( const CaloDigit& right ) : Base( right.cellID() ), m_e( right.e() ) {}
    CaloDigit& operator=( const CaloDigit& ) = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override;
    static const CLID&        classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve cell identifier/key @attention alias to Base::key() method!
    [[nodiscard]] const LHCb::CaloCellID& cellID() const;

    /// update cell identifier/key @attention alias to Base::setKey() method!
    void setCellID( const LHCb::CaloCellID& CellID );

    /// Retrieve const  Energy deposition in the given cell
    [[nodiscard]] double e() const;

    /// Update  Energy deposition in the given cell
    void setE( double value );

    friend std::ostream& operator<<( std::ostream& str, const CaloDigit& obj ) { return obj.fillStream( str ); }

  protected:
    /// Shortcut for own base class
    using Base = KeyedObject<LHCb::CaloCellID>;

  private:
    double m_e{0}; ///< Energy deposition in the given cell

  }; // class CaloDigit

  /// Definition of Keyed Container for CaloDigit
  typedef KeyedContainer<CaloDigit, Containers::HashMap> CaloDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::CaloDigit::clID() const { return LHCb::CaloDigit::classID(); }

inline const CLID& LHCb::CaloDigit::classID() { return CLID_CaloDigit; }

inline std::ostream& LHCb::CaloDigit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "e :	" << (float)m_e << std::endl
    << " }";
  return s;
}

inline double LHCb::CaloDigit::e() const { return m_e; }

inline void LHCb::CaloDigit::setE( double value ) { m_e = value; }

inline const LHCb::CaloCellID& LHCb::CaloDigit::cellID() const { return key(); }

inline void LHCb::CaloDigit::setCellID( const LHCb::CaloCellID& CellID ) { setKey( CellID ); }

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
#include "Event/MCHit.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCExtendedHit = 231;

  /** @class MCExtendedHit MCExtendedHit.h
   *
   * Extend MCHit with momentum of particle at entry plane
   *
   * @author G.Corti
   *
   */

  class MCExtendedHit : public LHCb::MCHit {
  public:
    /// Default Constructor
    MCExtendedHit() : m_momentum( 0.0, 0.0, 0.0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// pxpz slope at entry
    double pxpz() const;

    /// pypz slope at entry
    double pypz() const;

    /// Retrieve const  momentum-vector of particle when entering the detector
    const Gaudi::XYZVector& momentum() const;

    /// Update  momentum-vector of particle when entering the detector
    void setMomentum( const Gaudi::XYZVector& value );

    friend std::ostream& operator<<( std::ostream& str, const MCExtendedHit& obj ) { return obj.fillStream( str ); }

  private:
    Gaudi::XYZVector m_momentum; ///< momentum-vector of particle when entering the detector

  }; // class MCExtendedHit

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCExtendedHit::clID() const { return LHCb::MCExtendedHit::classID(); }

inline const CLID& LHCb::MCExtendedHit::classID() { return CLID_MCExtendedHit; }

inline std::ostream& LHCb::MCExtendedHit::fillStream( std::ostream& s ) const {
  LHCb::MCHit::fillStream( s );
  s << "{ "
    << "momentum :	" << m_momentum << std::endl
    << " }";
  return s;
}

inline const Gaudi::XYZVector& LHCb::MCExtendedHit::momentum() const { return m_momentum; }

inline void LHCb::MCExtendedHit::setMomentum( const Gaudi::XYZVector& value ) { m_momentum = value; }

inline double LHCb::MCExtendedHit::pxpz() const {

  double pxpz = 9999.;
  if ( 1.e-4 * Gaudi::Units::MeV < fabs( this->momentum().z() ) ) {
    pxpz = this->momentum().x() / this->momentum().z();
  }
  return pxpz;
}

inline double LHCb::MCExtendedHit::pypz() const {

  double pypz = 9999.;
  if ( 1.e-4 * Gaudi::Units::MeV < fabs( this->momentum().z() ) ) {
    pypz = this->momentum().y() / this->momentum().z();
  }
  return pypz;
}

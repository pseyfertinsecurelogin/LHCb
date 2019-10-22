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
#include "Kernel/RichSmartID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class RichSummaryPhoton RichSummaryPhoton.h
   *
   * RICH reconstruction summary object representing a single reconstructed
   * Cherenkov photon
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class RichSummaryPhoton final {
  public:
    /// Vector of RichSummaryPhotons
    using Vector = std::vector<LHCb::RichSummaryPhoton>;

    /// Default Constructor
    RichSummaryPhoton() = default;

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  The reconstructed Cherenkov theta value for this photon
    [[nodiscard]] float cherenkovTheta() const;

    /// Update  The reconstructed Cherenkov theta value for this photon
    void setCherenkovTheta( float value );

    /// Retrieve const  The reconstructed Cherenkov phi value for this photon
    [[nodiscard]] float cherenkovPhi() const;

    /// Update  The reconstructed Cherenkov phi value for this photon
    void setCherenkovPhi( float value );

    /// Retrieve const  RichSmartID for the RICH hit associated to this reconstructed photon
    [[nodiscard]] const LHCb::RichSmartID& smartID() const;

    /// Update  RichSmartID for the RICH hit associated to this reconstructed photon
    void setSmartID( const LHCb::RichSmartID& value );

    friend std::ostream& operator<<( std::ostream& str, const RichSummaryPhoton& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    float             m_cherenkovTheta{-999}; ///< The reconstructed Cherenkov theta value for this photon
    float             m_cherenkovPhi{-999};   ///< The reconstructed Cherenkov phi value for this photon
    LHCb::RichSmartID m_smartID;              ///< RichSmartID for the RICH hit associated to this reconstructed photon

  }; // class RichSummaryPhoton

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::RichSummaryPhoton::fillStream( std::ostream& s ) const {
  s << "{ "
    << "cherenkovTheta :	" << m_cherenkovTheta << std::endl
    << "cherenkovPhi :	" << m_cherenkovPhi << std::endl
    << "smartID :	" << m_smartID << std::endl
    << " }";
  return s;
}

inline float LHCb::RichSummaryPhoton::cherenkovTheta() const { return m_cherenkovTheta; }

inline void LHCb::RichSummaryPhoton::setCherenkovTheta( float value ) { m_cherenkovTheta = value; }

inline float LHCb::RichSummaryPhoton::cherenkovPhi() const { return m_cherenkovPhi; }

inline void LHCb::RichSummaryPhoton::setCherenkovPhi( float value ) { m_cherenkovPhi = value; }

inline const LHCb::RichSmartID& LHCb::RichSummaryPhoton::smartID() const { return m_smartID; }

inline void LHCb::RichSummaryPhoton::setSmartID( const LHCb::RichSmartID& value ) { m_smartID = value; }

/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

//-----------------------------------------------------------------------------
/** @file RichDetectorType.h
 *
 *  Header file for RICH particle ID enumeration : RichDetectorType
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   08/07/2004
 */
//-----------------------------------------------------------------------------

#pragma once

// std include
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

//  General namespace for RICH specific definitions documented in RichSide.h
namespace Rich {

  /// Number of RICH detectors
  static const uint8_t NRiches = 2;

  /** @enum Rich::DetectorType
   *
   *  RICH Detector types
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   08/07/2004
   */
  enum DetectorType : int8_t {
    InvalidDetector = -1, ///< Unspecified Detector
    Rich1           = 0,  ///< RICH1 detector
    Rich2           = 1,  ///< RICH2 detector
    Rich            = 1,  ///< Single RICH detector
    TRID            = 1   ///< In case the name TRID catches on
  };

  /** Text conversion for DetectorType enumeration
   *
   *  @param detector RICH detector enumeration
   *  @return Detector type as a string
   */
  std::string text( const Rich::DetectorType detector );

  /// Type for container of detector types
  typedef std::vector<Rich::DetectorType> Detectors;
  // using Detectors = std::vector<Rich::DetectorType>;

  /// Access a vector of valid detector types
  const Detectors& detectors() noexcept;

  /// Implement textual ostream << method for Rich::DetectorType enumeration
  inline std::ostream& operator<<( std::ostream& s, const Rich::DetectorType& detector ) {
    return s << Rich::text( detector );
  }

  /// Type for fixed size arrays with RICH detector information
  template <typename TYPE>
  using DetectorArray = std::array<TYPE, NRiches>;

} // namespace Rich

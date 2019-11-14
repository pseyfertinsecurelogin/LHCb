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
/** @file RichSide.h
 *
 *  Header file for RICH particle ID enumeration : Rich::Side
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   08/07/2004
 */
//-----------------------------------------------------------------------------

#pragma once

// STL files
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Kernel
#include "Kernel/RichDetectorType.h"

namespace Rich {

  /// Total number of PD panels
  inline constexpr uint16_t NTotalPDPanels = 4;

  /// Number of PD panels per RICH detector
  inline constexpr uint16_t NPDPanelsPerRICH = 2;

  /** @enum Rich::Side
   *
   *  RICH detector side enumeration. Used to indentify the "side" of
   *  each of the RICH detectors.
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   08/07/2004
   */
  enum Side : int32_t {
    InvalidSide = -1, ///< Invalid side
                      // RICH1
    top    = 0,       ///< Upper panel in RICH1
    bottom = 1,       ///< Lower panel in RICH1
                      // RICH2
    left  = 0,        ///< Left panel in RICH2
    right = 1,        ///< Right panel in RICH2
                      // Generic
    firstSide  = 0,   ///< Upper panel in RICH1 or Left panel in RICH2
    secondSide = 1    ///< Lower panel in RICH1 or Right panel in RICH2
  };

  /** Text conversion for Rich::Side enumeration
   *
   *  @param side Rich Side enumeration
   *  @return Rich Side as an std::string
   */
  std::string text( const Rich::Side side );

  /** Text conversion for Rich::DetectorType and Rich::Side enumeration
   *
   *  @param rich Rich Detector
   *  @param side Rich Side enumeration
   *  @return Rich Side as an std::string
   */
  std::string text( const Rich::DetectorType rich, const Rich::Side side );

  /// Type for container of of side types
  using Sides = std::vector<Side>;

  /// Access a vector of valid particle ID types
  const Sides& sides() noexcept;

  /// Implement textual ostream << method for Rich::Side enumeration
  inline std::ostream& operator<<( std::ostream& s, const Rich::Side& side ) { return s << Rich::text( side ); }

  /// Type for fixed size arrays with RICH panel information
  template <typename TYPE>
  using PanelArray = std::array<TYPE, NPDPanelsPerRICH>;

} // namespace Rich

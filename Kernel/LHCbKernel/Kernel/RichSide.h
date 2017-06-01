
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
#include <string>
#include <vector>
#include <iostream>
#include <cstdint>
#include <array>

// Kernel
#include "Kernel/RichDetectorType.h"

namespace Rich
{

  /// Total number of PD panels
  static const uint8_t NTotalPDPanels   = 4;

  /// Number of PD panels per RICH detector
  static const uint8_t NPDPanelsPerRICH = 2;

  /** @enum Rich::Side
   *
   *  RICH detector side enumeration. Used to indentify the "side" of
   *  each of the RICH detectors.
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   08/07/2004
   */
  enum Side : int8_t
    {
      InvalidSide  = -1, ///< Invalid side
      top          = 0,  ///< Upper panel in RICH1
      bottom       = 1,  ///< Lower panel in RICH1
      left         = 0,  ///< Left panel in RICH2
      right        = 1   ///< Right panel in RICH2
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
  typedef std::vector<Side> Sides;
  //using Sides = std::vector<Side>;

  /// Access a vector of valid particle ID types
  const Sides & sides() noexcept;

  /// Implement textual ostream << method for Rich::Side enumeration
  inline std::ostream& operator << ( std::ostream & s,
                                     const Rich::Side & side )
  {
    return s << Rich::text( side );
  }

  /// Type for fixed size arrays with RICH panel information
  template < typename TYPE >
  using PanelArray = std::array< TYPE, NPDPanelsPerRICH >;

}

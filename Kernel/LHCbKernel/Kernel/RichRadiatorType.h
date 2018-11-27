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

//=================================================================================
/** @file RichRadiatorType.h
 *
 *  Header file for RICH particle ID enumeration : RichRadiatorType
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   08/07/2004
 */
//=================================================================================

#ifndef LHCBKERNEL_RICHRADIATORTYPE_H
#define LHCBKERNEL_RICHRADIATORTYPE_H 1

// STL
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// General namespace for RICH specific definitions documented in RichSide.h
namespace Rich
{

  /// Number of RICH radiators
  static const uint8_t NRadiatorTypes = 3;

  /** @enum Rich::RadiatorType
   *
   *  RICH radiator types
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   08/07/2004
   */
  enum RadiatorType : int8_t
  {
    InvalidRadiator = -1, ///< Unspecified radiator type
    Aerogel         = 0,  ///< Aerogel in RICH1
    Rich1Gas        = 1,  ///< Gaseous RICH1 radiator
    Rich2Gas        = 2,  ///< Gaseous RICH2 radiator
    C4F10           = 1,  ///< Gaseous RICH1 radiator (to be removed)
    CF4             = 2,  ///< Gaseous RICH2 radiator (to be removed)
    // background types
    GasQuartzWin  = 3, ///< Quartz windows to the gas radiator volumes
    HPDQuartzWin  = 4, ///< HPD Quartz windows
    Nitrogen      = 5, ///< Nitrogen volume
    AerogelFilter = 6, ///< Aerogel filter material
    CO2           = 7, ///< Carbon dioxide
    PMTQuartzWin  = 8  ///< MAPMT Quartz windows
  };

  /** Text conversion for RadiatorType enumeration
   *
   *  @param radiator Radiator type enumeration
   *  @return Radiator type as an std::string
   */
  std::string text( const Rich::RadiatorType radiator );

  /// Type for container of radiator types
  typedef std::vector< Rich::RadiatorType > Radiators;
  // using Radiators = std::vector<Rich::RadiatorType>;

  /// Access a vector of all radiator types
  const Radiators &radiators() noexcept;

  /// Implement textual ostream << method for Rich::RadiatorType enumeration
  inline std::ostream &operator<<( std::ostream &s, const Rich::RadiatorType radiator )
  {
    return s << Rich::text( radiator );
  }

  /// Type for fixed size arrays with radiator information
  template < typename TYPE >
  using RadiatorArray = std::array< TYPE, NRadiatorTypes >;

} // namespace Rich

#endif // LHCBKERNEL_RICHRADIATORTYPE_H
